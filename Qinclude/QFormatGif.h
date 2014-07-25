#ifndef QFORMATGIF_H
#define QFORMATGIF_H

// CompuServe Graphics Interchange Format.  File extension: .gif.  A GIF file can be either still, containing a
// single image, or animated, having multiple images.  An image stored inside a GIF file has its own dimensions
// and also a position defined relatively to the logical screen which is to delineate an area where pixels
// should be visible to the viewer.  The opening image contained by a GIF file is called "title image".  Only
// the title image can be interlaced in order to provide the ability of progressive image displaying while data
// is transmitting.  All sorts of interactions with GIF's "transparency color" replaced by interaction with
// RasterImage's visibility mask.  Saving and loading use the next common understanding of GIF's term
// "background": it means no certain color but an under-laying graphics instead.  Netscape Navigator's
// "N-Loops" extension is supported.
//
// For saving, only RasterImage's CMT_PALETTED color model is acceptable.  An image to be saved into GIF
// representation can be with or without a visibility mask.  WARNING. Both saving kinds, still-oriented and
// animation-oriented, will require an additional entry in GIF's color table with the intention of signifying
// transparent pixels if the image or an animation frame has a visibility mask associated, thus a palette with
// 256 entries is incompatible with a masked image.  Accordingly, if all animation frames have the same shared
// palette, presence of a visibility mask at least at one frame will raise size of the output global palette by
// one.  Produced GIF files have version "89a".  If "manual" animation assembling is used instead of automatic
// which involves GenerateInputAnimation static member function, the user should note that the first frame
// cannot have disposal method DT_RESTORE_TO_PREVIOUS.  An image passed for saving must have dimensions which
// do not exceed 65535.
// DEFAULTS. The title image is not to be interlaced; loop_quantity is 0 ("loop infinity times").
//
// For loading, the only image type which can appear is IT_PALETTED.  If an image has transparency in GIF
// domain, it comes in with a visibility mask.  Loading accepts both GIF versions, "87a" and "89a".  A GIF file
// is treated as animated if it has a Graphic Control Extension block before the first image.  Absence of the
// Netscape's extension field for an animation being loaded is understood as "play once", which is the common
// practice.  A lot of concessions are made to the format for loading purpose since there were and, most
// likely, still are many imperfectly designed GIF encoders whose ways of file stuffing have become widespread.
// Some notable concessions are:
//   if size of the received image data less than the expected, then lacking number of bytes is appended using
//     zeroes (it means that unknown image areas will be colored by the first color from the color table);
//   if a color index points out of the color table, then 0 index is used;
//   if an unknown disposal method is encountered, then DT_LEAVE replaces it;
//   if an animation frame has no Graphic Control Extension, then this frame is considered as the last and GCE
//     from the previous frame is used.
// The "Deferred clear code" feature is supported in the LZW implementation.  In loading from a dynamic
// bytestream, LZW decoding resumes just from the pixel at which it had stopped before, therefore no redundant
// performance and memory resources are involved.  Due to neglecting by some GIF encoders the specification,
// loading of a GIF image happens into the allocated "draft image" in order to allow the user displaying a
// failed-to-load image if it's required.  In the case of loading from a dynamic bytestream, if all expected
// bytes were received but loading function still returns 'false', then it's acceptable for the control logic
// to finish, with treating that file as normal.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImageSequence.h"

namespace Q
{

class Q_ITEM FormatGif : public FormatBase, public ProgressBase
{
public:
    enum DisposalType
    {
        DT_UNSPECIFIED,
        DT_LEAVE,
        DT_RESTORE_TO_BACKGROUND,
        DT_RESTORE_TO_PREVIOUS
    };

    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    // ET_FILE_HAS_NO_IMAGES: this case allowed by the specification
    // ET_PLAIN_TEXT_EXTENSION_NOT_SUPPORTED: representation of a graphical image via text characters is
    //   undoubtedly obsolete and hence unsupported
    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,                 // saving and loading
        ET_FORMAT_IDENTIFIER_MISMATCH,     // loading
        ET_FORMAT_MISMATCH,                // loading
        ET_FILE_HAS_NO_IMAGES,                 // loading
        ET_PLAIN_TEXT_EXTENSION_NOT_SUPPORTED  // loading
    };

    // animation frame holder
    class AnimationFrame
    {
    public:
        // construction
        AnimationFrame () {};  // uninitialized
        AnimationFrame (const RasterImagePtr& rspqImage, DisposalType eDisposalMethod, int iDelayTime,
            int iPositionX, int iPositionY, bool bUserInputExpected = false)
            :
            ImagePtr(rspqImage),
            DisposalMethod(eDisposalMethod),
            DelayTime(iDelayTime),
            PositionX(iPositionX),
            PositionY(iPositionY),
            UserInputExpected(bUserInputExpected)
        {
        };

        RasterImagePtr ImagePtr;
        DisposalType DisposalMethod;
        int DelayTime;              // in 1/100 of a second
        int PositionX, PositionY;
        bool UserInputExpected;

    private:
        friend class FormatGif;

        bool m_bHasTransparency;
        int m_iTransparentColorIndex;
    };
    typedef StillArray<AnimationFrame> Animation;

    // destruction
    ~FormatGif ();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.  When storing an animation, all frame images must have the same dimensions as
    // the logical screen has.  Parameter bAllPalettesAreEqual indicates if a single global palette common to
    // all frames is to be used instead of per-frame local palettes; it must be 'false' if animation frames'
    // palettes are known to differ or if the situation is simply uncertain.  The minimal possible
    // bits-per-pixel value to be used in encoding is automatically derived from appropriate palette's size.
    FormatGif (const RasterImagePtr& rspqImage);
    FormatGif (const Animation& rqAnimation, int iLogicalScreenWidth, int iLogicalScreenHeight,
        bool bAllPalettesAreEqual);

    // file settings
    void SetIsTitleImageInterlaced (bool bInterlaced);  // only for a still GIF
    void SetLoopQuantity (int iQuantity);               // [0, 65535], 0 is for "replay infinity times"

    // Saving of a still image.  Progressed.
    bool SaveImage (const char* acFilename);
    void SaveImage (StillArray<char>& rqStaticBytestream);

    // Saving of an animation.  A produced file always has a Global Color Table.  All frames are
    // non-interlaced.  Progressed.
    bool SaveAnimation (const char* acFilename);
    void SaveAnimation (StillArray<char>& rqStaticBytestream);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading data (Logical Screen Descriptor, Global
    // Color Table, and Netscape Navigator's "N-Loops" extension field) and to reach the first Image
    // Descriptor.
    FormatGif (const char* acFilename, bool& rbSuccess);
    FormatGif (bool bBytestreamIsStatic, const char* acBytestream, int iBytestreamSize, bool& rbSuccess);

    // file attributes
    int GetLogicalScreenWidth () const;
    int GetLogicalScreenHeight () const;
    bool IsAnimated () const;
    bool IsTitleImageInterlaced () const;
    int GetLoopQuantity () const;

    // Loading of the title image.  NOTE. The GIF interlacing scheme assumes possibility of presence of "empty
    // passes", so, in the case of loading from a dynamic bytestream, the image may be transmitted completely
    // even if loaded_pass_quantity hasn't reached 4.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage, int iDynamicBytestreamSize = -1);

    // For animated GIFs, the function loads frames into rqAnimation.  In the case of loading from a dynamic
    // bytestream, size of rqAnimation grows with time by transmitted frames.  NOTE. If the first frame of a
    // multiframe animation fails to load, then this function returns 'false', but if some other frame fails to
    // load, then it's considered as partial success (using GIF's indulgent criteria) and 'true' is returned.
    bool LoadAnimation (Animation& rqAnimation, int iDynamicBytestreamSize = -1);

    // Supporting calls (for the loading functions above) in case when the feature of loading from a dynamic
    // bytestream is used for transmitting the title image.  Return value of GetDevelopedRowQuantity can vary from
    // 0 to logical_screen_height inclusively.  For non-interlaced images only GetDevelopedRowQuantity is usable,
    // allowing an application to hide not yet loaded rows.  If the image is interlaced, return value of
    // GetCurrentPassIndex can vary from 0 to 3 inclusively, but it may jump e.g. from 1 to 3 because some
    // passes may be empty for small images.  For interlaced case, loaded_row_quantity resets every time as a
    // new pass starts to unroll.  Generally speaking, the application may not take into account values from
    // GetDevelopedRowQuantity for passes other than the first (for which hiding needed), just updating the
    // display image wholly while it isn't transmitted completely.
    int GetDevelopedRowQuantity () const;
    int GetCurrentPassIndex () const;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // The next two static functions are, respectively, to generate a ready-to-save GIF animation from a
    // sequence of raster images, and from a loaded GIF animation a ready-to-display image sequence.

    // From a raster image sequence provided, this function assembles an animation with the smallest size
    // possible for future reincarnation into a GIF file.  The function is responsible for setting of the next
    // data members of an AnimationFrame object: ImagePtr, DisposalMethod, and PositionX with PositionY; the
    // remaining is left for the user.  All images in rqImageSequence must be already paletted and can be with
    // or without a visibility mask.  Parameter rqAnimation must be allocated and have the same length as
    // rqImageSequence (data members of AnimationFrame objects may be uninitialized).  If bCropFrameImages is
    // 'true', then only a subimage of a frame will be saved whenever it's possible reasoning from pixels of
    // the previous frame: the changing pixels' bounding rectangular area is written in such cases instead of
    // the full frame.  Setting to 'true' bWriteUnchangingPixelsAsTransparency means writing transparency
    // color's palette index for pixels which remain still if compare with the previous frame: this allows by
    // now "looking through this frame at the previous one" on per-pixel level, which increases compression
    // efficiency as well.  Ability of lossy compression is supplied by parameter fRequiredPixelSimilarity that
    // defines which difference between two corresponding pixels from two adjacent frames is to be considered
    // as meaning; extreme values are: 0.0 -- almost all pixel pairs will fall into "the same", 1.0 -- strict
    // equality is needed to consider two pixels as "the same".  The last animation frame gets disposal method
    // DT_RESTORE_TO_BACKGROUND.  Color reduction and palette shrinking are out of the scope of this function.
    static void GenerateInputAnimation (const RasterImageSequence& rqImageSequence, Animation& rqAnimation,
        bool bCropFrameImages, bool bWriteUnchangingPixelsAsTransparency, float fRequiredPixelSimilarity = 1.0f);

    // Taking a GIF animation, this function produces a sequence of raster images with the same dimensions
    // correspondingly to the logical screen, all having IT_RGB_8_8_8 type and, for the uniformity reasons,
    // each with a visibility mask associated.  Parameter array rqImageSequence must have the same length as
    // rqAnimation (images inside the sequence should be uninitialized).  Parameter rqForRestToPrevImages is
    // just a smart pointer with no special meaning for the user; it must be created beforehand and freed when
    // it's done (it is caused by presence of the feature of loading from a dynamic bytestream).  Setting of
    // bLastFrameOnly to 'true' may be useful when image sequence construction is needed on-the-fly while
    // entire animation is not yet downloaded from a dynamic bytestream, so only an image from the last
    // animation frame, after that frame is freshly transmitted, is processed and appended to the image
    // sequence.
    static void GenerateOutputImageSequence (const Animation& rqAnimation, RasterImageSequence& rqImageSequence,
        StillArray<RasterImagePtr>& rqForRestToPrevImages, bool bLastFrameOnly = false);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    void WriteImage (Stream& rqStream);
    void WriteAnimation (Stream& rqStream);
    void WriteImageData (const RasterImagePtr& rspqImage, int iBitsPerPixel, bool bInterlaced,
        ProgressProbe& rqProgress, float fScopeCoefficient, Stream& rqStream);
    static bool ArePixelsSimilar (int iX0, int iY0, int iX1, int iY1, const RasterImagePtr& rspqImage0,
        const RasterImagePtr& rspqImage1, float fRequiredPixelSimilarity);
    bool LoadHeadingData (int iBytestreamSize = -1);
    int LoadImageData (int& riNextReadPos, int iWidth, int iHeight, bool bInterlaced,
        const RasterImage::Palette& rqActiveColorTable, AnimationFrame* pqAnimationFrame, int iBytestreamSize,
        bool bAnimationFrame, RasterImagePtr& rspqImage);
    int GetIrlcRow (int iPass, int iRowWithinPass) const;
    void UpdateInterlacedView (RasterImagePtr& rspqImage) const;

    RasterImagePtr m_spqImageSaveTo;
    Animation m_qAnimationSaveTo;
    StillArray<RasterImage::Palette> m_qLocalColorTablesSaveTo;
    StillArray<int> m_qFrameBitsPerPixelSaveTo;
    int m_iLogicalScreenWidth, m_iLogicalScreenHeight;
    int m_iBitsPerPixel;
    bool m_bHasGlobalColorTable;
    RasterImage::Palette m_qGlobalColorTable;
    int m_iBackgroundColorIndex;
    bool m_bIsAnimated;                         // (default: false)
    bool m_bTitleImageIsInterlaced;             // (default: false)
    int m_iLoopQuantity;
    int m_iFirstIDOrGCEPosition;
    int m_iDevelopedRowQuantity, m_iCurrentPassIndex;
    StillArray<char> m_qCompressedImageData, m_qDecompressedImageData;
    StillArray<bool> m_qRowsToFilledState;
    StillArray<char> m_qSavedImageRow;
    RasterImage::VisibilityMask m_qSavedVMRow;
    bool m_bDraftImageAllocated;                // (default: false)
    bool m_bDynaInitialized;                    // (default: false)
    bool m_bHasRestorePoint;                    // (default: false)
    bool m_bHasNRPALSB;                         // (default: false)
    bool m_bHasAnimationRestorePoint;           // (default: false)
    int m_iNextReadPosAfterLastSubBlock, m_iRestoreX, m_iRestoreY, m_iRestoreBytePos, m_iRestoreBitRatio,
        m_iRestorePass, m_iRestoreRowWithinPass, m_iNextReadPosAfterLastAnimationFrame, m_iAnimationRestorePointId;
    bool m_bRestoreLastFrameWithNoGCE;
    void* m_pvLzw;  // default: 0

    static const PaletteColor ms_qTransparencyColor;   // black
    static const PaletteColor ms_qDraftColor;          // white
    static const int ms_aiIrlcStrt[4];
    static const int ms_aiIrlcStep[4];
    static const float ms_fPixelSimilarityBaseMapper;  // 0.95
};

#include "QFormatGif.inl"

typedef SmartPointer<FormatGif> FormatGifPtr;

}

#endif



