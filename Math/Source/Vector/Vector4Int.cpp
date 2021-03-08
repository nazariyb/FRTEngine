#include "Vector4Int.h"

namespace PROJECT_NAMESPACE
{

Vector4Int::Vector4Int()
    : Vector4Int(0)
{}

Vector4Int::Vector4Int(const Vector2Int& XYVector, const Vector2Int& ZWVector)
{
    _data = XYVector._data;
    SetZ(ZWVector.GetX());
    SetW(ZWVector.GetY());
}

Vector4Int::Vector4Int(const Vector3Int& XYZVector, int W/* = 0*/)
{
    _data = XYZVector._data;
    SetW(W);
}

Vector4Int::Vector4Int(int X, const Vector3Int& YZWVector)
{
    _data = DirectX::XMVectorSetInt(X, YZWVector.GetX(), YZWVector.GetY(), YZWVector.GetZ());
}

Vector4Int::Vector4Int(const Vector4Int& Other)
{
    _data = Other._data;
}

Vector4Int& Vector4Int::operator=(const Vector4Int Other)
{
    _data = Other._data;
    return *this;
}

Vector4Int::Vector4Int(int Value)
    : Vector3Int(Value)
{}

Vector4Int::Vector4Int(int X, int Y, int Z/* = 0*/, int W/* = 0*/)
{
    _data = DirectX::XMVectorSetInt(X, Y, Z, W);
}

Vector4Int::Vector4Int(const int DataSource[])
    : Vector4Int(DataSource[0], DataSource[1], DataSource[2], DataSource[3])
{}

Vector4Int::Vector4Int(const std::pair<int, int>& XYDataSource, const std::pair<int, int>& ZWDataSource)
    : Vector4Int(XYDataSource.first, XYDataSource.second, ZWDataSource.first, ZWDataSource.second)
{}

Vector4Int::Vector4Int(const std::tuple<int, int>& XYDataSource, const std::tuple<int, int>& ZWDataSource)
    : Vector4Int(std::get<0>(XYDataSource), std::get<1>(XYDataSource), std::get<0>(ZWDataSource), std::get<1>(ZWDataSource))
{}

Vector4Int::Vector4Int(const std::vector<int>& DataSource)
{
    // TODO: assert size of DataSource
    _data = DirectX::XMVectorSetInt(DataSource[0], DataSource[1], DataSource[2], DataSource[3]);
}

Vector4Int::Vector4Int(const DirectX::XMVECTOR& DataSource)
{
    _data = DataSource;
}

Vector4Int::Vector4Int(const DirectX::XMINT4& DataSource)
    : Vector4Int(DataSource.x, DataSource.y, DataSource.z, DataSource.w)
{}

Vector4Int::~Vector4Int()
{}

inline int Vector4Int::GetW() const
{
    return DirectX::XMVectorGetIntW(_data);
}

inline void Vector4Int::SetW(int Value)
{
    _data = DirectX::XMVectorSetIntW(_data, Value);
}

inline int Vector4Int::DotProduct(const Vector4Int& Other) const
{
    return GetX() * Other.GetX() + GetY() * Other.GetY() + GetZ() * Other.GetZ() + GetW() * Other.GetW();
}

inline Vector4Int Vector4Int::CrossProduct(const Vector4Int& Other) const
{
    // TODO:
    return {
            GetY() * Other.GetZ() - GetZ() * Other.GetY(),
            GetZ() * Other.GetX() - GetX() * Other.GetZ(),
            GetX() * Other.GetY() - GetY() * Other.GetX()
    };
}

//inline float Vector4Int::GetMagnitudeSquared() const
//{
//
//}
//
//inline float Vector4Int::GetMagnitudeEstimated() const
//{
//
//}

inline Vector4Int Vector4Int::GetNormalized() const
{
    // TODO: must return float vector
    return { DirectX::XMVector4Normalize(_data) };
}

inline std::string Vector4Int::GetAsString() const
{
    return "<"
        + std::to_string(GetX()) + ", "
        + std::to_string(GetY()) + ", "
        + std::to_string(GetZ()) + ", "
        + std::to_string(GetW()) + ">";
}

inline float Vector4Int::GetAngleInDegreesWith(const Vector4Int& Other) const
{
    return DirectX::XMConvertToDegrees(GetAngleInRadiansWith(Other));
}

inline float Vector4Int::GetAngleInRadiansWith(const Vector4Int& Other) const
{
    // TODO: make it work with int
    return GetSingleValueFromReplicatedVector(DirectX::XMVector4AngleBetweenVectors(_data, Other._data));
}

inline int Vector4Int::DotProduct(const Vector4Int& lhs, const Vector4Int& rhs)
{
    return lhs.DotProduct(rhs);
}

inline Vector4Int Vector4Int::CrossProduct(const Vector4Int& lhs, const Vector4Int& rhs)
{
    return lhs.CrossProduct(rhs);
}

inline float Vector4Int::GetAngleInDegreesBetween(const Vector4Int& lhs, const Vector4Int& rhs)
{
    return lhs.GetAngleInDegreesWith(rhs);
}

inline float Vector4Int::GetAngleInRadiansBetween(const Vector4Int& lhs, const Vector4Int& rhs)
{
    return lhs.GetAngleInRadiansWith(rhs);
}

inline Vector4Int Vector4Int::GetZeroVector()
{
    return { DirectX::XMVectorZero() };
}

inline Vector4Int Vector4Int::GetSplatOneVector()
{
    return { DirectX::XMVectorSplatOne() };
}


#pragma region Operators

inline Vector4Int operator+(const Vector4Int& lhs, const Vector4Int& rhs)
{
    return {
        (int)(lhs._data.m128_i32[0] + rhs._data.m128_i32[0]),
        (int)(lhs._data.m128_i32[1] + rhs._data.m128_i32[1]),
        (int)(lhs._data.m128_i32[2] + rhs._data.m128_i32[2]),
        (int)(lhs._data.m128_i32[3] + rhs._data.m128_i32[3])
    };
}

inline Vector4Int operator+(int Scalar, const Vector4Int& Vector)
{
    return { DirectX::XMVectorReplicateInt(Scalar) + Vector };
}

inline Vector4Int operator+(const Vector4Int& Vector, int Scalar)
{
    return Scalar + Vector;
}

inline Vector4Int operator-(const Vector4Int& lhs, const Vector4Int& rhs)
{
    return {
        (int)(lhs._data.m128_i32[0] - rhs._data.m128_i32[0]),
        (int)(lhs._data.m128_i32[1] - rhs._data.m128_i32[1]),
        (int)(lhs._data.m128_i32[2] - rhs._data.m128_i32[2]),
        (int)(lhs._data.m128_i32[3] - rhs._data.m128_i32[3])
    };
}

inline Vector4Int operator-(const Vector4Int& Vector, int Scalar)
{
    return { Vector - DirectX::XMVectorReplicateInt(Scalar) };
}

inline int operator*(const Vector4Int& lhs, const Vector4Int& rhs)
{
    return lhs.DotProduct(rhs);
}

inline Vector4Int operator*(int Scalar, const Vector4Int& Vector)
{
    return {
        (int)Vector._data.m128_i32[0] * Scalar,
        (int)Vector._data.m128_i32[1] * Scalar,
        (int)Vector._data.m128_i32[2] * Scalar,
        (int)Vector._data.m128_i32[3] * Scalar
    };
}

inline Vector4Int operator*(const Vector4Int& Vector, int Scalar)
{
    return Scalar * Vector;
}

inline bool operator==(const Vector4Int& lhs, const Vector4Int& rhs)
{
    return DirectX::XMVectorGetIntX(lhs._data) == DirectX::XMVectorGetIntX(rhs._data)
        && DirectX::XMVectorGetIntY(lhs._data) == DirectX::XMVectorGetIntY(rhs._data)
        && DirectX::XMVectorGetIntZ(lhs._data) == DirectX::XMVectorGetIntZ(rhs._data)
        && DirectX::XMVectorGetIntW(lhs._data) == DirectX::XMVectorGetIntW(rhs._data);
}

inline bool operator==(int Scalar, const Vector4Int& Vector)
{
    return Vector4Int(DirectX::XMVectorReplicateInt(Scalar)) == Vector;
}

inline bool operator==(const Vector4Int& Vector, int Scalar)
{
    return Scalar == Vector;
}

inline bool operator!=(const Vector4Int& lhs, const Vector4Int& rhs)
{
    return !(lhs == rhs);
}

inline bool operator!=(int Scalar, const Vector4Int& Vector)
{
    return !(Scalar == Vector);
}

inline bool operator!=(const Vector4Int& Vector, int Scalar)
{
    return !(Vector == Scalar);
}

#pragma endregion

}
