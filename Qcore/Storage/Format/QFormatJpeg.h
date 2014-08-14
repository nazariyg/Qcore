#ifndef QFORMATJPEG_H
#define QFORMATJPEG_H

// Joint Photographic Experts Group Image Interchange Format.  File extensions: .jpg .jpeg .jpe.  Arithmetic
// coding is not implemented because of the patent.  DNL markers are not supported due to their total vainness.
// Forward and inverse discrete cosine transformations use integer arithmetic algorithms.
//
// For saving, acceptable RasterImage's color models are: CMT_GRAYSCALE, CMT_RGB, and CMT_CMYK.  A JPEG
// internal color model can be chosen when storing an image, but only a one compatible with the color model of
// that image:
//   RasterImage::CMT_GRAYSCALE : only CMT_GRAYSCALE;
//   RasterImage::CMT_RGB       : CMT_YCC or CMT_RGB;
//   RasterImage::CMT_CMYK      : CMT_YCCK or CMT_CMYK.
// If the internal color model is not set, then the most matching one is derived automatically:
//   RasterImage::CMT_GRAYSCALE : CMT_GRAYSCALE;
//   RasterImage::CMT_RGB       : CMT_YCC;
//   RasterImage::CMT_CMYK      : CMT_YCCK.
// An image must be without a visibility mask.  All JPEG's color modes imply 8 bits per sample.  An image
// passed for saving must have dimensions which do not exceed 65535.  JPEG quality 0.5 results into usage of
// the default quantization tables supplied by the format specification; other values do linear mapping of
// these tables if < 0.5, and non-linear mapping if > 0.5; quality value 0.0 brings images with 8x8 "dots",
// value 1.0 produces no quantization at all, which is, theoretically, loss-less.  The downsampling schemes
// 4:4:4 and 4:2:2 can be applied only to a YCbCr image.  The effect from tweaking of scan quantity for a
// progressive JPEG image is identical to that in Adobe Photoshop.  A peculiarity exists concerning some
// markers and JPEG's color models: JFIF marker is not written in case of CMT_RGB, Adobe marker is not written
// in case of CMT_GRAYSCALE.  Saving writes version 1.1 into the JFIF marker.  In writing of JPEG's CMT_RGB,
// the component identifiers get numbers equal to the char codes in ASCII "RGB" and, similarly, for CMT_CMYK it
// is "CMYK".  Saving uses optimized Huffman entropy coding tables.
// DEFAULTS. Data organization is DOT_PLANE; JPEG compression quality is 0.75; YCbCr chromaticity sampling is
// YCCCST_4_4_4; progressive scan quantity is 5; JFIF marker is enabled, dot density meaning is
// JFIFPDMT_PIXELS_PER_INCH, horizontal and vertical density both are 72; Adobe marker is enabled.
//
// For loading, an image arrives with type IT_GRAYSCALE_8, IT_RGB_8_8_8, or IT_CMYK_8_8_8_8.  Without a
// visibility mask.  Since JPEG is a complex format, contemporary graphical applications support only a subset
// of it, so that's why the list of possible exceptions is not very small.
// DEFAULTS. Upsampling filter is UFT_BILINEAR.

#include "QRasterImage.h"
#include "QProgressBase.h"
#include "QFormatExif.h"

namespace Q
{

class Q_ITEM FormatJpeg : public FormatBase, public ProgressBase
{
public:
    enum ColorModelType
    {
        CMT_GRAYSCALE,
        CMT_YCC,   // YCbCr
        CMT_RGB,
        CMT_YCCK,  // Adobe-style CMYK
        CMT_CMYK   // plane CMYK
    };

    // relates only to saving; with 4:2:2 scheme it's possible to reduce file size approximately on 15-25% at
    // the cost of a little quality reduction
    enum YCbCrChromaSamplingType
    {
        YCCCST_4_4_4,  // 4:4:4, either chromaticity channel isn't subsampled
        YCCCST_4_2_2   // 4:2:2, either chromaticity channel has dimensions half of original
    };

    enum DataOrganizationType
    {
        DOT_PLANE,       // regular JPEG ("Baseline/Extended DCT Huffman")
        DOT_PROGRESSIVE  // + extra ability of progressive display while transmitting ("Progressive DCT Huffman")
    };

    enum JfifDotDensityMeaningType
    {
        JFIFPDMT_ASPECT_RATIO,
        JFIFPDMT_PIXELS_PER_INCH,
        JFIFPDMT_PIXELS_PER_CENTIMETER
    };

    // relates only to loading
    enum UpsamplingFilterType
    {
        UFT_BOX,      // fast
        UFT_BILINEAR  // somewhat slower but better
    };

    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    // ET_LOSSLESS_COMPRESSION_NOT_SUPPORTED: in case of JPEG-LS
    // ET_HIERARCHICAL_COMPRESSION_NOT_SUPPORTED: the image is in the hierarchical mode which is not supported
    // ET_NON_8_BPS_PRECISION_NOT_SUPPORTED: most likely that the image has 12 bits-per-sample precision
    // ET_COMPONENT_QUANTITY_NOT_SUPPORTED: unknown component layout
    // ET_JFIF_VERSION_NOT_SUPPORTED: JFIF marker flags a version greater than 1.02
    // ET_COMPONENTS_ARE_NOT_INTERLEAVED: the image is progressive but the component layout contradicts it
    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,                      // saving and loading
        ET_FORMAT_IDENTIFIER_MISMATCH,              // loading
        ET_FORMAT_MISMATCH,                         // loading
        ET_ARITHMETIC_CODING_NOT_SUPPORTED,         // loading
        ET_LOSSLESS_COMPRESSION_NOT_SUPPORTED,      // loading
        ET_HIERARCHICAL_COMPRESSION_NOT_SUPPORTED,  // loading
        ET_NON_8_BPS_PRECISION_NOT_SUPPORTED,       // loading
        ET_COMPONENT_QUANTITY_NOT_SUPPORTED,        // loading
        ET_DNL_MARKER_NOT_SUPPORTED,                // loading
        ET_JFIF_VERSION_NOT_SUPPORTED,              // loading
        ET_COMPONENTS_ARE_NOT_INTERLEAVED           // loading
    };

    // WT_JFIF_MARKER_IS_INCORRECT: optional JFIF marker is incorrect
    // WT_EXIF_METADATA_ARE_INCORRECT: Exif metadata is incorrect
    enum WarningType
    {
        WT_JFIF_MARKER_IS_INCORRECT,    // loading
        WT_EXIF_METADATA_ARE_INCORRECT  // loading
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.
    FormatJpeg (const RasterImagePtr& rspqImage);

    // file settings
    void SetColorModel (ColorModelType eColorModel);
    void SetQuality (float fQuality);  // the range is [0.0, 1.0]
    void SetYCbCrChromaSampling (YCbCrChromaSamplingType eYCbCrChromaSampling);  // only if color model is CMT_YCC
    void SetDataOrganization (DataOrganizationType eDataOrganization);
    void SetProgressiveScanQuantity (int iQuantity);  // only if data org. is DOT_PROGRESSIVE; can be 3, 4, or 5

    // JFIF marker related
    void SetJfifMarker (JfifDotDensityMeaningType eJfifDotDensityMeaning, int iJfifDotDensityHorizontally,
        int iJfifDotDensityVertically);
    void UnsetJfifMarker ();

    // Adobe marker related
    void UnsetAdobeMarker ();  // should be used knowledgeably

    // Exif metadata related
    void SetExifMetadata (const FormatExifPtr& rspqExifMetadata);

    // Saving.  Progressed.
    bool SaveImage (const char* acFilename);
    void SaveImage (StillArray<char>& rqStaticBytestream);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data.
    FormatJpeg (const char* acFilename, bool& rbSuccess);
    FormatJpeg (bool bBytestreamIsStatic, const char* acBytestream, int iBytestreamSize, bool& rbSuccess);

    // file attributes
    int GetWidth () const;
    int GetHeight () const;
    int GetBitsPerPixel () const;
    ColorModelType GetColorModel () const;
    DataOrganizationType GetDataOrganization () const;

    // JFIF marker related
    bool HasJfifMarker () const;
    JfifDotDensityMeaningType GetJfifDotDensityMeaning () const;
    int GetJfifDotDensityHorizontally () const;
    int GetJfifDotDensityVertically () const;

    // Exif metadata related
    bool HasExifMetadata () const;
    const FormatExif& GetExifMetadata () const;

    // for changing of the upsampling filter which participates in rescaling of chromaticity information
    void SetUpsamplingFilter (UpsamplingFilterType eUpsamplingFilter);

    // Loading.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage, int iDynamicBytestreamSize = -1);

    // Support for loading from a dynamic bytestream.  Return value of GetDevelopedRowQuantity can vary from 0 to
    // image_height inclusively.  For images which are non-progressive, only GetDevelopedRowQuantity is usable,
    // allowing applications to hide not yet loaded rows.  Generally speaking, in case of a progressive JPEG an
    // application may not take into account value from GetDevelopedRowQuantity for passes other than the first
    // (for which hiding is needed), just updating the image on the display wholly while it isn't transmitted
    // completely.  Due to the format's design, it is unknown beforehand how many passes has this or that
    // progressive JPEG.
    int GetDevelopedRowQuantity () const;
    int GetCurrentPassIndex () const;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    // support for loading
    enum ProcessType
    {
        PT_BASELINE_DCT,
        PT_EXTENDED_DCT,
        PT_PROGRESSIVE_DCT
    };

    // support for loading
    enum ResumeZoneType
    {
        IZT_TABLES_AND_MISC,
        IZT_NON_PROGRESSIVE_MCU,
        IZT_PROGRESSIVE_DC_INITIAL_MCU,
        IZT_PROGRESSIVE_DC_SUCCEEDING_MCU,
        IZT_PROGRESSIVE_AC_INITIAL_MCU,
        IZT_PROGRESSIVE_AC_SUCCEEDING_MCU,
        IZT_RESTART_INTERVAL,
        IZT_END_OF_SCAN
    };

    // support for saving
    class ScanProgressionParams
    {
    public:
        ScanProgressionParams () {};
        ScanProgressionParams (int iStartOfSpectral, int iEndOfSpectral, int iSuccApproxBitPosHigh,
            int iSuccApproxBitPosLow, int iComponent = -1)
            :
            StartOfSpectral(iStartOfSpectral),
            EndOfSpectral(iEndOfSpectral),
            SuccApproxBitPosHigh(iSuccApproxBitPosHigh),
            SuccApproxBitPosLow(iSuccApproxBitPosLow),
            Component(iComponent),
            Dc(( iStartOfSpectral == 0 ))
        {
        };

        bool Dc;
        int StartOfSpectral, EndOfSpectral, SuccApproxBitPosHigh, SuccApproxBitPosLow, Component;
    };

    class HuffmanTableSet
    {
    public:
        HuffmanTableSet ()
        {
            BitsList[0] = 0;
        };

        int BitsList[17];
        StillArray<int> HuffValList;
        int MinCode[17];
        int MaxCode[17];
        int ValueOffset[17];
    };

    class FrameCompSpecParams
    {
    public:
        unsigned char ComponentIdentifier;
        int SamplingFactorH;
        int SamplingFactorV;
        unsigned char QuantTableDestSelector;
        int ProgressiveBufferWidthInDataUnits;
    };

    // support for loading
    class ScanCompSpecParams
    {
    public:
        int ComponentIndexInFrame;
        FrameCompSpecParams* FrameCompSpecParamsPtr;
        HuffmanTableSet* DcHuffmanTableSetPtr;
        HuffmanTableSet* AcHuffmanTableSetPtr;
        int WidthInDataUnits;
    };

    void WriteImageData (Stream& rqStream);
    static void GatherStatistics (const StillArray<short>& rqBlock, int& riPrevDc, unsigned int* auiDcFrequencies,
        unsigned int* auiAcFrequencies);
    static void GetHuffmanTableSetFromStatistics (HuffmanTableSet& rqHTS, unsigned int* auiFrequencies);
    static int GetHuffmanDerivedTables (StillArray<int>& rqHuffCodeList, StillArray<int>& rqHuffSizeList,
        const HuffmanTableSet& rqHTS, int* piLastK = 0);
    static void EncodeBlockHuffman (const StillArray<short>& rqBlock, int& riPrevDc,
        const StillArray<int>& rqDcHuffCodeOrderedList, const StillArray<int>& rqDcHuffSizeOrderedList,
        const StillArray<int>& rqAcHuffCodeOrderedList, const StillArray<int>& rqAcHuffSizeOrderedList,
        Stream& rqStream);
    static void EncodeHuffmanBits (unsigned int* auiFrequencies, int iValue, int iBitSize, Stream* pqStream);
    static void EncodeHuffmanBuffBits (unsigned int* auiFrequencies, char* acBitBuff, int iBitSize,
        Stream* pqStream);
    static void EncodeHuffmanSymbol (unsigned int* auiFrequencies, int iSymbol, const int* aiHuffCodeOrderedList,
        const int* aiHuffSizeOrderedList, Stream* pqStream);
    static void EncodeHuffmanEobRun (unsigned int* auiFrequencies, int& riEobRun, const int* aiHuffCodeOrderedList,
        const int* aiHuffSizeOrderedList, char* acBitBuff, int& riBBQ, Stream* pqStream);
    static void EncodeDcInitial (unsigned int* auiFrequencies, const StillArray<short>& rqBlock, int& riPrevDc,
        const ScanProgressionParams& rqSPP, const int* aiHuffCodeOrderedList, const int* aiHuffSizeOrderedList,
        Stream* pqStream);
    static void EncodeDcSucceeding (const StillArray<short>& rqBlock, const ScanProgressionParams& rqSPP,
        Stream* pqStream);
    static void EncodeAcInitial (unsigned int* auiFrequencies, const StillArray<short>& rqBlock,
        const ScanProgressionParams& rqSPP, int& riEobRun, const int* aiHuffCodeOrderedList,
        const int* aiHuffSizeOrderedList, char* acBitBuff, int& riBBQ, Stream* pqStream);
    static void EncodeAcSucceeding (unsigned int* auiFrequencies, const StillArray<short>& rqBlock,
        const ScanProgressionParams& rqSPP, int& riEobRun, const int* aiHuffCodeOrderedList,
        const int* aiHuffSizeOrderedList, char* acBitBuff, int& riBBQ, Stream* pqStream);
    bool LoadHeadingData (int iBytestreamSize = -1);
    int ReachMarkerThroughTablesAndMisc (unsigned char ucMarker2ndByte, Stream& rqStream);
    void CompileImage (RasterImagePtr& rspqImage);
    static int DoNextBitProc (int& riBit, unsigned char& rucB, int& riCnt, Stream& rqStream);
    static int DoHuffmanDecodeProc (int& riValue, unsigned char& rucB, int& riCnt, const HuffmanTableSet& rqHTS,
        Stream& rqStream);
    static int DoReceiveProc (int& riV, int iSsss, unsigned char& rucB, int& riCnt, Stream& rqStream);
    static void DoExtendProc (int& riV, int iV, int iT);

    RasterImagePtr m_spqImageSaveTo;
    int m_iWidth, m_iHeight;
    int m_iDevelopedRowQuantity, m_iCurrentPassIndex;
    DataOrganizationType m_eDataOrganization;                        // (default: DOT_PLANE)
    ProcessType m_eProcess;
    ColorModelType m_eColorModel;                                    // (default: autodetect)
    float m_fQuality;                                                // (default: 0.75)
    YCbCrChromaSamplingType m_eYCbCrChromaSampling;                  // (default: YCCCST_4_4_4)
    int m_iProgressiveScanQuantity;                                  // (default: 5)
    UpsamplingFilterType m_eUpsamplingFilter;                        // (default: UFT_BILINEAR)
    bool m_bHasJfifMarker;                                           // (default: true)
    JfifDotDensityMeaningType m_eJfifDotDensityMeaning;              // (default: JFIFPDMT_PIXELS_PER_INCH)
    int m_iJfifDotDensityHorizontally;                               // (default: 72)
    int m_iJfifDotDensityVertically;                                 // (default: 72)
    bool m_bColorModelPrecised;
    bool m_bHasAdobeMarker;                                          // (default: true)
    bool m_bHasExifMetadata;
    FormatExifPtr m_spqExifMetadata;
    bool m_bImageInitialized, m_bRestartIntervalsEnabled, m_bScanIsFirst, m_bEcsIsLast;
    unsigned char m_ucInScanComponentQuantity, m_ucStartOfSpectral, m_ucEndOfSpectral, m_ucResumeB;
    int m_iFrameMarkerSecondByte, m_iSamplePrecision, m_iComponentQuantity, m_iMaxSamplingFactorH,
        m_iMaxSamplingFactorV, m_iDynamicBytestreamResumeBytePosition, m_iRestartInterval,
        m_iDecodedComponentQuantity, m_iSuccApproxBitPosHigh, m_iSuccApproxBitPosLow, m_iInScanMcuQuantity,
        m_iInEcsMcuQuantity, m_iRemainingMcuQuantity, m_iRSTCounter, m_iMcu, m_iComp, m_iIDUY, m_iIDUX,
        m_iIDUHeight, m_iIDUWidth, m_iResumeCnt, m_iMcuRowHeight, m_iResumeEobRun, m_iSABPLTopVal,
        m_iSABPLNegTopVal, m_iDevelopedRowQuantityUB, m_iDynamicSamplingCausedRowMargine, m_iDYPRangeTop,
        m_iDYPRangeBottom;
    short* m_asBlockPtr;
    short m_asBlockArray[64];
    char m_acBlock[64];
    short m_asResumeBlock[64];
    int m_aiCrInR[256];
    int m_aiCbInG[256];
    int m_aiCrInG[256];
    int m_aiCbInB[256];
    ScanCompSpecParams* m_pqSCSP;
    RasterImagePtr m_spqComponentImage;
    Link<int,int>* m_pqLTC;
    StillArray<FrameCompSpecParams> m_qFrameCompSpecParamsArray;
    int m_aaiQuantizationTableDestination[4][64];
    HuffmanTableSet m_aaqHuffmanTableDestination[2][4];
    StillArray<ScanCompSpecParams> m_qScanCompSpecParamsArray;
    StillArray<RasterImagePtr> m_qComponentImages;
    StillArray<StillArray<StillArray<short> > > m_qComponentBuffers;
    StillArray<Link<Link<int,int>,Link<int,int> > > m_qProgressionRecordsPerComponent;
    StillArray<Link<int,int> > m_qLeftTopCoordArray;
    StillArray<int> m_qPredArray;
    ResumeZoneType m_eResumeZone;
    int m_iBitsPerPixel;
    static const int ms_iBitBuffSize;
};

#include "QFormatJpeg.inl"

typedef SmartPointer<FormatJpeg> FormatJpegPtr;

}

#endif



