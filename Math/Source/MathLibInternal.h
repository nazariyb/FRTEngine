#pragma once

#ifdef MATHLIB_EXPORTS
#define MATHLIB_API __declspec(dllexport)
#else
#define MATHLIB_API __declspec(dllimport)
#endif

//#define MATHLIB_API

#ifndef PROJECT_NAMESPACE
#define PROJECT_NAMESPACE frt
#endif

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#ifndef MATHLIB_CONSTANTS
#define MATHLIB_CONSTANTS
//namespace PROJECT_NAMESPACE
//{
//namespace Constants
//{
//float PI = DirectX::XM_PI;
//float TWO_PI = DirectX::XM_2PI;
//
//float PI_DIV_2 = DirectX::XM_PIDIV2;
//float PI_DIV_4 = DirectX::XM_PIDIV4;
//
//float ONE_DIV_PI = DirectX::XM_1DIVPI;
//float ONE_DIV_2_PI = DirectX::XM_1DIV2PI;
//}
//}
#endif

#define DECLARE_BASIC_VECTOR_FRIENDS(VectorType)\
\
template<typename ScalarType>\
class VectorType;\
\
template<typename ScalarType>\
VectorType<ScalarType> operator+(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator+(ScalarType Scalar, const VectorType<ScalarType>& Vector);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator+(const VectorType<ScalarType>& Vector, ScalarType Scalar);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator-(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator-(const VectorType<ScalarType>& Vector, ScalarType Scalar);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator*(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator*(ScalarType Scalar, const VectorType<ScalarType>& Vector);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator*(const VectorType<ScalarType>& Vector, ScalarType Scalar);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator==(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator==(ScalarType Scalar, const VectorType<ScalarType>& Vector);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator==(const VectorType<ScalarType>& Vector, ScalarType Scalar);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator!=(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator!=(ScalarType Scalar, const VectorType<ScalarType>& Vector);\
\
template<typename ScalarType>\
VectorType<ScalarType> operator!=(const VectorType<ScalarType>& Vector, ScalarType Scalar);


#define DECLARE_BASIC_VECTOR_BODY(VectorType) \
public:\
\
VectorType<ScalarType> DotProduct(const VectorType<ScalarType>& Other);\
\
VectorType<ScalarType> CrossProduct(const VectorType<ScalarType>& Other);\
\
float GetMagnitude();\
\
float GetMagnitudeSquared();\
\
VectorType<ScalarType> GetNormalized();\
\
VectorType<ScalarType>& Normalize();\
\
std::string GetAsString();\
\
float GetAngleInDegreesWith(const VectorType<ScalarType>& Other);\
\
float GetAngleInRadiansWith(const VectorType<ScalarType>& Other);\
\
static VectorType<ScalarType> GetZeroVector();\
\
static VectorType<ScalarType> GetSplatOneVector();\
\
static VectorType<ScalarType> DotProduct(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
static VectorType<ScalarType> CrossProduct(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
static float GetAngleInDegreesBetween(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
static float GetAngleInRadiansBetween(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
friend VectorType<ScalarType> operator+<>(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
friend VectorType<ScalarType> operator+<>(ScalarType Scalar, const VectorType<ScalarType>& Vector);\
\
friend VectorType<ScalarType> operator+<>(const VectorType<ScalarType>& Vector, ScalarType Scalar);\
\
friend VectorType<ScalarType> operator-<>(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
friend VectorType<ScalarType> operator-<>(const VectorType<ScalarType>& Vector, ScalarType Scalar);\
\
friend VectorType<ScalarType> operator*<>(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
friend VectorType<ScalarType> operator*<>(ScalarType Scalar, const VectorType<ScalarType>& Vector);\
\
friend VectorType<ScalarType> operator*<>(const VectorType<ScalarType>& Vector, ScalarType Scalar);\
\
friend VectorType<ScalarType> operator==<>(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
friend VectorType<ScalarType> operator==<>(ScalarType Scalar, const VectorType<ScalarType>& Vector);\
\
friend VectorType<ScalarType> operator==<>(const VectorType<ScalarType>& Vector, ScalarType Scalar);\
\
friend VectorType<ScalarType> operator!=<>(const VectorType<ScalarType>& lhs, const VectorType<ScalarType>& rhs);\
\
friend VectorType<ScalarType> operator!=<>(ScalarType Scalar, const VectorType<ScalarType>& Vector);\
\
friend VectorType<ScalarType> operator!=<>(const VectorType<ScalarType>& Vector, ScalarType Scalar);\
\
private:


#define DECLARE_DATA_TYPE_DRIVEN_VECTOR_FRIENDS(VectorType)\
\
template<typename ScalarType, typename DataType>\
class VectorType;\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator+(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator+(ScalarType Scalar, const VectorType<ScalarType, DataType>& Vector);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator+(const VectorType<ScalarType, DataType>& Vector, ScalarType Scalar);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator-(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator-(const VectorType<ScalarType, DataType>& Vector, ScalarType Scalar);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator*(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator*(ScalarType Scalar, const VectorType<ScalarType, DataType>& Vector);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator*(const VectorType<ScalarType, DataType>& Vector, ScalarType Scalar);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator==(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator==(ScalarType Scalar, const VectorType<ScalarType, DataType>& Vector);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator==(const VectorType<ScalarType, DataType>& Vector, ScalarType Scalar);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator!=(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator!=(ScalarType Scalar, const VectorType<ScalarType, DataType>& Vector);\
\
template<typename ScalarType, typename DataType>\
VectorType<ScalarType, DataType> operator!=(const VectorType<ScalarType, DataType>& Vector, ScalarType Scalar);


#define DECLARE_DATA_TYPE_DRIVEN_VECTOR_BODY(VectorType) \
public:\
\
VectorType<ScalarType, DataType> DotProduct(const VectorType<ScalarType, DataType>& Other);\
\
VectorType<ScalarType, DataType> CrossProduct(const VectorType<ScalarType, DataType>& Other);\
\
float GetMagnitude();\
\
float GetMagnitudeSquared();\
\
VectorType<ScalarType, DataType> GetNormalized();\
\
VectorType<ScalarType, DataType>& Normalize();\
\
std::string GetAsString();\
\
float GetAngleInDegreesWith(const VectorType<ScalarType, DataType>& Other);\
\
float GetAngleInRadiansWith(const VectorType<ScalarType, DataType>& Other);\
\
static VectorType<ScalarType, DataType> GetZeroVector();\
\
static VectorType<ScalarType, DataType> GetSplatOneVector();\
\
static VectorType<ScalarType, DataType> DotProduct(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
static VectorType<ScalarType, DataType> CrossProduct(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
static float GetAngleInDegreesBetween(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
static float GetAngleInRadiansBetween(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
friend VectorType<ScalarType, DataType> operator+<>(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
friend VectorType<ScalarType, DataType> operator+<>(ScalarType Scalar, const VectorType<ScalarType, DataType>& Vector);\
\
friend VectorType<ScalarType, DataType> operator+<>(const VectorType<ScalarType, DataType>& Vector, ScalarType Scalar);\
\
friend VectorType<ScalarType, DataType> operator-<>(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
friend VectorType<ScalarType, DataType> operator-<>(const VectorType<ScalarType, DataType>& Vector, ScalarType Scalar);\
\
friend VectorType<ScalarType, DataType> operator*<>(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
friend VectorType<ScalarType, DataType> operator*<>(ScalarType Scalar, const VectorType<ScalarType, DataType>& Vector);\
\
friend VectorType<ScalarType, DataType> operator*<>(const VectorType<ScalarType, DataType>& Vector, ScalarType Scalar);\
\
friend VectorType<ScalarType, DataType> operator==<>(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
friend VectorType<ScalarType, DataType> operator==<>(ScalarType Scalar, const VectorType<ScalarType, DataType>& Vector);\
\
friend VectorType<ScalarType, DataType> operator==<>(const VectorType<ScalarType, DataType>& Vector, ScalarType Scalar);\
\
friend VectorType<ScalarType, DataType> operator!=<>(const VectorType<ScalarType, DataType>& lhs, const VectorType<ScalarType, DataType>& rhs);\
\
friend VectorType<ScalarType, DataType> operator!=<>(ScalarType Scalar, const VectorType<ScalarType, DataType>& Vector);\
\
friend VectorType<ScalarType, DataType> operator!=<>(const VectorType<ScalarType, DataType>& Vector, ScalarType Scalar);\
\
private:

