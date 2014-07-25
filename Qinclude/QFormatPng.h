#ifndef QFORMATPNG_H
#define QFORMATPNG_H

// Portable Network Graphics Format.  File extension: .png.  This is just about complete implementation of PNG
// format on the level of "W3C Recommendation 10 November 2003"; the only one thing which was advisedly omitted
// is iTXt ancillary chunk which implies UTF-8 text encoding.  RasterImage's visibility mask acts very
// naturally for PNG via usage of tRNS chunk.  PNG format states the next two kinds of textual data:
// length-limited 1-79 characters Latin1 string (keyword) and length-unlimited string (actual text).  A Latin1
// string should not contain leading, trailing, or consecutive spaces, and its characters are bound within the
// ranges 32-126 and 161-255; a length-unlimited string can be built from characters in the ranges 32-126 and
// 161-255 as well plus the following characters: tab (charcode 9), lf (10), ff (12), and cr (13).  Not all PNG
// chunks are exposed for access since those which haven't a member function associated with take effect in an
// implicit way.
//
// For saving, acceptable RasterImage's color models are: CMT_PALETTED, CMT_PALETTED_WITH_ALPHA, CMT_GRAYSCALE,
// CMT_GRAYSCALE_WITH_ALPHA, CMT_RGB, and CMT_RGBA.  The possible bits-per-pixel values in conjunction with the
// color models are noted below:
//   RasterImage::CMT_PALETTED             : the bits-per-pixel value can be 1, 2, 4, or 8;
//   RasterImage::CMT_PALETTED_WITH_ALPHA  : the bits-per-pixel value can be 1, 2, 4, or 8;
//   RasterImage::CMT_GRAYSCALE            : the bits-per-pixel value can be 1, 2, 4, 8, or 16;
//   RasterImage::CMT_GRAYSCALE_WITH_ALPHA : the bits-per-pixel value can be 16 or 32;
//   RasterImage::CMT_RGB                  : the bits-per-pixel value can be 24 or 48;
//   RasterImage::CMT_RGBA                 : the bits-per-pixel value can be 32 or 64.
// For paletted and paletted-with-alpha images, the bpp value must allow image's pixels to address any entry in
// the palette, making true the next: 2^bpp >= palette_size.  If an image for saving has a visibility mask,
// then the two ways of behavior can follow depending if it is paletted or not: in case of a paletted image,
// PNG's color table grows by one entry to represent fully transparent pixels (PNG's bpp auto-raises
// synchronously if needed) and the supporting tRNS chunk is written; in case of a grayscale, RGB, or RGBA
// image, the saving process tries to find an unused color for being able to signify invisible areas with it.
// WARNING. If a paletted image has palette size 256 and also a visibility mask associated, then it cannot be
// saved since one extra palette entry is required; and if a non-paletted image utilizes all possible color
// values within the given pixel precision (very unlikely in fact), then its saving together with a visibility
// mask is impossible too (exception ET_VISIBILITY_MASK_SIMULTANEOUSLY_WITH_ALL_POSSIBLE_COLOR_VALUES_USED will
// be thrown).  With the purpose of easing file reading by variety of applications, no sBIT chunk appears in a
// PNG data.  The situation must be avoided when iCCP and sRGB chunks are set simultaneously since they have
// very close dedications.  All non-pixel-data-related chunks precede the first IDAT chunk.  If saving uncovers
// some text which doesn't correspond with the rules stated above, no corrections but only assertions take
// place.
// DEFAULTS. Filter selection method is FSM_MINIMUM_SUM_OF_ABSOLUTE_DIFFERENCES; interlace method is IMT_NONE;
// data chunk splitting is DCST_KILOBYTES_8; compression is CT_DEFLATE_INFLATE; filter method is FMT_ADAPTIVE;
// all ancillary chunks are disabled except pHYs which holds 2835 (72 dpi) for both pixels-per-unit values,
// where unit is PPDUT_METER.
//
// For loading, the next image types can appear: IT_PALETTED, IT_PALETTED_WITH_ALPHA, IT_GRAYSCALE_8,
// IT_GRAYSCALE_16, IT_GRAYSCALE_WITH_ALPHA_8_8, IT_GRAYSCALE_WITH_ALPHA_16_16, IT_RGB_8_8_8, IT_RGB_16_16_16,
// IT_RGBA_8_8_8_8, and IT_RGBA_16_16_16_16.  An image can arrive with or without a visibility mask.  In the
// loading process, there are no concessions (like for GIF) and any critical error causes an assertion with
// subsequent exiting from the routine.  NOTE. Some ancillary PNG chunks can occur in a file later than actual
// pixel data, so in such a case they are available only after image loading.  An sBIT chunk, if is present,
// alters the image in the way of more precise and close-to-intended look.  For loading of textual data, there
// are no assertions but auto-corrections employed in order to bring format-conforming texts.  In loading from
// a dynamic bytestream, the implementation of ZLib decompression remembers its state each time when a portion
// of bytes is processed, so PNG loading returns exactly to the same pixel after a renewal; memory occupation
// is minimal for this development.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"
#include "QDate.h"

namespace Q
{

class Q_ITEM FormatPng : public FormatBase, public ProgressBase
{
public:
    // relates only to saving; the enumerands represent approaches to finding of that PNG filter for each row
    // which results into lesser overall file size
    enum FilterSelectionMethodType
    {
        FSM_MINIMUM_SUM_OF_ABSOLUTE_DIFFERENCES,  // well established technique, faster
        FSM_LOCAL_MULTICHOICE                     // more zealous method to bring smaller files, slower
    };

    enum InterlaceMethodType
    {
        IMT_NONE,   // regular
        IMT_ADAM_7  // interlaced (i.e. with the ability to gradually improve in look while being transmitted)
    };

    // relates only to saving; splitting of pixel data for network-oriented PNG files may make their receiving
    // more appropriate
    enum DataChunkSplittingType
    {
        DCST_SINGLE,       // pixel data lay continuously
        DCST_KILOBYTES_8,  // pixel data is separated with 8 kilobytes per chunk
        DCST_KILOBYTES_32  // pixel data is separated with 32 kilobytes per chunk
    };

    // support for ancillary sRGB chunk
    enum StandardRgbColorSpaceRenderingIntentType
    {
        SRGBCSRIT_PERCEPTUAL,
        SRGBCSRIT_RELATIVE_COLORIMETRIC,
        SRGBCSRIT_SATURATION,
        SRGBCSRIT_ABSOLUTE_COLORIMETRIC
    };

    // support for ancillary pHYs chunk
    enum PhysicalPixelDimensionsUnitType
    {
        PPDUT_UNKNOWN,  // PNG's term
        PPDUT_METER
    };

    // support for ancillary sPLT chunk
    enum SuggestedPaletteSampleDepthType
    {
        SPSDT_8_BITS,
        SPSDT_16_BITS
    };

    // relates only to loading
    enum ColorModelType
    {
        CMT_INDEXED,  // represents paletted and paletted-with-alpha color models
        CMT_GRAYSCALE,
        CMT_GRAYSCALE_WITH_ALPHA,
        CMT_RGB,
        CMT_RGBA
    };

    // relates only to loading; PNG format defines only one item
    enum CompressionType
    {
        CT_DEFLATE_INFLATE
    };

    // relates only to loading; PNG format defines only one item
    enum FilterMethodType
    {
        FMT_ADAPTIVE
    };

    // ET_VISIBILITY_MASK_SIMULTANEOUSLY_WITH_ALL_POSSIBLE_COLOR_VALUES_USED: pixels in the image use all
    //   possible color values while visibility-mask-caused transparent areas must be represented by some unused
    //   color (a solution may be to increase the bits-per-pixel value for PNG data)
    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    enum ExceptionType
    {
        ET_VISIBILITY_MASK_SIMULTANEOUSLY_WITH_ALL_POSSIBLE_COLOR_VALUES_USED,  // saving
        ET_CANNOT_ACCESS_FILE,                                                  // saving and loading
        ET_FORMAT_IDENTIFIER_MISMATCH,                                      // loading
        ET_FORMAT_MISMATCH,                                                 // loading
        ET_UNKNOWN_CRITICAL_CHUNK_ENCOUNTERED,                                  // loading
        ET_ZLIB_EXCEPTION                                                       // loading
    };

    // WT_CHUNK_PLTE_IS_INCORRECT: that non-paletted image has ancillary PLTE chunk which is incorrect
    enum WarningType
    {
        WT_CHUNK_PLTE_IS_INCORRECT,                          // loading
        WT_ANCILLARY_CHUNK_cHRM_IS_INCORRECT,                // loading
        WT_ANCILLARY_CHUNK_gAMA_IS_INCORRECT,                // loading
        WT_ANCILLARY_CHUNK_iCCP_IS_INCORRECT,                // loading
        WT_ANCILLARY_CHUNK_sBIT_IS_INCORRECT,                // loading
        WT_ANCILLARY_CHUNK_sRGB_IS_INCORRECT,                // loading
        WT_ANCILLARY_CHUNK_bKGD_IS_INCORRECT,                // loading
        WT_ANCILLARY_CHUNK_hIST_IS_INCORRECT,                // loading
        WT_ANCILLARY_CHUNK_tRNS_IS_INCORRECT,                // loading
        WT_ANCILLARY_CHUNK_pHYs_IS_INCORRECT,                // loading
        WT_ONE_OR_MORE_ANCILLARY_CHUNKS_sPLT_ARE_INCORRECT,  // loading
        WT_ANCILLARY_CHUNK_tIME_IS_INCORRECT,                // loading
        WT_ONE_OR_MORE_ANCILLARY_CHUNKS_tEXt_ARE_INCORRECT,  // loading
        WT_ONE_OR_MORE_ANCILLARY_CHUNKS_zTXt_ARE_INCORRECT   // loading
    };

    // support for ancillary sPLT chunk
    class SuggestedPaletteColor
    {
    public:
        // construction
        SuggestedPaletteColor () {};  // uninitialized
        SuggestedPaletteColor (int iRed, int iGreen, int iBlue, int iAlpha, int iFrequency)
            :
            Red(iRed),
            Green(iGreen),
            Blue(iBlue),
            Alpha(iAlpha),
            Frequency(iFrequency)
        {
        };

        int Red, Green, Blue, Alpha, Frequency;
    };

    // support for ancillary sPLT chunk
    class SuggestedPalette
    {
    public:
        // construction
        SuggestedPalette () {};  // uninitialized
        SuggestedPalette (const String& rqName, SuggestedPaletteSampleDepthType eSampleDepth,
            const StillArray<SuggestedPaletteColor>& rqPalette)
            :
            Name(rqName),
            SampleDepth(eSampleDepth),
            Palette(rqPalette)
        {
        };

        String Name;  // length must be in the range [1, 79]
        SuggestedPaletteSampleDepthType SampleDepth;
        StillArray<SuggestedPaletteColor> Palette;  // will be sorted on Frequency field before saving
    };

    // support for ancillary tEXt and zTXt chunks
    class TextualData
    {
    public:
        // construction
        TextualData () {};  // uninitialized
        TextualData (const String& rqKeyword, const String& rqText)
            :
            Keyword(rqKeyword),
            Text(rqText)
        {
        };

        String Keyword;  // length must be in the range [1, 79]
        String Text;

    private:
        friend class FormatPng;
        bool m_bOnlyAs_tEXt;
    };

    // destruction
    ~FormatPng ();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.  Parameter iBitsPerPixel can be omitted only for paletted and
    // paletted-with-alpha images, and in this case the minimal possible bpp is automatically derived from the
    // palette size.
    FormatPng (const RasterImagePtr& rspqImage, int iBitsPerPixel = -1);

    // file settings
    void SetFilterSelectionMethod (FilterSelectionMethodType eFilterSelectionMethod);
    void SetInterlaceMethod (InterlaceMethodType eInterlaceMethod);
    void SetDataChunkSplitting (DataChunkSplittingType eDataChunkSplitting);

    // pHYs chunk (horizontal and vertical resolutions)
    void UnsetPhysicalPixelDimensions ();  // turns the chunk off
    void SetPhysicalPixelDimensions (int iPixelsPerUnitX, int iPixelsPerUnitY,
        PhysicalPixelDimensionsUnitType ePhysicalPixelDimensionsUnit);

    // cHRM chunk (white point and chromaticity)
    void SetChromaticities (
        float fWhitePointX, float fWhitePointY,
        float fRedX, float fRedY,
        float fGreenX, float fGreenY,
        float fBlueX, float fBlueY);

    // gAMA chunk (gamma)
    void SetGamma (float fGamma);

    // iCCP chunk (ICC profile); the name's length must be in the range [1, 79]
    void SetICCProfile (const String& rqICCProfileName, const StillArray<char>& rqICCProfile);

    // sRGB chunk (standard RGB color space)
    void SetStandardRgbColorSpaceRenderingIntent (
        StandardRgbColorSpaceRenderingIntentType eStandardRgbColorSpaceRenderingIntent);

    // bKGD chunk (background).  Grayscale, red, green, and blue background values are in the scale of PNG's
    // bits-per-sample value.
    void SetBackgroundGrayscale (int iGrayscale);
    void SetBackgroundRgb (int iRed, int iGreen, int iBlue);
    void SetBackgroundPaletteIndex (int iPaletteIndex);

    // hIST chunk (image histogram)
    void SetHistogramFrequencies (const StillArray<int>& rqHistogramFrequencies);

    // sPLT chunk (suggested palette); can be multiple
    int AddSuggestedPalette (const SuggestedPalette& rqSuggestedPalette);

    // tIME chunk (last-modification time)
    void SetLastModificationTime (const Date& rqLastModificationTime);

    // tEXt/zTXt chunk (textual data); can be multiple.  Whether a text is to be stored in the simpler way
    // using tEXt or as compressed using zTXt chunk is determined from parameter bOnlyAs_tEXt: 'false' means
    // decide automatically from the text's length, 'true' forces to use tEXt chunk length regardless.
    int AddTextualData (TextualData& rqTextualData, bool bOnlyAs_tEXt = false);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);
    bool SaveImage (StillArray<char>& rqStaticBytestream);  // NOTE. It is able to throw exceptions.

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to reach the first IDAT chunk collecting all heading
    // information on the way.
    FormatPng (const char* acFilename, bool& rbSuccess);
    FormatPng (bool bBytestreamIsStatic, const char* acBytestream, int iBytestreamSize, bool& rbSuccess);

    // file attributes
    int GetWidth () const;
    int GetHeight () const;
    int GetBitsPerPixel () const;
    ColorModelType GetColorModel () const;
    InterlaceMethodType GetInterlaceMethod () const;
    CompressionType GetCompression () const;
    FilterMethodType GetFilterMethod () const;
    int GetBitsPerSample () const;

    // pHYs chunk (horizontal and vertical resolutions)
    bool Has_pHYs_Chunk () const;
    int GetPixelsPerUnitX () const;
    int GetPixelsPerUnitY () const;
    PhysicalPixelDimensionsUnitType GetPhysicalPixelDimensionsUnit () const;

    // cHRM chunk (white point and chromaticity)
    bool Has_cHRM_Chunk () const;
    float GetChromaticityWhitePointX () const;
    float GetChromaticityWhitePointY () const;
    float GetChromaticityRedX () const;
    float GetChromaticityRedY () const;
    float GetChromaticityGreenX () const;
    float GetChromaticityGreenY () const;
    float GetChromaticityBlueX () const;
    float GetChromaticityBlueY () const;

    // gAMA chunk (gamma)
    bool Has_gAMA_Chunk () const;
    float GetGamma () const;

    // iCCP chunk (ICC profile)
    bool Has_iCCP_Chunk () const;
    const String& GetICCProfileName () const;
    const StillArray<char>& GetICCProfile () const;

    // sRGB chunk (standard RGB color space)
    bool Has_sRGB_Chunk () const;
    StandardRgbColorSpaceRenderingIntentType GetStandardRgbColorSpaceRenderingIntent () const;

    // bKGD chunk (background).  Grayscale, red, green, and blue background values are in the scale of PNG's
    // bits-per-sample value.
    bool Has_bKGD_Chunk () const;
    int GetBackgroundGrayscale () const;
    int GetBackgroundRed () const;
    int GetBackgroundGreen () const;
    int GetBackgroundBlue () const;
    int GetBackgroundPaletteIndex () const;

    // hIST chunk (image histogram)
    bool Has_hIST_Chunk () const;
    const StillArray<int>& GetHistogramFrequencies () const;

    // sPLT chunk (suggested palette); can be multiple
    int GetSuggestedPaletteQuantity () const;
    const SuggestedPalette& GetSuggestedPalette (int i) const;

    // tIME chunk (last-modification time).  NOTE. Due to PNG format's design, this chunk can become available
    // (if is present) not only after construction but also after image loading; after construction it is
    // unknown whether the file is free of this chunk, but it will be known with calling of Has_tIME_Chunk
    // function after the image is loaded.
    bool Has_tIME_Chunk () const;
    const Date& GetLastModificationTime () const;

    // tEXt/zTXt chunk (textual data); can be multiple.  NOTE. Due to PNG format's design, chunks of that kind
    // can become available (if is present) not only after construction but also after image loading (i.e.
    // image loading may increase number of accessible texts); after construction it is unknown whether the
    // file contains no more instances of this chunk, but it will be known with calling of
    // GetTextualDataQuantity function after the image is loaded.
    int GetTextualDataQuantity () const;
    const TextualData& GetTextualData (int i) const;

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage, int iDynamicBytestreamSize = -1);

    // Support for loading from a dynamic bytestream.  For non-interlaced images, only GetDevelopedRowQuantity is
    // usable allowing an application to hide not yet loaded rows.  When an image being loaded is interlaced,
    // return value from GetCurrentPassIndex can vary from 0 to 6 inclusively, but it may jump e.g. from 0 to
    // 3 because some passes may be empty for small images.  Function GetDevelopedRowQuantity acts within each
    // pass similarly to how it would behave in a non-interlaced case: varies from 0 to image_height
    // inclusively.  Generally speaking, the application may not take into account a return value from
    // GetDevelopedRowQuantity for passes other than the first (for which hiding is needed), just updating the
    // image wholly while it isn't loaded completely.  A visibility mask, if is present, gets alike coarse look
    // as the image, reaching eventually the true state.  On finish, loaded_pass_quantity is 6 and
    // loaded_row_quantity is equal to image_height.
    int GetDevelopedRowQuantity () const;
    int GetCurrentPassIndex () const;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    enum InterruptionPointType
    {
        IPT_LENGTH_FIELD,
        IPT_TYPE_FIELD,
        IPT_DATA,
        IPT_CRC_FIELD
    };

    bool WriteImageData (Stream& rqStream);
    void WritePixel (const RasterImagePtr& rspqImage, int iX, int iY, char* acDecompressedData, int& riBytePos,
        int& riBitRatio) const;
    void AssignFBAndFilterRow (int iFilterType, int iRow, unsigned char* pucRow, const unsigned char* aucOrigRow,
        const unsigned char* aucOrigUpRow, int iRowSizeWithFB) const;
    static int GetMSADForFilteredRow (const unsigned char* pucRow, int iRowSizeWithFB);
    void GetFilterTypesForRows (StillArray<int>& rqFilterTypes, int iHeight, const unsigned char* aucFirstRow,
        int iRowSizeWithFB) const;
    bool LoadHeadingData (int iBytestreamSize = -1);
    static int PredictPaeth (int iA, int iB, int iC);
    void UnfilterRow (int iRow, unsigned char* pucRow, const unsigned char* aucUpRow, int iRowSizeWithFB) const;
    void ReadPixel (const char* acDecompressedData, int& riBytePos, int& riBitRatio,
        const RasterImagePtr& rspqImage, int iX, int iY) const;

    RasterImagePtr m_spqImageSaveTo;
    int m_iWidth, m_iHeight;
    int m_iBitDepth, m_iSampleRange, m_iSampleMaxValue, m_iSamplesPerPixel, m_iBitsPerPixel;
    ColorModelType m_eColorModel;
    CompressionType m_eCompression;                         // (default: CT_DEFLATE_INFLATE)
    FilterMethodType m_eFilterMethod;                       // (default: FMT_ADAPTIVE)
    InterlaceMethodType m_eInterlaceMethod;                 // (default: IMT_NONE)
    FilterSelectionMethodType m_eFilterSelectionMethod;     // (default: FSM_MINIMUM_SUM_OF_ABSOLUTE_DIFFERENCES)
    DataChunkSplittingType m_eDataChunkSplitting;           // (default: DCST_KILOBYTES_8)
    int m_iNextReadPos;
    bool m_bImageIsInitialized;  // default: false
    int m_iRowSizeWithFB, m_iExpectedImageSize;
    int m_aiWidthInPass[7];
    int m_aiRowSizeWithFBInPass[7];
    int m_aiHeightInPass[7];
    int m_aiPassSize[7];
    int m_iBytesPerImgSample, m_iImgSampleMaxValue;
    bool m_bUniformByteWay;

    bool m_bHas_PLTE_Chunk;                                 // default: false
    RasterImage::Palette m_qPalette;

    bool m_bHas_cHRM_Chunk;                                 // default: false
    float m_fChromaticityWhitePointX, m_fChromaticityWhitePointY;
    float m_fChromaticityRedX, m_fChromaticityRedY;
    float m_fChromaticityGreenX, m_fChromaticityGreenY;
    float m_fChromaticityBlueX, m_fChromaticityBlueY;

    bool m_bHas_gAMA_Chunk;                                 // default: false
    float m_fGamma;

    bool m_bHas_iCCP_Chunk;                                 // default: false
    String m_qICCProfileName;
    StillArray<char> m_qICCProfile;

    bool m_bHas_sBIT_Chunk;                                 // default: false
    int m_iSignificantBitsGrayscale,
        m_iSignificantBitsRed, m_iSignificantBitsGreen, m_iSignificantBitsBlue,
        m_iSignificantBitsAlpha;
    int m_iShiftVGray, m_iShiftVRed, m_iShiftVGreen, m_iShiftVBlue, m_iShiftVAlpha;
    int m_iMaxVGray, m_iMaxVRed, m_iMaxVGreen, m_iMaxVBlue, m_iMaxVAlpha;

    bool m_bHas_sRGB_Chunk;                                 // default: false
    StandardRgbColorSpaceRenderingIntentType m_eStandardRgbColorSpaceRenderingIntent;

    bool m_bHas_bKGD_Chunk;                                 // default: false
    int m_iBackgroundGrayscale,
        m_iBackgroundRed, m_iBackgroundGreen, m_iBackgroundBlue,
        m_iBackgroundPaletteIndex;

    bool m_bHas_hIST_Chunk;                                 // default: false
    StillArray<int> m_qHistogramFrequencies;

    bool m_bHas_tRNS_Chunk;                                 // default: false
    int m_iTransparencyGrayscale,
        m_iTransparencyRed, m_iTransparencyGreen, m_iTransparencyBlue;
    StillArray<unsigned char> m_qPaletteAlphaAddon;

    bool m_bHas_pHYs_Chunk;                                 // (default: true)
    int m_iPixelsPerUnitX;                                  // (default: 2835)
    int m_iPixelsPerUnitY;                                  // (default: 2835)
    PhysicalPixelDimensionsUnitType m_ePhysicalPixelDimensionsUnit;  // (default: PPDUT_METER)

    ModestArray<SuggestedPalette> m_qSuggestedPalettes;

    bool m_bHas_tIME_Chunk;                                 // default: false
    Date m_qLastModificationTime;

    ModestArray<TextualData> m_qTextualDataArray;

    // dynamical loading related
    int m_iDevelopedRowQuantity, m_iCurrentPassIndex;
    InterruptionPointType m_eInterruptionPoint;  // default: IPT_LENGTH_FIELD
    int m_iCurrentChunkLength, m_iCurrentChunkDataBeginningPos;
    StillArray<char> m_qCompressedData, m_qDecompressedRowData, m_qDecompressedUpRowData;
    bool m_bDoneWithTheImage;  // default: false
    int m_iAfterImagePos;
    int m_iDecompressedByteQuantity, m_iY, m_iPass, m_iYInPass;  // default: 0
    void* m_pvZLib;  // default: 0

    // support for saving of textual data
    static const int ms_iMinimalTextLengthToBeCompressed;  // 1024

    // if 'false', always the default ZLib strategy is used
    static const bool ms_bChooseBestFromManifoldZLibStrategies;  // true

    // Support for loading of Latin1 strings.  If 'true', the class turns not allowed characters into question
    // marks and takes out redundant blanks.
    static const bool ms_bCorrectLatin1Limited80StringsForLoading;  // true

    // Support for loading of length-unlimited texts.  If 'true', the class turns not allowed characters into
    // question marks.
    static const bool ms_bCorrectTextsForLoading;  // true
};

#include "QFormatPng.inl"

typedef SmartPointer<FormatPng> FormatPngPtr;

}

#endif



