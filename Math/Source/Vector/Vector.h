#pragma once

#include <windows.h>
#include <string>
#include <vector>

#include "MathLibInternal.h"


namespace PROJECT_NAMESPACE
{

DECLARE_BASIC_VECTOR_FRIENDS(Vector)

template<typename ScalarType=float>
class Vector
{

    DECLARE_BASIC_VECTOR_BODY(Vector)

public:
    Vector(unsigned int Dimensions=3)
        : _dimensions(Dimensions)
    {
        _data.reserve(_dimensions);
    }

    Vector(const std::vector<ScalarType>& DataSource)
        : _dimensions(DataSource.size()), _data(DataSource)
    {}

private:
    unsigned int _dimensions;
    std::vector<ScalarType> _data;
};

template<typename ScalarType>
Vector<ScalarType> Vector<ScalarType>::DotProduct(const Vector<ScalarType>& Other)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
static Vector<ScalarType> Vector<ScalarType>::DotProduct(const Vector<ScalarType>& lhs, const Vector<ScalarType>& rhs)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> Vector<ScalarType>::CrossProduct(const Vector<ScalarType>& Other)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
static Vector<ScalarType> Vector<ScalarType>::CrossProduct(const Vector<ScalarType>& lhs, const Vector<ScalarType>& rhs)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
float Vector<ScalarType>::GetMagnitude()
{
    // TODO:
    return 0.f;
}

template<typename ScalarType>
float Vector<ScalarType>::GetMagnitudeSquared()
{
    // TODO:
    return 0.f;
}

template<typename ScalarType>
Vector<ScalarType> Vector<ScalarType>::GetNormalized()
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType>& Vector<ScalarType>::Normalize()
{
    // TODO:
    return *this;
}

template<typename ScalarType>
std::string Vector<ScalarType>::GetAsString()
{
    // TODO:
    return "";
}

template<typename ScalarType>
float Vector<ScalarType>::GetAngleInDegreesWith(const Vector<ScalarType>& Other)
{
    // TODO:
    return 0.f;
}

template<typename ScalarType>
float Vector<ScalarType>::GetAngleInRadiansWith(const Vector<ScalarType>& Other)
{
    // TODO:
    return 0.f;
}

template<typename ScalarType>
Vector<ScalarType> operator+(const Vector<ScalarType>& lhs, const Vector<ScalarType>& rhs)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator+(ScalarType Scalar, const Vector<ScalarType>& Vector)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator+(const Vector<ScalarType>& Vector, ScalarType Scalar)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator-(const Vector<ScalarType>& lhs, const Vector<ScalarType>& rhs)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator-(const Vector<ScalarType>& Vector, ScalarType Scalar)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator*(const Vector<ScalarType>& lhs, const Vector<ScalarType>& rhs)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator*(ScalarType Scalar, const Vector<ScalarType>& Vector)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator*(const Vector<ScalarType>& Vector, ScalarType Scalar)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator==(const Vector<ScalarType>& lhs, const Vector<ScalarType>& rhs)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator==(ScalarType Scalar, const Vector<ScalarType>& Vector)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator==(const Vector<ScalarType>& Vector, ScalarType Scalar)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator!=(const Vector<ScalarType>& lhs, const Vector<ScalarType>& rhs)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator!=(ScalarType Scalar, const Vector<ScalarType>& Vector)
{
    // TODO:
    return *this;
}

template<typename ScalarType>
Vector<ScalarType> operator!=(const Vector<ScalarType>& Vector, ScalarType Scalar)
{
    // TODO:
    return *this;
}

} // PROJECT_NAMESPACE
