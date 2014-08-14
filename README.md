# Qcore

**Qcore** is a C/C++ library that implements smart pointers, run-time type introspection, efficient arrays, maps, sets, and lists, fast sorting, various algorithms for data compression, image processing, about 20 image formats, pieces from computational geometry, and a lightweight but well-featured 3D engine.

Qcore library primarily focuses on ease of use, performance, reliability, and overall Quality.

The platform-specific declarations in [System/QSystem.h](Qcore/System/QSystem.h) lay out a foundation upon which the library is built. The supported platforms are Windows (MSVC), OS X, and Linux.

The classes that are located in [Storage/Heap/](Qcore/Storage/Heap) include the implementation of the smart pointer system, which allows for smart pointers to be used in C++ with native look and feel. Put into life mainly by the QHeap class ([.h](Qcore/Storage/Heap/QHeap.h), [.cpp](Qcore/Storage/Heap/QHeap.cpp)) and its satellite QSmartPointer class ([.h](Qcore/Storage/Heap/QSmartPointer.h), [.inl](Qcore/Storage/Heap/QSmartPointer.inl)), the smart pointer system of Qcore takes away most of the worries about when and how the heap memory allocated for an object of a class is going to be deallocated. An object of any class that derives from the QHeap class gets destructed and deallocated automatically when the object's reference counter hits zero with the last smart pointer having detached from the object, which entails the destruction of the object's member data and so on. On practice, smart pointers are just like regular C++ pointers. Any smart pointer evaluates to the address of the object it points to, its `->` operator is used for accessing the member functions and member data of the object, `*` operator is used for dereferencing, `=` operator for assigning, and `==` with `!=` operators for comparison. Other C++ libraries may have memory leaks if they want, but Qcore hasn't got time for any of this.

The efficient array, map, set, list, and iterator implementations that replace some of the container and iterator classes shipped with the C++ Standard Library are gathered in [Storage/Containment/](Qcore/Storage/Containment). The `std::vector` class of the C++ Standard Library loses its monopoly on array functionality and is substituted by a selection of three classes that the developer can choose from to optimize performance in each specific case of array use. And these array classes are QStillArray ([.h](Qcore/Storage/Containment/QStillArray.h), [.inl](Qcore/Storage/Containment/QStillArray.inl)), which is optimal for arrays that are expected to stay of the same size ever since they were created with a number of preallocated elements, QArray ([.h](Qcore/Storage/Containment/QArray.h), [.inl](Qcore/Storage/Containment/QArray.inl)), which derives from the QDynamicArrayBase class ([.h](Qcore/Storage/Containment/QDynamicArrayBase.h), [.inl](Qcore/Storage/Containment/QDynamicArrayBase.inl)) and works best with arrays that are expected to grow or shrink dynamically and possibly by large numbers of elements, and QModestArray ([.h](Qcore/Storage/Containment/QModestArray.h), [.inl](Qcore/Storage/Containment/QModestArray.inl)), which also derives from the QDynamicArrayBase class and is most suitable for arrays that are expected to grow or shrink dynamically by small numbers of elements. Another essential container class is QMap ([.h](Qcore/Storage/Containment/QMap.h), [.inl](Qcore/Storage/Containment/QMap.inl)) that replaces the associative array functionality of the `std::map` class with a highly optimized implementation of the red-black tree algorithm for key-value storage, and the QMap class is accompanied with the QMapIterator class ([.h](Qcore/Storage/Containment/QMapIterator.h), [.inl](Qcore/Storage/Containment/QMapIterator.inl)) that implements iterators for the container. As an alternative to the QMap class, there is also the QHashMap class ([.h](Qcore/Storage/Containment/QHashMap.h), [.inl](Qcore/Storage/Containment/QHashMap.inl)) that improves the performance of key-value storage as long as the keys are kept small in size and the class is a more optimal choice over QMap if the keys are pointers or objects (which are equivalent to pointers); the QHashMap class goes together with the QHashMapIterator class ([.h](Qcore/Storage/Containment/QHashMapIterator.h), [.inl](Qcore/Storage/Containment/QHashMapIterator.inl)) for iterators. Qcore also offers a wider choice for set functionality with the QSet class ([.h](Qcore/Storage/Containment/QSet.h), [.inl](Qcore/Storage/Containment/QSet.inl)) that implements the storage of unique values by means of red-black trees and the QBsSet class ([.h](Qcore/Storage/Containment/QBsSet.h), [.inl](Qcore/Storage/Containment/QBsSet.inl)) that takes advantage of the exceptionally fast binary search algorithm for sets of values that require much more lookup operations than insert operations. Doubly-linked lists are implemented by the QList class ([.h](Qcore/Storage/Containment/QList.h), [.inl](Qcore/Storage/Containment/QList.inl)) along with the QListIterator class ([.h](Qcore/Storage/Containment/QListIterator.h), [.inl](Qcore/Storage/Containment/QListIterator.inl)), which are most useful for arrays that require frequent insert and remove operations.

The QSortUtils class ([.h](Qcore/Storage/Sorting/QSortUtils.h), [.inl](Qcore/Storage/Sorting/QSortUtils.inl)), which is located in [Storage/Sorting/](Qcore/Storage/Sorting), provides a lightning-fast implementation of the efficient Quicksort sorting algorithm and lets it be applicable to C++ native arrays, objects of the Qcore's array classes, including doubly-linked lists, all with an option for using a custom comparator for the comparison of the elements to be sorted.

In [Storage/Compression/](Qcore/Storage/Compression) you can find classes that implement some of the most prominent data compression algorithms. The QCompressionLzw class ([.h](Qcore/Storage/Compression/QCompressionLzw.h), [.cpp](Qcore/Storage/Compression/QCompressionLzw.cpp), [.inl](Qcore/Storage/Compression/QCompressionLzw.inl), [.mcr](Qcore/Storage/Compression/QCompressionLzw.mcr)) takes on high-performance compression and decompression using the Lempel-Ziv-Welch algorithm (LZW) with variable code lengths, which had its patent expired right about the time when QCompressionLzw was written, and the class supports 3 different LZW encoding types (general-purpose, GIF-like, and TIFF-like) and supports decompressing data in real-time from a dynamically growing stream of data by suspending and then resuming the state of the decompression process. The wrapper over zlib library, which is the QCompressionZLib class ([.h](Qcore/Storage/Compression/QCompressionZLib.h), [.cpp](Qcore/Storage/Compression/QCompressionZLib.cpp), [.inl](Qcore/Storage/Compression/QCompressionZLib.inl), [.mcr](Qcore/Storage/Compression/QCompressionZLib.mcr)), is able to suspend and resume decompression as well when working from a dynamic data stream. The QCompressionDxt class ([.h](Qcore/Storage/Compression/QCompressionDxt.h), [.cpp](Qcore/Storage/Compression/QCompressionDxt.cpp), [.inl](Qcore/Storage/Compression/QCompressionDxt.inl), [.mcr](Qcore/Storage/Compression/QCompressionDxt.mcr)) implements the DXT image compression, also known as S3TC, that is used by DDS textures in DirectX and lets images to be decompressed extremely fast with simple hardware implementations while preserving color information fairly well for compressed images; the DXT compression, however, is not particularly simple and requires performing principal component analysis on the source image, which is implemented in the QPrincipalComponentsAnalysis class ([.h](Qcore/Statistics/QPrincipalComponentsAnalysis.h), [.cpp](Qcore/Statistics/QPrincipalComponentsAnalysis.cpp)). Other implemented algorithms are the Apple's PackBits algorithm ([.h](Qcore/Storage/Compression/QCompressionPackBits.h), [.cpp](Qcore/Storage/Compression/QCompressionPackBits.cpp)) that is still used by some image formats and the CCITT's FAX algorithm ([.h](Qcore/Storage/Compression/QCompressionFax.h), [.cpp](Qcore/Storage/Compression/QCompressionFax.cpp), [.inl](Qcore/Storage/Compression/QCompressionFax.inl), [.mcr](Qcore/Storage/Compression/QCompressionFax.mcr)) that works best with monochrome images.

In [Graphics/Color/](Qcore/Graphics/Color), there are located classes that represent colors from several color models (called "modes" in Photoshop), namely RGB, RGBA, CMYK, CMYK-with-alpha, L*a*b*, L*a*b*-with-alpha, XYZ, and YCC, as well as the QColorModelConversion class ([.h](Qcore/Graphics/Color/QColorModelConversion.h), [.cpp](Qcore/Graphics/Color/QColorModelConversion.cpp)) that lets you convert colors between different color models.

A number of classes that are closely related to raster images and raster graphics have found their home in [Graphics/RasterImagic/](Qcore/Graphics/RasterImagic). Here you can see the QRasterImage class ([.h](Qcore/Graphics/RasterImagic/QRasterImage.h), [.cpp](Qcore/Graphics/RasterImagic/QRasterImage.cpp), [.inl](Qcore/Graphics/RasterImagic/QRasterImage.inl)) that represents raster images and provides support for 10 color models, 33 different image types, and high-dynamic-range imaging (HDR images). There is also the QRasterImageConversion class ([.h](Qcore/Graphics/RasterImagic/QRasterImageConversion.h), [.cpp](Qcore/Graphics/RasterImagic/QRasterImageConversion.cpp), [.inl](Qcore/Graphics/RasterImagic/QRasterImageConversion.inl)) that can convert a raster image from one image type into another, effectively converting between different color models and color precisions and using a dedicated class, QRasterImageQuantization ([.h](Qcore/Graphics/RasterImagic/QRasterImageQuantization.h), [.cpp](Qcore/Graphics/RasterImagic/QRasterImageQuantization.cpp), [.inl](Qcore/Graphics/RasterImagic/QRasterImageQuantization.inl)), for generating the most optimal palette based on the source image when converting a true-color image into an indexed one. Another class in this group, QRasterImageAdjustment ([.h](Qcore/Graphics/RasterImagic/QRasterImageAdjustment.h), [.cpp](Qcore/Graphics/RasterImagic/QRasterImageAdjustment.cpp)), lets you adjust the brightness, contrast, hue, saturation, lightness, and gamma of an image, blur and sharpen images, as well as apply your own filters to images. The raster image class suite wouldn't be complete without a class for image transformations, which is QRasterImageTransformation ([.h](Qcore/Graphics/RasterImagic/QRasterImageTransformation.h), [.cpp](Qcore/Graphics/RasterImagic/QRasterImageTransformation.cpp), [.inl](Qcore/Graphics/RasterImagic/QRasterImageTransformation.inl), [.mcr](Qcore/Graphics/RasterImagic/QRasterImageTransformation.mcr)) and that is able to resize any of the 33 existing types of raster images, rotate images by arbitrary angles, and skew images, all with any of the 7 supported interpolations, such as bilinear, bicubic, nearest-neighbor, and others. Finally, there is the QDiscretePlotting class ([.h](Qcore/Graphics/RasterImagic/QDiscretePlotting.h), [.inl](Qcore/Graphics/RasterImagic/QDiscretePlotting.inl), [.mcr](Qcore/Graphics/RasterImagic/QDiscretePlotting.mcr)) that, as a part of the 2D capabilities of Qcore, implements the fastest algorithms for plotting lines, circles, ellipses, quadratic Bezier curves, and cubic Bezier curves in a raster image.

Qcore happens to be especially fond of image formats, which are implemented with the classes in [Storage/Format/](Qcore/Storage/Format). Some of the nearly 20 implemented formats are PNG, JPEG, TIFF, GIF, PSD, BMP, TGA, and EXR, plus an implementation of the format used by EXIF image metadata. With the QFormatPng class ([.h](Qcore/Storage/Format/QFormatPng.h), [.cpp](Qcore/Storage/Format/QFormatPng.cpp), [.inl](Qcore/Storage/Format/QFormatPng.inl), [.mcr](Qcore/Storage/Format/QFormatPng.mcr)), the lossless PNG format is implemented to almost every smallest detail and the class lets PNG images be loaded and saved with the support for 6 color models, bits-per-pixel ranging from 1 to 64, compression strategies optimized for either size or speed, image interlacing for progressive display, various PNG metadata, as well as loading an image from a dynamic data stream when only a part of the PNG data is available; in the tests that were run with the QFormatPng class at the time, the implementation was showing unmatched performance and was even compressing images into files smaller than the respective PNG files produced by the highly acclaimed [pngcrush](http://en.wikipedia.org/wiki/Pngcrush) utility. Another image format implementation that stands out is the fully-featured implementation of the JPEG format in the QFormatJpeg class ([.h](Qcore/Storage/Format/QFormatJpeg.h), [.cpp](Qcore/Storage/Format/QFormatJpeg.cpp), [.inl](Qcore/Storage/Format/QFormatJpeg.inl)) that is relying on the highly-optimized, integer-arithmetics-only QDiscreteCosineTransformi class ([.h](Qcore/Mathematics/QDiscreteCosineTransformi.h), [.cpp](Qcore/Mathematics/QDiscreteCosineTransformi.cpp), [.inl](Qcore/Mathematics/QDiscreteCosineTransformi.inl)) for discrete cosine transforms, which are at the heart of the JPEG format, and is carrying out JPEG image loading and saving with a great emphasis made on performance and output image size and with the support for CMYK, grayscale, YCC, and YCCK color models in addition to just RGB, adjustable image quality for image compression, 2 chroma downsampling schemes, progressive data encoding and decoding, integrated EXIF metadata, and progressive image loading from a dynamic JPEG data stream. The QFormatTiff class ([.h](Qcore/Storage/Format/QFormatTiff.h), [.cpp](Qcore/Storage/Format/QFormatTiff.cpp), [.inl](Qcore/Storage/Format/QFormatTiff.inl), [.mcr](Qcore/Storage/Format/QFormatTiff.mcr)) embodies one of the most comprehensive implementations of the TIFF format, with the support for 10 color models, 22 image types, arbitrary color precision, color components that are premultiplied with alpha, 7 compression methods, internal file directories, TIFF tags, different endianness, 8 possible image orientations, strip- and tile-oriented segment organization, images with color components organized into planes, visibility masks, multiline text in TIFF metadata, as well as integrated EXIF metadata. Creating animated GIF images is made simple thanks to the QFormatGif class ([.h](Qcore/Storage/Format/QFormatGif.h), [.cpp](Qcore/Storage/Format/QFormatGif.cpp), [.inl](Qcore/Storage/Format/QFormatGif.inl), [.mcr](Qcore/Storage/Format/QFormatGif.mcr)), which is able to load still images and animated images into separate frames and save stills and animations into smallest files possible. While not being strictly an image format, the PSD format, which is used by Adobe Photoshop documents, has found its implementation in the QFormatPsd class ([.h](Qcore/Storage/Format/QFormatPsd.h), [.cpp](Qcore/Storage/Format/QFormatPsd.cpp), [.inl](Qcore/Storage/Format/QFormatPsd.inl), [.mcr](Qcore/Storage/Format/QFormatPsd.mcr)) and offers support for 5 color models together with their alpha-enabled counterparts, 16 bits per color component besides the regular 8 bits, composite images, all kinds of layers, RLE compression ("PackBits"), and 23 blending modes. The aforementioned EXIF metadata format is implemented by the QFormatExif class ([.h](Qcore/Storage/Format/QFormatExif.h), [.cpp](Qcore/Storage/Format/QFormatExif.cpp), [.inl](Qcore/Storage/Format/QFormatExif.inl), [.mcr](Qcore/Storage/Format/QFormatExif.mcr)), which provides support for myriads of various EXIF tags relating to image data structure, image data characteristics, software versions, image configuration, user information, file information, date and time, picture-taking conditions, and more.

The puzzle of a hardware-based 3D engine is put together in [Graphics/HardwareRendering/Rendering3d/](Qcore/Graphics/HardwareRendering/Rendering3d). The engine was designed to be driver-abstract. The OpenGL part of the engine was implemented first in the QOpenGLRendering class ([.h](Qcore/Graphics/HardwareRendering/Rendering3d/OpenGL/QOpenGLRendering.h), [.cpp](Qcore/Graphics/HardwareRendering/Rendering3d/OpenGL/QOpenGLRendering.cpp), [.inl](Qcore/Graphics/HardwareRendering/Rendering3d/OpenGL/QOpenGLRendering.inl)) and turned out to be good enough to stay without any DirectX accompaniment with the purpose of providing a minimalistic 3D engine.
