#include "Vector3Int.h"

namespace PROJECT_NAMESPACE
{

Vector3Int::Vector3Int()
    : Vector3Int(0)
{}

Vector3Int::Vector3Int(const Vector2Int& XYVector, int Z /*= 0*/)
{
    _data = XYVector._data;
    SetZ(Z);
}

Vector3Int::Vector3Int(int X, const Vector2Int& YZVector)
{
    _data = DirectX::XMVectorSetInt(X, YZVector.GetX(), YZVector.GetY(), 0);
}

Vector3Int::Vector3Int(const Vector3Int& Other)
{
    _data = Other._data;
}

Vector3Int& Vector3Int::operator=(const Vector3Int Other)
{
    _data = Other._data;
    return *this;
}


Vector3Int::Vector3Int(int Value)
    : Vector2Int(Value)
{}

Vector3Int::Vector3Int(int X, int Y, int Z/*=0*/)
{
    _data = DirectX::XMVectorSetInt(X, Y, Z, 0);
}

Vector3Int::Vector3Int(const int DataSource[])
    : Vector3Int(DataSource[0], DataSource[1], DataSource[2])
{}

Vector3Int::Vector3Int(const std::pair<int, int>& XYDataSource, int Z /*= 0*/)
    : Vector3Int(XYDataSource.first, XYDataSource.second, Z)
{}

Vector3Int::Vector3Int(int X, const std::pair<int, int>& YZDataSource)
    : Vector3Int(X, YZDataSource.first, YZDataSource.second)
{}

Vector3Int::Vector3Int(const std::tuple<int, int>& XYDataSource, int Z /*= 0*/)
    : Vector3Int(std::get<0>(XYDataSource), std::get<1>(XYDataSource), Z)
{}

Vector3Int::Vector3Int(int X, const std::tuple<int, int>& YZDataSource)
    : Vector3Int(X, std::get<0>(YZDataSource), std::get<1>(YZDataSource))
{}

Vector3Int::Vector3Int(const std::tuple<int, int, int>& DataSource)
    : Vector3Int(std::get<0>(DataSource), std::get<1>(DataSource), std::get<2>(DataSource))
{}

Vector3Int::Vector3Int(const std::vector<int>& DataSource)
{
    // TODO: assert size of DataSource
    _data = DirectX::XMVectorSetInt(DataSource[0], DataSource[1], DataSource[2], 0);
}

Vector3Int::Vector3Int(const DirectX::XMVECTOR& DataSource)
{
    _data = DataSource;
}

Vector3Int::Vector3Int(const DirectX::XMINT3& DataSource)
    : Vector3Int(DataSource.x, DataSource.y, DataSource.z)
{}

Vector3Int::~Vector3Int()
{}

inline int Vector3Int::GetZ() const
{
    return DirectX::XMVectorGetIntZ(_data);
}

inline void Vector3Int::SetZ(int Value)
{
    _data = DirectX::XMVectorSetIntZ(_data, Value);
}

inline int Vector3Int::DotProduct(const Vector3Int& Other) const
{
    return GetX() * Other.GetX() + GetY() * Other.GetY() + GetZ() * Other.GetZ();
}

inline Vector3Int Vector3Int::CrossProduct(const Vector3Int& Other) const
{
    return {
            GetY() * Other.GetZ() - GetZ() * Other.GetY(),
            GetZ() * Other.GetX() - GetX() * Other.GetZ(),
            GetX() * Other.GetY() - GetY() * Other.GetX()
    };
}

//inline float Vector3Int::GetMagnitudeSquared() const
//{
//
//}
//
//inline float Vector3Int::GetMagnitudeEstimated() const
//{
//
//}

inline Vector3Int Vector3Int::GetNormalized() const
{
    // TODO: must return float vector
    return { DirectX::XMVector3Normalize(_data) };
}

inline std::string Vector3Int::GetAsString() const
{
    return "<" 
        + std::to_string(GetX()) + ", "
        + std::to_string(GetY()) + ", "
        + std::to_string(GetZ()) + ">";
}

inline float Vector3Int::GetAngleInDegreesWith(const Vector3Int& Other) const
{
    return DirectX::XMConvertToDegrees(GetAngleInRadiansWith(Other));
}

inline float Vector3Int::GetAngleInRadiansWith(const Vector3Int& Other) const
{
    // TODO: make it work with int
    return GetSingleValueFromReplicatedVector(DirectX::XMVector3AngleBetweenVectors(_data, Other._data));
}

inline int Vector3Int::DotProduct(const Vector3Int& lhs, const Vector3Int& rhs)
{
    return lhs.DotProduct(rhs);
}

inline Vector3Int Vector3Int::CrossProduct(const Vector3Int& lhs, const Vector3Int& rhs)
{
    return lhs.CrossProduct(rhs);
}

inline float Vector3Int::GetAngleInDegreesBetween(const Vector3Int& lhs, const Vector3Int& rhs)
{
    return lhs.GetAngleInDegreesWith(rhs);
}

inline float Vector3Int::GetAngleInRadiansBetween(const Vector3Int& lhs, const Vector3Int& rhs)
{
    return lhs.GetAngleInRadiansWith(rhs);
}

inline Vector3Int Vector3Int::GetZeroVector()
{
    return { DirectX::XMVectorZero() };
}

inline Vector3Int Vector3Int::GetSplatOneVector()
{
    return { DirectX::XMVectorSplatOne() };
}


#pragma region Operators

inline Vector3Int operator+(const Vector3Int& lhs, const Vector3Int& rhs)
{
    return {
        (int)(lhs._data.m128_i32[0] + rhs._data.m128_i32[0]),
        (int)(lhs._data.m128_i32[1] + rhs._data.m128_i32[1]),
        (int)(lhs._data.m128_i32[2] + rhs._data.m128_i32[2])
    };
}

inline Vector3Int operator+(int Scalar, const Vector3Int& Vector)
{
    return { DirectX::XMVectorReplicateInt(Scalar) + Vector };
}

inline Vector3Int operator+(const Vector3Int& Vector, int Scalar)
{
    return Scalar + Vector;
}

inline Vector3Int operator-(const Vector3Int& lhs, const Vector3Int& rhs)
{
    return {
        (int)(lhs._data.m128_i32[0] - rhs._data.m128_i32[0]),
        (int)(lhs._data.m128_i32[1] - rhs._data.m128_i32[1]),
        (int)(lhs._data.m128_i32[2] - rhs._data.m128_i32[2])
    };
}

inline Vector3Int operator-(const Vector3Int& Vector, int Scalar)
{
    return { Vector - DirectX::XMVectorReplicateInt(Scalar) };
}

inline int operator*(const Vector3Int& lhs, const Vector3Int& rhs)
{
    return lhs.DotProduct(rhs);
}

inline Vector3Int operator*(int Scalar, const Vector3Int& Vector)
{
    return {
        (int)Vector._data.m128_i32[0] * Scalar,
        (int)Vector._data.m128_i32[1] * Scalar,
        (int)Vector._data.m128_i32[2] * Scalar,
    };
}

inline Vector3Int operator*(const Vector3Int& Vector, int Scalar)
{
    return Scalar * Vector;
}

inline bool operator==(const Vector3Int& lhs, const Vector3Int& rhs)
{
    return DirectX::XMVectorGetIntX(lhs._data) == DirectX::XMVectorGetIntX(rhs._data)
        && DirectX::XMVectorGetIntY(lhs._data) == DirectX::XMVectorGetIntY(rhs._data)
        && DirectX::XMVectorGetIntZ(lhs._data) == DirectX::XMVectorGetIntZ(rhs._data);
}

inline bool operator==(int Scalar, const Vector3Int& Vector)
{
    return Vector3Int(DirectX::XMVectorReplicateInt(Scalar)) == Vector;
}

inline bool operator==(const Vector3Int& Vector, int Scalar)
{
    return Scalar == Vector;
}

inline bool operator!=(const Vector3Int& lhs, const Vector3Int& rhs)
{
    return !(lhs == rhs);
}

inline bool operator!=(int Scalar, const Vector3Int& Vector)
{
    return !(Scalar == Vector);
}

inline bool operator!=(const Vector3Int& Vector, int Scalar)
{
    return !(Vector == Scalar);
}

#pragma endregion

}
