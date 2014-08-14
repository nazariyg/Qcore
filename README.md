# Qcore

Qcore implements smart pointers, type introspection, efficient arrays and maps, fast sorting, about 20 image formats from scratch, image processing, various algorithms for data compression, some pieces from computational geometry, and a lightweight 3D engine.

In [Graphics/Color/](Qcore/Graphics/Color), you can see classes that are representing colors from several color models, namely RGB, RGBA, CMYK, CMYK-with-alpha, L*a*b*, L*a*b*-with-alpha, XYZ, and YCC, the class that represents colors in color palettes, as well as the ColorModelConversion class ([.h](Qcore/Graphics/Color/QColorModelConversion.h), [.cpp](Qcore/Graphics/Color/QColorModelConversion.cpp)) that lets you convert colors between different color models.

The puzzle of a hardware-based 3D engine is put together in [Graphics/HardwareRendering/Rendering3d/](Qcore/Graphics/HardwareRendering/Rendering3d). The engine was designed to be driver-abstract and allow for switching between OpenGL and DirectX by simply selecting the respective class to be used for 3D rendering. The OpenGL abstraction layer was implemented first ([.h](Qcore/Graphics/HardwareRendering/Rendering3d/OpenGL/QOpenGLRendering.h), [.cpp](Qcore/Graphics/HardwareRendering/Rendering3d/OpenGL/QOpenGLRendering.cpp), [.inl](Qcore/Graphics/HardwareRendering/Rendering3d/OpenGL/QOpenGLRendering.inl)) and turned out to be good enough to remain in perfect solitude.

A number of classes closely related to raster graphics and raster images have found their home in [Graphics/RasterImagic/](Qcore/Graphics/RasterImagic). Here you can find the QRasterImage class ([.h](Qcore/Graphics/RasterImagic/QRasterImage.h), [.cpp](Qcore/Graphics/RasterImagic/QRasterImage.cpp), [.inl](Qcore/Graphics/RasterImagic/QRasterImage.inl)) that represents raster images and provides support for 10 color models (called "modes" in Photoshop), 33 different image types, and high-dynamic-range imaging (HDR). There is also the QRasterImageConversion class ([.h](Qcore/Graphics/RasterImagic/QRasterImageConversion.h), [.cpp](Qcore/Graphics/RasterImagic/QRasterImageConversion.cpp), [.inl](Qcore/Graphics/RasterImagic/QRasterImageConversion.inl)) that can convert a raster image from one image type into another, effectively converting between different color models and color precisions, and using a dedicated class, which is QRasterImageQuantization ([.h](Qcore/Graphics/RasterImagic/QRasterImageQuantization.h), [.cpp](Qcore/Graphics/RasterImagic/QRasterImageQuantization.cpp), [.inl](Qcore/Graphics/RasterImagic/QRasterImageQuantization.inl)), for finding the most optimal palette based when transforming a true-color image into an indexed image with minimum loss of information possible. Another class in this group, QRasterImageAdjustment ([.h](Qcore/Graphics/RasterImagic/QRasterImageAdjustment.h), [.cpp](Qcore/Graphics/RasterImagic/QRasterImageAdjustment.cpp)), lets you adjust brightness, contrast, hue, saturation, lightness, and gamma of an image, blur and sharpen images, as well as apply your own filters to images. The raster image class suite wouldn't be complete without a class for image transformations, which is QRasterImageTransformation ([.h](Qcore/Graphics/RasterImagic/QRasterImageTransformation.h), [.cpp](Qcore/Graphics/RasterImagic/QRasterImageTransformation.cpp), [.inl](Qcore/Graphics/RasterImagic/QRasterImageTransformation.inl), [.mcr](Qcore/Graphics/RasterImagic/QRasterImageTransformation.mcr)) and is able to resize any of the 33 supported types of raster images, rotate image by an arbitrary angle, and skew images, all with any of the 7 supported interpolations, such as bilinear, bicubic, nearest-neighbor, and others. Finally, there is the QDiscretePlotting class ([.h](Qcore/Graphics/RasterImagic/QDiscretePlotting.h), [.cpp](Qcore/Graphics/RasterImagic/QDiscretePlotting.cpp), [.inl](Qcore/Graphics/RasterImagic/QDiscretePlotting.inl), [.mcr](Qcore/Graphics/RasterImagic/QDiscretePlotting.mcr)) that, as a part of the 2D capabilities of Qcore, implements the fastest algorithms for plotting lines, circles, ellipses, quadratic Bezier curves, and cubic Bezier curves in raster images.

In [Storage/Compression/](Qcore/Storage/Compression) you can find the classes that efficiently implement some of the most prominent data compression algorithms. The QCompressionLzw class ([.h](Qcore/Storage/Compression/QCompressionLzw.h), [.cpp](Qcore/Storage/Compression/QCompressionLzw.cpp), [.inl](Qcore/Storage/Compression/QCompressionLzw.inl), [.mcr](Qcore/Storage/Compression/QCompressionLzw.mcr)) takes on high-performance compression and decompression using the variable-length-code Lempel-Ziv-Welch algorithm (LZW), which had its patent expired right about the time when QCompressionLzw was written, and the class supports 3 different LZW encoding types (general-purpose, GIF-like, and TIFF-like) as well as decompressing data in real-time from a dynamically growing stream of bytes by suspending and then resuming the decoding process. The wrapper over the zlib library, which is the QCompressionZLib class ([.h](Qcore/Storage/Compression/QCompressionZLib.h), [.cpp](Qcore/Storage/Compression/QCompressionZLib.cpp), [.inl](Qcore/Storage/Compression/QCompressionZLib.inl), [.mcr](Qcore/Storage/Compression/QCompressionZLib.mcr)), is able to suspend and resume decoding as well when decompressing from a dynamic data stream. The QCompressionDxt class ([.h](Qcore/Storage/Compression/QCompressionDxt.h), [.cpp](Qcore/Storage/Compression/QCompressionDxt.cpp), [.inl](Qcore/Storage/Compression/QCompressionDxt.inl), [.mcr](Qcore/Storage/Compression/QCompressionDxt.mcr)) implements the DXT image compression, also known as S3TC, that is used by DDS textures in DirectX and that lets images to be decompressed extremely fast by simple hardware implementations while preserving color information fairly well; DXT compression, however, in not particularly simple and requires performing principal component analysis of the source image, which is implemented in the QPrincipalComponentsAnalysis class ([.h](Qcore/Statistics/QPrincipalComponentsAnalysis.h), [.cpp](Qcore/Statistics/QPrincipalComponentsAnalysis.cpp)). Other implemented algorithms are the Apple's PackBits algorithm ([.h](Qcore/Statistics/QCompressionPackBits.h), [.cpp](Qcore/Statistics/QCompressionPackBits.cpp)) that is still used by some image formats and the FAX algorithm ([.h](Qcore/Storage/Compression/QCompressionFax.h), [.cpp](Qcore/Storage/Compression/QCompressionFax.cpp), [.inl](Qcore/Storage/Compression/QCompressionFax.inl), [.mcr](Qcore/Storage/Compression/QCompressionFax.mcr)) that works best with monochrome images.
