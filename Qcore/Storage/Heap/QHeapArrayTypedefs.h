#ifndef QHEAPARRAYTYPEDEFS_H
#define QHEAPARRAYTYPEDEFS_H

#include "QHeapArray.h"
#include "QSmartPointer.h"

namespace Q
{

typedef HeapArray<bool> BoolHeapArray;
typedef SmartPointer<BoolHeapArray> BoolHeapArrayPtr;

typedef HeapArray<char> CharHeapArray;
typedef SmartPointer<CharHeapArray> CharHeapArrayPtr;

typedef HeapArray<unsigned char> UCharHeapArray;
typedef SmartPointer<UCharHeapArray> UCharHeapArrayPtr;

typedef HeapArray<short> ShortHeapArray;
typedef SmartPointer<ShortHeapArray> ShortHeapArrayPtr;

typedef HeapArray<unsigned short> UShortHeapArray;
typedef SmartPointer<UShortHeapArray> UShortHeapArrayPtr;

typedef HeapArray<long> LongHeapArray;
typedef SmartPointer<LongHeapArray> LongHeapArrayPtr;

typedef HeapArray<unsigned long> ULongHeapArray;
typedef SmartPointer<ULongHeapArray> ULongHeapArrayPtr;

typedef HeapArray<int> IntHeapArray;
typedef SmartPointer<IntHeapArray> IntHeapArrayPtr;

typedef HeapArray<unsigned int> UIntHeapArray;
typedef SmartPointer<UIntHeapArray> UIntHeapArrayPtr;

typedef HeapArray<float> FloatHeapArray;
typedef SmartPointer<FloatHeapArray> FloatHeapArrayPtr;

typedef HeapArray<double> DoubleHeapArray;
typedef SmartPointer<DoubleHeapArray> DoubleHeapArrayPtr;

typedef HeapArray<Vector2f> Vector2fHeapArray;
typedef SmartPointer<Vector2fHeapArray> Vector2fHeapArrayPtr;

typedef HeapArray<Vector2d> Vector2dHeapArray;
typedef SmartPointer<Vector2dHeapArray> Vector2dHeapArrayPtr;

typedef HeapArray<Vector3f> Vector3fHeapArray;
typedef SmartPointer<Vector3fHeapArray> Vector3fHeapArrayPtr;

typedef HeapArray<Vector3d> Vector3dHeapArray;
typedef SmartPointer<Vector3dHeapArray> Vector3dHeapArrayPtr;

typedef HeapArray<Vector4f> Vector4fHeapArray;
typedef SmartPointer<Vector4fHeapArray> Vector4fHeapArrayPtr;

typedef HeapArray<Vector4d> Vector4dHeapArray;
typedef SmartPointer<Vector4dHeapArray> Vector4dHeapArrayPtr;

typedef HeapArray<Matrix2f> Matrix2fHeapArray;
typedef SmartPointer<Matrix2fHeapArray> Matrix2fHeapArrayPtr;

typedef HeapArray<Matrix2d> Matrix2dHeapArray;
typedef SmartPointer<Matrix2dHeapArray> Matrix2dHeapArrayPtr;

typedef HeapArray<Matrix3f> Matrix3fHeapArray;
typedef SmartPointer<Matrix3fHeapArray> Matrix3fHeapArrayPtr;

typedef HeapArray<Matrix3d> Matrix3dHeapArray;
typedef SmartPointer<Matrix3dHeapArray> Matrix3dHeapArrayPtr;

typedef HeapArray<Matrix4f> Matrix4fHeapArray;
typedef SmartPointer<Matrix4fHeapArray> Matrix4fHeapArrayPtr;

typedef HeapArray<Matrix4d> Matrix4dHeapArray;
typedef SmartPointer<Matrix4dHeapArray> Matrix4dHeapArrayPtr;

typedef HeapArray<ColorRgb> ColorRgbHeapArray;
typedef SmartPointer<ColorRgbHeapArray> ColorRgbHeapArrayPtr;

typedef HeapArray<ColorRgba> ColorRgbaHeapArray;
typedef SmartPointer<ColorRgbaHeapArray> ColorRgbaHeapArrayPtr;

}

#endif



