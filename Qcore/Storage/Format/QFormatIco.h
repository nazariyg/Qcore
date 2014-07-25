#ifndef QFORMATICO_H
#define QFORMATICO_H

// Microsoft Windows Icon/Cursor Image Format.  File extensions: .ico .cur.  The format allows a file to
// contain arbitrary number of images, each with a color model from the list of possible ones.
//
// For saving, acceptable RasterImage's color models are: CMT_PALETTED, CMT_RGB, and CMT_RGBA.  The possible
// bits-per-pixel values in conjunction with the color models are noted below:
//   RasterImage::CMT_PALETTED : the bits-per-pixel value can be 1, 4, or 8;
//   RasterImage::CMT_RGB      : the bits-per-pixel value must be 24;
//   RasterImage::CMT_RGBA     : the bits-per-pixel value must be 32.
// For paletted images, the bpp value must allow image's pixels to address any entry in the palette, making
// true the next: 2^bpp >= palette_size.  A visibility mask must be always present for CMT_PALETTED and CMT_RGB
// images.  Images will be stored within a file in the same order as they are inserted with InsertImage member
// function.  An image passed for saving must have dimensions which do not exceed 255.
//
// For loading, the next image types can appear: IT_PALETTED, IT_RGB_8_8_8, and IT_RGBA_8_8_8_8.  Paletted and
// RGB images always arrive together with a visibility mask associated.  If a paletted image has only two
// colors, then it is not necessary treated as black-and-white, which is the common practice among readers.
// Another common practice is to take image's heading information from the near-pixel area rather than from the
// file's header.

#include "QFormatBase.h"
#include "QProgressBase.h"
#include "QRasterImage.h"

namespace Q
{

class Q_ITEM FormatIco : public FormatBase, public ProgressBase
{
public:
    enum FileResourceType
    {
        FRT_ICON,
        FRT_CURSOR
    };

    // relates only to loading
    enum ColorModelType
    {
        CMT_PALETTED,
        CMT_RGB,
        CMT_RGBA
    };

    // ET_FORMAT_IDENTIFIER_MISMATCH: signature discrepancy, causes no assert-false
    enum ExceptionType
    {
        ET_CANNOT_ACCESS_FILE,              // saving and loading
        ET_FORMAT_IDENTIFIER_MISMATCH,  // loading
        ET_FORMAT_MISMATCH              // loading
    };

    // relates only to loading; holds attributes of a stored image for being able to consider to load actual
    // pixel data or not
    class ImageRecord
    {
    public:
        bool IsSquare;
        int Width, Height;
        ColorModelType ColorModel;
        int PaletteSize;  // meaningful only if an image is paletted; can be 2, 16, or 256
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For saving.
    FormatIco (FileResourceType eFileResource = FRT_ICON);

    // Insertion an icon/cursor image to be saved.  Parameter iBitsPerPixel can be omitted only for paletted
    // images, and in this case the minimal possible bpp is automatically derived from the palette size.
    // Returns the position of insertion.
    int InsertImage (const RasterImagePtr& rqImage, int iBitsPerPixel = -1);

    // Saving.  Progressed.
    bool SaveFile (const char* acFilename);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Construction.  For loading.  Initially tries to read the heading information on each image stored.
    FormatIco (const char* acFilename, bool& rbSuccess);

    // file attributes
    FileResourceType GetFileResourceType () const;
    int GetImageQuantity () const;  // 0 is the valid number
    const ImageRecord& GetImageRecord (int i = 0) const;

    // Loading of an image.  Progressed.
    bool LoadImage (RasterImagePtr& rspqImage, int i = 0);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
    class ImageToSaveRecord
    {
    public:
        RasterImagePtr ImagePtr;
        ColorModelType ColorModel;
        int Bpp;
        int PaletteSize;
    };

    class ImageStoreRecord
    {
    public:
        int BitsPerPixel;
        int ImgByteOffset, ImgByteSize;
    };

    FileResourceType m_eFileResource;
    ModestArray<ImageToSaveRecord> m_qImageToSaveRecords;
    StillArray<ImageStoreRecord> m_qImageStoreRecords;
    int m_iImageQuantity;
    StillArray<ImageRecord> m_qImageRecords;
};

#include "QFormatIco.inl"

}

#endif



