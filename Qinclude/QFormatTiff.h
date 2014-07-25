#ifndef QFORMATTIFF_H
#define QFORMATTIFF_H

// Tagged Image Format.  File extensions: .tif .tiff.  This is just about complete implementation of TIFF
// format on the level of "Revision 6.0".  The format implies that a TIFF file can enclose arbitrary number of
// images; an image of such is called Image File Directory (IFD) or subfile and maintains its own independent
// characteristics, that leaves for the container-file to summarize only number of stored IFDs, common
// strip/tile orientation and little/big-endian byte order upon the whole.  For this reason, the focus is
// shifted from interaction with FormatTiff class towards interfacing with its internal class
// ImageFileDirectory whose instances can be either inserted to or retrieved from a FormatTiff object.
// Information associated with each IFD is divided among IFD fields, also known as tags.  When saving, critical
// fields are assigned implicitly, but ancillary ones are controlled by the optional set-calls; for loading, an
// IFD field can be accessed only if it is present in the file or has the default value(s) encouraged by the
// specification.  The specification states that any textual IFD field can contain multiple strings, so an
// arbitrary sized array of text strings (represented by FormatTiff's type StringArray) can be passed for
// saving and be got on loading; however, usage of solitary strings is recommended.  The term "straight YCbCr
// image" means uncompressed non-JPEG YCbCr image.  The most notable features of this TIFF implementation are:
//   endianness is of no importance for both directions, as well as bit order within bytes when loading;
//   strip- and tile-oriented saving and loading are supported;
//   the class accepts any combination of bits-per-sample values in the valid ranges;
//   the implementation is optimized for images with 8 bits per unsigned integer sample;
//   the full range of compression schemes is available for both directions;
//   an image with components organized plane by plane is readable;
//   support for saving and loading of CIE L*a*b* images;
//   support for loading of straight YCbCr and "TIFF's paletted-with-alpha" images;
//   transparency information is written and read correctly, relying on the fact of alpha-premultiplication;
//   a visibility mask can be saved or loaded;
//   support for multiple strings in a single text field.
//
// For saving, acceptable RasterImage's color models are: CMT_PALETTED, CMT_GRAYSCALE,
// CMT_GRAYSCALE_WITH_ALPHA, CMT_RGB, CMT_RGBA, CMT_CMYK, CMT_LAB, and CMT_LAB_WITH_ALPHA.  To create a TIFF
// image with floating-point sample type, input image type must imply floating-point samples beforehand.  A
// bits-per-sample value must be in the range [1, 24] or be 32 for floating-point TIFFs; however, with L*a*b*
// images only bps 8, 16, or 32 can be used evenly and JPEG compression works with 8 bps only.  For paletted
// images, the bps value (which is also bpp) must allow image's pixels to address any entry in the palette,
// making true the next: 2^bpp >= palette_size.  NOTE. In addition to paletted images, common graphical
// applications mainly support TIFF images with 8 and 16 bps evenly.  Visibility masks are supported; the
// format's design implies storage of an image having a visibility mask solely, without any other IFDs.
// Transparency information is written in the associated form, that is with color components premultiplied by
// the alpha level.  Filtering is to be applied if compression is set to CT_LZW or CT_ZLIB.  Fax encoding
// doesn't use "uncompressed modes", "K factor" is 4.  IFD field PlanarConfiguration is written if there is
// more than one sample per pixel.
// DEFAULTS. Except critical IFD fields, the next ones are present in a saved file: NewSubFileType field with
// all boolean flags unset ("not a reduced resolution version", "not a single page of multipage image"),
// Compression field set to CT_NONE, resolution related fields representing 72 dpi both horizontally and
// vertically.
//
// For loading, an IFD image can appear with type IT_PALETTED, IT_GRAYSCALE_8, IT_GRAYSCALE_16,
// IT_GRAYSCALE_FLOAT, IT_GRAYSCALE_WITH_ALPHA_8_8, IT_GRAYSCALE_WITH_ALPHA_16_16,
// IT_GRAYSCALE_WITH_ALPHA_FLOAT, IT_RGB_8_8_8, IT_RGB_16_16_16, IT_RGB_FLOAT, IT_RGBA_8_8_8_8,
// IT_RGBA_16_16_16_16, IT_RGBA_FLOAT, IT_CMYK_8_8_8_8, IT_CMYK_16_16_16_16, IT_CMYK_FLOAT, IT_LAB_8_8_8,
// IT_LAB_16_16_16, IT_LAB_FLOAT, IT_LAB_WITH_ALPHA_8_8_8_8, IT_LAB_WITH_ALPHA_16_16_16_16, or
// IT_LAB_WITH_ALPHA_FLOAT.  An image can arrive with or without a visibility mask.  A TIFF file is considered
// as "normal" if there is at least one IFD which doesn't break the rules stated by the specification, and if
// some number of bad IFDs is present, the warning is thrown and only good ones are exposed.  Similarly is for
// IFD fields: incorrect ancillary fields are banned pretending their absence.  WARNING. If an image, which is
// about to be loaded, has one of the next orientation types: OT_LEFT_TOP, OT_RIGHT_TOP, OT_RIGHT_BOTTOM, or
// OT_LEFT_BOTTOM, then an arrived RasterImage object will have width and height different from those returned
// by GetWidth and GetHeight member functions, because of a rotation on 90 degrees.  Images with JPEG
// compression, straight YCbCr, and "TIFF's paletted-with-alpha" images are supported only within chunky
// component distribution scheme.  Straight YCbCr images on loading become RGB; upsampling of both chromaticity
// channels uses the bilinear filter; loading respects custom YCbCr coefficients if they are present in the
// file; although the specification says that for straight YCbCr images ReferenceBlackWhite field is required,
// loading doesn't take it too seriously.  Fax decoding supports "uncompressed modes".  Signed integer type of
// samples is treated very similarly to unsigned one, that is the full range of values is assumed to be valid;
// floating-point type supposed to have minimal value 0.0 and maximal value 1.0 (except for L*a*b*).  The
// specification isn't quite clear about meaning of SMinSampleValue and SMaxSampleValue fields, so, accordingly
// to some hints, clamping/normalizing role is understood.

#include "QProgressBase.h"
#include "QRasterImage.h"
#include "QFormatExif.h"
#include "QDate.h"

namespace Q
{

class Q_ITEM FormatTiff : public FormatBase, public ProgressBase
{
public:
    // relates only to saving; both schemes of pixel data segmentation are intended to ease image reading by
    // those applications and devices which have certain memory limitations; tentative size of a strip/tile is
    // indicated by ms_iKBytesPerSegment constant
    enum SegmentOrganizationType
    {
        SOT_STRIP_ORIENTED,  // pixel data are divided on horizontal stripes
        SOT_TILE_ORIENTED    // pixel data are divided on rectangular tiles
    };

    enum ByteOrderType
    {
        BOT_LITTLE_ENDIAN,
        BOT_BIG_ENDIAN  // network byte order
    };

    // for fax encoding types below, "one-dimensional" means that every row of pixels is compressed separately,
    // but "two-dimensional" signifies that each next row is coded relatively to the previous one;
    // two-dimensional approach is able to rise compression efficiency much for an image containing
    // predominantly words
    enum CompressionType
    {
        CT_NONE,
        CT_MODIFIED_HUFFMAN,  // fax encoding, only for bilevel images, one-dimensional
        CT_GROUP_3,           // fax encoding, only for bilevel images, can be one- or two-dimensional
        CT_GROUP_4,           // fax encoding, only for bilevel images, two-dimensional
        CT_RLE,               // PackBits compression scheme, for all image kinds
        CT_LZW,               // LZW, for all image kinds
        CT_ZLIB,              // ZIP, for all image kinds
        CT_JPEG_TNT           // renewed JPEG compression, only for non-paletted images without transparency
    };

    enum ResolutionUnitType
    {
        RUT_NO_ABSOLUTE_UNIT_OF_MEASUREMENT,
        RUT_INCH,
        RUT_CENTIMETER
    };

    enum OrientationType
    {
        OT_TOP_LEFT,      // 0th row is at the image's visual top, 0th column is the visual left-hand side
        OT_TOP_RIGHT,     // 0th row is at the image's visual top, 0th column is the visual right-hand side
        OT_BOTTOM_RIGHT,  // 0th row is at the image's visual bottom, 0th column is the visual right-hand side
        OT_BOTTOM_LEFT,   // 0th row is at the image's visual bottom, 0th column is the visual left-hand side
        OT_LEFT_TOP,      // 0th row is the image's visual left-hand side, 0th column is the visual top
        OT_RIGHT_TOP,     // 0th row is the image's visual right-hand side, 0th column is the visual top
        OT_RIGHT_BOTTOM,  // 0th row is the image's visual right-hand side, 0th column is the visual bottom
        OT_LEFT_BOTTOM    // 0th row is the image's visual left-hand side, 0th column is the visual bottom
    };

    enum ThreshholdingType
    {
        TT_NO_DITHERING_OR_HALFTONING_APPLIED,
        TT_ORDERED_DITHERING_OR_HALFTONING_APPLIED,
        TT_RANDOMIZED_DITHERING_OR_HALFTONING_APPLIED
    };

    enum GrayResponseUnitType
    {
        GRUT_TENTHS_OF_UNIT,
        GRUT_HUNDREDTHS_OF_UNIT,
        GRUT_THOUSANDTHS_OF_UNIT,
        GRUT_TEN_THOUSANDTHS_OF_UNIT,
        GRUT_HUNDRED_THOUSANDTHS_OF_UNIT
    };

    // relates only to loading
    enum ColorModelType
    {
        CMT_BILEVEL,  // black-and-white (also known as "bitmap") with 1 bit per pixel
        CMT_PALETTED,
        CMT_GRAYSCALE,
        CMT_GRAYSCALE_WITH_ALPHA,
        CMT_RGB,
        CMT_RGBA,
        CMT_CMYK,
        CMT_LAB,
        CMT_LAB_WITH_ALPHA,
        CMT_YCC       // YCbCr: straight YCbCr or JPEG-compressed
    };

    // relates only to loading
    enum SubfileType
    {
        ST_FULL_RESOLUTION_IMAGE,
        ST_REDUCED_RESOLUTION_IMAGE,
        ST_SINGLE_PAGE_OF_MULTIPAGE_IMAGE
    };

    // relates only to loading
    enum InkSetType
    {
        IST_CMYK,
        IST_NOT_CMYK
    };

    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,              // saving and loading
        ET_FORMAT_IDENTIFIER_MISMATCH,  // loading
        ET_FORMAT_MISMATCH,             // loading
        ET_ZLIB_EXCEPTION                   // loading
    };

    // WT_ONE_OR_MORE_IFD_FIELDS_SKIPPED: some ancillary IFD field(s) has a content not allowed by the
    //   specification
    // WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD_DUE_TO_UNSUPPORTED_NUMBER_OF_SAMPLES_PER_PIXEL: the
    //   samples-per-pixel number is out of the supported range [1, 4]
    // WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD_DUE_TO_SAMPLE_FORMAT_NONUNIFORMITY: cases like when a pixel has
    //   the first sample of unsigned integer type, the second sample of signed integer type, but the third one
    //   is of floating-point type -- not supported
    // WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD_DUE_TO_OBSOLETE_JPEG_COMPRESSION: unsuccessful original JPEG
    //   compression scheme isn't supported
    enum WarningType
    {
        WT_ONE_OR_MORE_IFD_FIELDS_SKIPPED,                                                      // loading
        WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD_DUE_TO_UNSUPPORTED_NUMBER_OF_SAMPLES_PER_PIXEL,  // loading
        WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD_DUE_TO_SAMPLE_FORMAT_NONUNIFORMITY,              // loading
        WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD_DUE_TO_OBSOLETE_JPEG_COMPRESSION,                // loading
        WT_ONE_OR_MORE_SUBFILES_FAILED_TO_LOAD,                                                 // loading
        WT_EXIF_METADATA_ARE_INCORRECT                                                          // loading
    };

    // TIFF internal types
    typedef unsigned char Byte;
    typedef unsigned short Short;
    typedef unsigned int Long;
    typedef char SByte;
    typedef char Undefined;
    typedef short SShort;
    typedef int SLong;
    typedef float Float;
    typedef double Double;

    // to hold textual data
    typedef StillArray<String> StringArray;

private:  // this section defines things used by ImageFileDirectory internally, with implicit influence
    // private
    enum PhotometricInterpretationType
    {
        PIT_WHITE_IS_ZERO,
        PIT_BLACK_IS_ZERO,
        PIT_RGB,
        PIT_PALETTED,
        PIT_TRANSPARENCY_MASK,
        PIT_CMYK,
        PIT_YCC,
        PIT_CIE_LAB
    };

    // private
    enum FillOrderType
    {
        FOT_HIGH_ORDER_BIT_FIRST,
        FOT_LOW_ORDER_BIT_FIRST
    };

    // private
    enum PlanarConfigurationType
    {
        PCT_CHUNKY_FORMAT,
        PCT_PLANAR_FORMAT
    };

    // private
    enum PredictorType
    {
        PT_NO_PREDICTION,
        PT_HORIZONTAL_DIFFERENCING
    };

    // private
    enum ExtraSampleType
    {
        EST_UNSPECIFIED,
        EST_ASSOCIATED_ALPHA,
        EST_UNASSOCIATED_ALPHA
    };

    // private
    enum SampleFormatType
    {
        SFT_UNSIGNED_INTEGER,
        SFT_SIGNED_INTEGER,
        SFT_FLOATING_POINT,
        SFT_UNDEFINED
    };

    // private
    enum YCbCrSubSamplingType
    {
        YCCSST_ONE_TO_ONE,
        YCCSST_ONE_TO_TWO,
        YCCSST_ONE_TO_FOUR
    };

    // private
    enum YCbCrPositioningType
    {
        YCCPT_CENTERED,
        YCCPT_COSITED
    };

    // private
    class SegmentRecord
    {
    public:
        Long Offset, ByteCount, UncompressedByteCount;
        Long Width, Length;                 // segment dimensions
        Recti SubimageRectInner;            // associated with strip/tile
        Recti SubimageRectOuter;            // associated with full image
        bool SubimageRectInnerIsMeaning;    // always 'false' for strips but not for tiles
    };

public:
    // this class is proposed to hold IFD characteristics (together with an image when saving)
    class Q_ITEM ImageFileDirectory : public Heap
    {
    public:

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        // Construction of an IFD.  For saving.  Parameter rqBitsPerSample must contain that number of
        // bits-per-sample values which corresponds with the number of samples per pixel.  Parameter
        // bTreatImageAsBilevel is for ability of saving of TIFF bilevel images; it can be 'true' only if the
        // provided image is paletted and its palette has two entries: black and white (in this order).
        ImageFileDirectory (const RasterImagePtr& rspqImage, const StillArray<Short>& rqBitsPerSample,
            bool bTreatImageAsBilevel = false);

        // Setting of IFD fields and, in that way, setting of parameters for saving of the IFD.  Those
        // functions which relate to the same IFD field are joined together.  NOTE. TIFF format implies that
        // such multi-value information like resolution, position, halftoning matrix dimensions, grayscale
        // optical density, and min/max pairs are in separate IFD fields which should be set collectively
        // (allied functions are placed near each other) except when the user is confident that only a part of
        // information is required to be present.

        void SetCompression (CompressionType eCompression);

        void SetT42d (bool bT42d);  // only if compression is CT_GROUP_3; 'true' turns on two-dimensional encoding

        void SetJpegQuality (float fQuality);  // only if compression is CT_JPEG_TNT; the range is [0.0, 1.0]

        void SetOrientation (OrientationType eOrientation);

        void SetIsReducedResolutionVersion (bool bItIs);    // e.g. thumbnail
        void SetIsSinglePageOfMultipageImage (bool bItIs);  // if it's so, the calling is encouraged

        void SetXResolution (float fXResolution);

        void SetYResolution (float fYResolution);

        void SetResolutionUnit (ResolutionUnitType eResolutionUnit);

        void UnsetResolutionInfo ();  // turns off the three resolution-related fields

        void SetXPosition (float fXPosition);

        void SetYPosition (float fYPosition);

        void SetTransferFunction (const StillArray<Short>& rqTransferFunction);

        void SetTransferRange (
            Short TransferBlack0, Short TransferWhite0,
            Short TransferBlack1, Short TransferWhite1,
            Short TransferBlack2, Short TransferWhite2);

        void SetWhitePoint (float fWhitePointX, float fWhitePointY);

        void SetPrimaryChromaticities (
            float fChromaticityRedX, float fChromaticityRedY,
            float fChromaticityGreenX, float fChromaticityGreenY,
            float fChromaticityBlueX, float fChromaticityBlueY);

        void SetReferenceBlackWhite (
            float fReferenceBlack0, float fReferenceWhite0,
            float fReferenceBlack1, float fReferenceWhite1,
            float fReferenceBlack2, float fReferenceWhite2);

        void SetDateTime (const Date& rqDateTime);

        void SetImageDescription (const StringArray& rqImageDescription);

        void SetMake (const StringArray& rqMake);

        void SetModel (const StringArray& rqModel);

        void SetSoftware (const StringArray& rqSoftware);

        void SetArtist (const StringArray& rqArtist);

        void SetCopyright (const StringArray& rqCopyright);

        void SetHostComputer (const StringArray& rqHostComputer);

        void SetDocumentName (const StringArray& rqDocumentName);

        void SetPageName (const StringArray& rqPageName);

        void SetPageNumber (Short PageNumber, Short TotalPageNumber);

        void SetTargetPrinter (const StringArray& rqTargetPrinter);

        void SetDotRange (const StillArray<Link<Short,Short> >& rqDotRange);

        void SetThreshholding (ThreshholdingType eThreshholding);

        void SetCellWidth (Short CellWidth);

        void SetCellLength (Short CellLength);

        void SetGrayResponseUnit (GrayResponseUnitType eGrayResponseUnit);

        void SetGrayResponseCurve (const StillArray<Short>& rqGrayResponseCurve);

        void SetHalftoneHints (Short HighlightGrayLevel, Short ShadowGrayLevel);

        void SetMinSampleValue (const StillArray<Short>& rqMinSampleValue);

        void SetMaxSampleValue (const StillArray<Short>& rqMaxSampleValue);

        void SetSMinSampleValueUI (const StillArray<Long>& rqSMinSampleValueUI);   // can be called if bps != 32
        void SetSMinSampleValueFP (const StillArray<Float>& rqSMinSampleValueFP);  // can be called if bps is 32

        void SetSMaxSampleValueUI (const StillArray<Long>& rqSMaxSampleValueUI);   // can be called if bps != 32
        void SetSMaxSampleValueFP (const StillArray<Float>& rqSMaxSampleValueFP);  // can be called if bps is 32

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        // (For loading, FormatTiff performs construction of an IFD by itself.)

        // Some of IFD fields are not exposed since they affect images implicitly.  Some fields haven't got
        // function Has...Field because:
        //   a) they are critical and required to be present;
        //   b) their values are forced by the specification to have the default and therefore are defined in
        //      any case.
        // Those functions which relate to the same IFD field are joined together.

        Long GetImageWidth () const;

        Long GetImageLength () const;  // that's the height

        int GetBitsPerPixel () const;
        const StillArray<Short>& GetBitsPerSample () const;

        ColorModelType GetColorModel () const;

        CompressionType GetCompression () const;

        OrientationType GetOrientation () const;

        bool IsSubfileReducedResolutionVersion () const;
        bool IsSubfileSinglePageOfMultipageImage () const;
        bool IsSubfileTransparencyMask () const;

        bool HasSubfileTypeField () const;  // obsolete
        SubfileType GetSubfileType () const;

        bool HasXResolutionField () const;
        float GetXResolution () const;

        bool HasYResolutionField () const;
        float GetYResolution () const;

        ResolutionUnitType GetResolutionUnit () const;

        bool HasXPositionField () const;
        float GetXPosition () const;

        bool HasYPositionField () const;
        float GetYPosition () const;

        const StillArray<Short>& GetTransferFunction () const;

        Short GetTransferBlack0 () const;
        Short GetTransferWhite0 () const;
        Short GetTransferBlack1 () const;
        Short GetTransferWhite1 () const;
        Short GetTransferBlack2 () const;
        Short GetTransferWhite2 () const;

        bool HasWhitePointField () const;
        float GetWhitePointX () const;
        float GetWhitePointY () const;

        bool HasPrimaryChromaticitiesField () const;
        float GetChromaticityRedX () const;
        float GetChromaticityRedY () const;
        float GetChromaticityGreenX () const;
        float GetChromaticityGreenY () const;
        float GetChromaticityBlueX () const;
        float GetChromaticityBlueY () const;

        float GetReferenceBlack0 () const;
        float GetReferenceWhite0 () const;
        float GetReferenceBlack1 () const;
        float GetReferenceWhite1 () const;
        float GetReferenceBlack2 () const;
        float GetReferenceWhite2 () const;

        bool HasDateTimeField () const;
        const StringArray& GetDateTime () const;

        bool HasImageDescriptionField () const;
        const StringArray& GetImageDescription () const;

        bool HasMakeField () const;
        const StringArray& GetMake () const;

        bool HasModelField () const;
        const StringArray& GetModel () const;

        bool HasSoftwareField () const;
        const StringArray& GetSoftware () const;

        bool HasArtistField () const;
        const StringArray& GetArtist () const;

        bool HasCopyrightField () const;
        const StringArray& GetCopyright () const;

        bool HasHostComputerField () const;
        const StringArray& GetHostComputer () const;

        bool HasDocumentNameField () const;
        const StringArray& GetDocumentName () const;

        bool HasPageNameField () const;
        const StringArray& GetPageName () const;

        bool HasPageNumberField () const;
        Short GetPageNumber () const;
        Short GetTotalPageNumber () const;

        bool HasTargetPrinterField () const;
        const StringArray& GetTargetPrinter () const;

        const StillArray<Link<Short,Short> >& GetDotRange () const;

        InkSetType GetInkSet () const;

        Short GetNumberOfInks () const;

        bool HasInkNamesField () const;
        const StringArray& GetInkNames () const;

        ThreshholdingType GetThreshholding () const;

        bool HasCellWidthField () const;
        Short GetCellWidth () const;

        bool HasCellLengthField () const;
        Short GetCellLength () const;

        GrayResponseUnitType GetGrayResponseUnit () const;

        bool HasGrayResponseCurveField () const;
        const StillArray<Short>& GetGrayResponseCurve () const;

        bool HasHalftoneHintsField () const;
        Short GetHighlightGrayLevel () const;
        Short GetShadowGrayLevel () const;

        bool HasMinSampleValueField () const;
        const StillArray<Short>& GetMinSampleValue () const;

        bool HasMaxSampleValueField () const;
        const StillArray<Short>& GetMaxSampleValue () const;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    private:
        friend class FormatTiff;

        // construction for loading
        ImageFileDirectory ();

        // used after fields are loaded
        void AssignBitsPerSampleAndSampleFormatDefaults ();
        bool AssignDerivedDefaultsAndAttributes ();

        // to avoid code redundancy
        bool AreBitsPerSampleInProperRanges () const;
        bool IsMinSampleAllowed () const;
        bool IsMaxSampleAllowed () const;
        bool IsTransferFunctionAllowed () const;
        bool IsDotRangeAllowed () const;
        bool IsTransferRangeAllowed () const;
        bool IsReferenceBlackWhiteAllowed () const;

        RasterImagePtr m_spqImageSaveTo;
        float m_fJpegQuality;               // (default: 0.75)
        bool m_bImageIsBilevel;
        bool m_bDoNotWriteResolutionInfo;   // (default: false)

        // The defaults are for both saving and loading; the user should rely only on the defaults remarked at
        // the beginning of this page.

        bool m_bNewSubfileTypeFieldIsPresent;            // default: false (has default content)
        bool m_bSubfileIsReducedResolutionVersion;       // default: false
        bool m_bSubfileIsSinglePageOfMultipageImage;     // default: false
        bool m_bSubfileIsTransparencyMask;               // default: false

        bool m_bSubfileTypeFieldIsPresent;               // default: false
        SubfileType m_eSubfileType;

        bool m_bImageWidthFieldIsPresent;                // default: false
        Long m_ImageWidth;

        bool m_bImageLengthFieldIsPresent;               // default: false
        Long m_ImageLength;

        bool m_bBitsPerSampleFieldIsPresent;             // default: false (has default content)
        StillArray<Short> m_qBitsPerSample;              // default: size is samples-per-pixel, all are 1

        bool m_bCompressionFieldIsPresent;               // default: false (has default content)
        CompressionType m_eCompression;                  // default: CT_NONE

        bool m_bPhotometricInterpretationFieldIsPresent; // default: false
        PhotometricInterpretationType m_ePhotometricInterpretation;

        bool m_bThreshholdingFieldIsPresent;             // default: false (has default content)
        ThreshholdingType m_eThreshholding;              // default: TT_NO_DITHERING_OR_HALFTONING_APPLIED

        bool m_bCellWidthFieldIsPresent;                 // default: false
        Short m_CellWidth;

        bool m_bCellLengthFieldIsPresent;                // default: false
        Short m_CellLength;

        bool m_bFillOrderFieldIsPresent;                 // default: false (has default content)
        FillOrderType m_eFillOrder;                      // default: FOT_HIGH_ORDER_BIT_FIRST

        bool m_bDocumentNameFieldIsPresent;              // default: false
        StringArray m_qDocumentName;

        bool m_bImageDescriptionFieldIsPresent;          // default: false
        StringArray m_qImageDescription;

        bool m_bMakeFieldIsPresent;                      // default: false
        StringArray m_qMake;

        bool m_bModelFieldIsPresent;                     // default: false
        StringArray m_qModel;

        bool m_bStripOffsetsFieldIsPresent;              // default: false
        StillArray<Long> m_qStripOffsets;

        bool m_bOrientationFieldIsPresent;               // default: false (has default content)
        OrientationType m_eOrientation;                  // default: OT_TOP_LEFT

        bool m_bSamplesPerPixelFieldIsPresent;           // default: false (has default content)
        Short m_SamplesPerPixel;                         // default: 1

        bool m_bRowsPerStripFieldIsPresent;              // default: false (has default content)
        Long m_RowsPerStrip;                             // default: 4294967295

        bool m_bStripByteCountsFieldIsPresent;           // default: false
        StillArray<Long> m_qStripByteCounts;

        bool m_bMinSampleValueFieldIsPresent;            // default: false (has default content)
        StillArray<Short> m_qMinSampleValue;             // default: size is samples-per-pixel, all are 0

        bool m_bMaxSampleValueFieldIsPresent;            // default: false (has default content)
        StillArray<Short> m_qMaxSampleValue;             // default: size is samples-per-pixel, all are 2^bps - 1

        bool m_bXResolutionFieldIsPresent;               // default: false
        float m_fXResolution;

        bool m_bYResolutionFieldIsPresent;               // default: false
        float m_fYResolution;

        bool m_bPlanarConfigurationFieldIsPresent;       // default: false (has default content)
        PlanarConfigurationType m_ePlanarConfiguration;  // default: PCT_CHUNKY_FORMAT

        bool m_bPageNameFieldIsPresent;                  // default: false
        StringArray m_qPageName;

        bool m_bXPositionFieldIsPresent;                 // default: false
        float m_fXPosition;

        bool m_bYPositionFieldIsPresent;                 // default: false
        float m_fYPosition;

        bool m_bFreeOffsetsFieldIsPresent;               // default: false
        StillArray<Long> m_qFreeOffsets;

        bool m_bFreeByteCountsFieldIsPresent;            // default: false
        StillArray<Long> m_qFreeByteCounts;

        bool m_bGrayResponseUnitFieldIsPresent;          // default: false (has default content)
        GrayResponseUnitType m_eGrayResponseUnit;        // default: GRUT_HUNDREDTHS_OF_UNIT

        bool m_bGrayResponseCurveFieldIsPresent;         // default: false
        StillArray<Short> m_qGrayResponseCurve;

        bool m_bT4OptionsFieldIsPresent;                 // default: false (has default content)
        bool m_bT42d;                                    // default: false
        bool m_bT4UncompressedModeUsed;                  // default: false
        bool m_bT4FillBitsBeforeEolUsed;                 // default: false

        bool m_bT6OptionsFieldIsPresent;                 // default: false (has default content)
        bool m_bT6UncompressedModeAllowed;               // default: false

        bool m_bResolutionUnitFieldIsPresent;            // default: false (has default content)
        ResolutionUnitType m_eResolutionUnit;            // default: RUT_INCH

        bool m_bPageNumberFieldIsPresent;                // default: false
        Short m_PageNumber;
        Short m_TotalPageNumber;

        bool m_bTransferFunctionFieldIsPresent;          // default: false (has default content)
        StillArray<Short> m_qTransferFunction;           // default: filled for each component as for NTSC

        bool m_bSoftwareFieldIsPresent;                  // default: false
        StringArray m_qSoftware;

        bool m_bDateTimeFieldIsPresent;                  // default: false
        StringArray m_qDateTime;

        bool m_bArtistFieldIsPresent;                    // default: false
        StringArray m_qArtist;

        bool m_bHostComputerFieldIsPresent;              // default: false
        StringArray m_qHostComputer;

        bool m_bPredictorFieldIsPresent;                 // default: false (has default content)
        PredictorType m_ePredictor;                      // default: PT_NO_PREDICTION

        bool m_bWhitePointFieldIsPresent;                // default: false
        float m_fWhitePointX, m_fWhitePointY;

        bool m_bPrimaryChromaticitiesFieldIsPresent;     // default: false
        float m_fChromaticityRedX, m_fChromaticityRedY;
        float m_fChromaticityGreenX, m_fChromaticityGreenY;
        float m_fChromaticityBlueX, m_fChromaticityBlueY;

        bool m_bColorMapFieldIsPresent;                  // default: false
        StillArray<Short> m_qColorMap;

        bool m_bHalftoneHintsFieldIsPresent;             // default: false
        Short m_HighlightGrayLevel, m_ShadowGrayLevel;

        bool m_bTileWidthFieldIsPresent;                 // default: false
        Long m_TileWidth;

        bool m_bTileLengthFieldIsPresent;                // default: false
        Long m_TileLength;

        bool m_bTileOffsetsFieldIsPresent;               // default: false
        StillArray<Long> m_qTileOffsets;

        bool m_bTileByteCountsFieldIsPresent;            // default: false
        StillArray<Long> m_qTileByteCounts;

        bool m_bInkSetFieldIsPresent;                    // default: false (has default content)
        InkSetType m_eInkSet;                            // default: IST_CMYK

        bool m_bInkNamesFieldIsPresent;                  // default: false
        StringArray m_qInkNames;

        bool m_bNumberOfInksFieldIsPresent;              // default: false (has default content)
        Short m_NumberOfInks;                            // default: 4

        bool m_bDotRangeFieldIsPresent;                  // default: false (has default content)
        StillArray<Link<Short,Short> > m_qDotRange;      // default: (multiple) (0, 2^bps - 1)

        bool m_bTargetPrinterFieldIsPresent;             // default: false
        StringArray m_qTargetPrinter;

        bool m_bExtraSamplesFieldIsPresent;              // default: false
        StillArray<ExtraSampleType> m_qExtraSamples;

        bool m_bSampleFormatFieldIsPresent;              // default: false (has default content)
        StillArray<SampleFormatType> m_qSampleFormat;    // default: SFT_UNSIGNED_INTEGER for each sample

        bool m_bSMinSampleValueFieldIsPresent;           // default: false (has default content)
        StillArray<Long> m_qSMinSampleValueUI;
        StillArray<SLong> m_qSMinSampleValueSI;
        StillArray<Float> m_qSMinSampleValueFP;

        bool m_bSMaxSampleValueFieldIsPresent;           // default: false (has default content)
        StillArray<Long> m_qSMaxSampleValueUI;
        StillArray<SLong> m_qSMaxSampleValueSI;
        StillArray<Float> m_qSMaxSampleValueFP;

        bool m_bTransferRangeFieldIsPresent;             // default: false (has default content)
        Short m_TransferBlack0, m_TransferWhite0;
        Short m_TransferBlack1, m_TransferWhite1;
        Short m_TransferBlack2, m_TransferWhite2;

        bool m_bJPEGTablesFieldIsPresent;                // default: false
        StillArray<Undefined> m_qJPEGTables;

        bool m_bYCbCrCoefficientsFieldIsPresent;         // default: false (has default content)
        float m_fYCbCrLumaRed;                           // default: 0.299
        float m_fYCbCrLumaGreen;                         // default: 0.587
        float m_fYCbCrLumaBlue;                          // default: 0.114

        bool m_bYCbCrSubSamplingFieldIsPresent;          // default: false (has default content)
        YCbCrSubSamplingType m_eYCbCrSubSamplingHoriz;   // default: YCCSST_ONE_TO_TWO
        YCbCrSubSamplingType m_eYCbCrSubSamplingVert;    // default: YCCSST_ONE_TO_TWO

        bool m_bYCbCrPositioningFieldIsPresent;          // default: false (has default content)
        YCbCrPositioningType m_eYCbCrPositioning;        // default: YCCPT_CENTERED

        bool m_bReferenceBlackWhiteFieldIsPresent;       // default: false (has default content)
        float m_fReferenceBlack0, m_fReferenceWhite0;
        float m_fReferenceBlack1, m_fReferenceWhite1;
        float m_fReferenceBlack2, m_fReferenceWhite2;

        bool m_bCopyrightFieldIsPresent;                 // default: false
        StringArray m_qCopyright;

        // derived attributes
        bool m_bBitsPerSamplesAreTheSame;
        int m_iBitsPerEachSample;
        bool m_bPixelSamplesAllAreUI;
        StillArray<int> m_qSampleUIValueRanges, m_qSampleUITopValues;
        int m_iUIValueRangePerEachSample, m_iUITopValuePerEachSample;
        StillArray<Long> m_qSampleSTopValuesUI;
        StillArray<SLong> m_qSampleSTopValuesSI;
        StillArray<Float> m_qSampleSTopValuesFP;
        bool m_bImageHasColorimetricInterpretation;
        bool m_bIsReducedResolutionVersion, m_bIsSinglePageOfMultipageImage, m_bIsTransparencyMask;
        bool m_bImageIsYCbCrNonJpeg;
        bool m_bStripsAreSpecified, m_bTilesAreSpecified;
        int m_iRowsPerStrip, m_iStripsPerImage;
        int m_iTilesAcross, m_iTilesDown, m_iTilesPerImage;
        int m_iSegmentsPerPlane;
        int m_iBitsPerPixel;
        StillArray<SegmentRecord> m_qSegments;
        ColorModelType m_eColorModel;
        bool m_bAlmostPalettedWithAlpha;
        bool m_bTypicalCaseForPixelSamples;
    };
    typedef SmartPointer<ImageFileDirectory> ImageFileDirectoryPtr;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.
    FormatTiff (SegmentOrganizationType eSegmentOrganization = SOT_STRIP_ORIENTED);

    // endianness
    void SetByteOrder (ByteOrderType eByteOrder);

    // Exif metadata
    void SetExifMetadata (const FormatExifPtr& rspqExifMetadata);

    // adding of an IFD; returns the position of insertion
    int InsertImageFileDirectory (const ImageFileDirectoryPtr& rspqIFD);

    // Saving.  Progressed.
    bool SaveFile (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data related to each stored IFD, plus
    // Exif.
    FormatTiff (const char* acFilename, bool& rbSuccess);

    // file attributes
    ByteOrderType GetByteOrder () const;
    int GetImageFileDirectoryQuantity () const;

    // Exif metadata.  Native TIFF fields naturally become part of Exif.
    bool HasExifMetadata () const;
    const FormatExif& GetExifMetadata () const;

    // getting of IFD characteristics without touching pixel data
    const ImageFileDirectory& GetImageFileDirectory (int i = 0) const;

    // Loading of an IFD image.  Progressed.
    bool LoadIFDImage (RasterImagePtr& rspqImage, int i = 0);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // helping functions for IFD text-fields
    static StringArray WrapSoleString (const String& rqString);         // for saving
    static String UnwrapSoleString (const StringArray& rqStringArray);  // for loading

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    enum FieldType
    {
        FT_BYTE,
        FT_ASCII,
        FT_SHORT,
        FT_LONG,
        FT_RATIONAL,
        FT_SBYTE,
        FT_UNDEFINED,
        FT_SSHORT,
        FT_SLONG,
        FT_SRATIONAL,
        FT_FLOAT,
        FT_DOUBLE
    };

    void PrepareIFDForSaving (const ImageFileDirectoryPtr& rspqIFD, bool bPreparingTransparencyMask);
    static void SaveIFDFields (const ImageFileDirectory& rqIFD, int& riSeparatedValuesBytePos,
        Stream& rqSaveStream, StillArray<int>& rqOffsetsBytePosArray, StillArray<int>& rqByteCountsBytePosArray);
    static int GetCountForString (const StringArray& rqStringArray);
    static void WriteString (const StringArray& rqStringArray, Stream& rqSaveStream);
    static void WriteRational (float fValue, Stream& rqSaveStream);
    static void FilterSegmentImage (const SegmentRecord& rqSegment, StillArray<char>& rqDecompressedData,
        const ImageFileDirectory& rqIFD);
    bool LoadIFDFields (int iFieldCount, ImageFileDirectory& rqIFD, Stream& rqFieldStream);
    static bool ReadString (StringArray& rqStringArray, int iCount, Stream& rqValueStream);
    static float ReadRational (Stream& rqValueStream);
    bool ExamineIFDFields (ImageFileDirectory& rqIFD);
    bool LoadIFD (RasterImagePtr& rspqImage, const ImageFileDirectory& rqIFD, bool bLoadingTransparencyMask);
    double ReadSample (int iSamplePos, const StillArray<char>& rqDecompressedData, const ImageFileDirectory& rqIFD,
        int& riBytePos, int& riBitRatio) const;
    void UnfilterSegmentImage (const SegmentRecord& rqSegment, StillArray<char>& rqDecompressedData,
        const ImageFileDirectory& rqIFD) const;

    ByteOrderType m_eByteOrder;                         // (default: BOT_LITTLE_ENDIAN)
    SegmentOrganizationType m_eSegmentOrganization;     // (default: SOT_STRIP_ORIENTED)
    bool m_bLittleEndian;
    ModestArray<ImageFileDirectoryPtr> m_qImageFileDirectories;
    ImageFileDirectoryPtr m_spqTransparencyMaskIFD;
    bool m_bHasExifMetadata;  // default: false
    FormatExifPtr m_spqExifMetadata;

    // for saving
    static const int ms_iKBytesPerSegment;  // 32

    static const float ms_fDefaultXResolutionInchForSave;  // 72.0
    static const float ms_fDefaultYResolutionInchForSave;  // 72.0

    static const int ms_iRationalNumbersScaler;  // 10000

    // if 'false', the default ZLib strategy is used
    static const bool ms_bChooseBestFromManifoldZLibStrategies;  // true

    // for loading; specifies if to perform trimming of read text strings
    static const bool ms_bTrimTexts;  // true
};

#include "QFormatTiff.inl"

}

#endif



