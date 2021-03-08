#include "Vector/Vector2Int.h"
#include "Vector/Vector3Int.h"

#include <cmath>

namespace PROJECT_NAMESPACE
{

Vector2Int::Vector2Int()
    : Vector2Int(0)
{}

Vector2Int::Vector2Int(const Vector2Int& Other)
{
    _data = Other._data;
}

Vector2Int::Vector2Int(const Vector3Int& Other)
    : Vector2Int(Other.GetX(), Other.GetY())
{}


Vector2Int& Vector2Int::operator=(const Vector2Int& Other)
{
    _data = Other._data;
    return *this;
}

Vector2Int::Vector2Int(const int Value)
{
    _data = DirectX::XMVectorReplicateInt(Value);
}

Vector2Int::Vector2Int(const int X, const int Y)
{
    _data = DirectX::XMVectorSetInt(X, Y, 0, 0);
}

Vector2Int::Vector2Int(const int DataSource[])
    : Vector2Int(DataSource[0], DataSource[1])
{}

Vector2Int::Vector2Int(const std::pair<int, int>& DataSource)
    : Vector2Int(DataSource.first, DataSource.second)
{}

Vector2Int::Vector2Int(const std::tuple<int, int>& DataSource)
    : Vector2Int(std::get<0>(DataSource), std::get<1>(DataSource))
{}

Vector2Int::Vector2Int(const std::vector<int>& DataSource)
{
    // TODO: assert size of DataSource
    _data = DirectX::XMVectorSetInt(DataSource[0], DataSource[1], 0, 0);
}

Vector2Int::Vector2Int(const DirectX::XMVECTOR& DataSource)
{
    _data = DataSource;
}

Vector2Int::Vector2Int(const DirectX::XMINT2& DataSource)
    : Vector2Int(DataSource.x, DataSource.y)
{}

Vector2Int::~Vector2Int()
{}

inline int Vector2Int::GetX() const
{
    return DirectX::XMVectorGetIntX(_data);
}

inline int Vector2Int::GetY() const
{
    return DirectX::XMVectorGetIntY(_data);
}

inline void Vector2Int::SetX(int Value)
{
    _data = DirectX::XMVectorSetIntX(_data, Value);
}

inline void Vector2Int::SetY(int Value)
{
    _data = DirectX::XMVectorSetIntY(_data, Value);
}

inline int Vector2Int::DotProduct(const Vector2Int& Other) const
{
    return GetX() * Other.GetX() + GetY() * Other.GetY();
}

inline Vector2Int Vector2Int::GetPseudoCrossProduct() const
{
    return { -GetY(), GetX() };
}

inline float Vector2Int::GetAngleInDegreesWith(const Vector2Int& Other) const
{
    return DirectX::XMConvertToDegrees(GetAngleInRadiansWith(Other));
}

inline float Vector2Int::GetAngleInRadiansWith(const Vector2Int& Other) const
{
    // TODO: make it work with int
    return GetSingleValueFromReplicatedVector(DirectX::XMVector2AngleBetweenVectors(_data, Other._data));
}

inline float Vector2Int::GetMagnitude() const
{
    return std::sqrt(*this * *this);
}

inline float Vector2Int::GetMagnitudeSquared() const
{
    return *this * *this;
}

inline float Vector2Int::GetMagnitudeEstimated() const
{
    // TODO: make it fitable with int
    return GetSingleValueFromReplicatedVector(DirectX::XMVector2LengthEst(_data));
}

inline Vector2Int Vector2Int::GetNormalized() const
{
    // TODO: must return float vector
    return { DirectX::XMVector2Normalize(_data) };
}

inline std::string Vector2Int::GetAsString() const
{
    return "<" + std::to_string(GetX()) + ", "
               + std::to_string(GetY()) + ">";
}

inline int Vector2Int::DotProduct(const Vector2Int& lhs, const Vector2Int& rhs)
{
    return lhs.DotProduct(rhs);
}

//inline Vector2Int Vector2Int::GetPseudoCrossProduct(const Vector2Int& lhs, const Vector2Int& rhs)
//{
//    return lhs.GetPseudoCrossProduct(rhs);
//}

inline float Vector2Int::GetAngleInDegreesBetween(const Vector2Int& lhs, const Vector2Int& rhs)
{
    return lhs.GetAngleInDegreesWith(rhs);
}

inline float Vector2Int::GetAngleInRadiansBetween(const Vector2Int& lhs, const Vector2Int& rhs)
{
    return lhs.GetAngleInRadiansWith(rhs);
}

inline Vector2Int Vector2Int::GetZeroVector()
{
    return { DirectX::XMVectorZero() };
}

inline Vector2Int Vector2Int::GetSplatOneVector()
{
    return { DirectX::XMVectorSplatOne() };
}

inline float Vector2Int::GetSingleValueFromReplicatedVector(const DirectX::XMVECTOR& Vector) const
{
    return DirectX::XMVectorGetX(Vector);
}

#pragma region Operators

inline Vector2Int operator+(const Vector2Int& lhs, const Vector2Int& rhs)
{
    return { (int)(lhs._data.m128_i32[0] + rhs._data.m128_i32[0]),
             (int)(lhs._data.m128_i32[1] + rhs._data.m128_i32[1]) };
}

inline Vector2Int operator+(int Scalar, const Vector2Int& Vector)
{
    return { DirectX::XMVectorReplicateInt(Scalar) + Vector };
}

inline Vector2Int operator+(const Vector2Int& Vector, int Scalar)
{
    return Scalar + Vector;
}

inline Vector2Int operator-(const Vector2Int& lhs, const Vector2Int& rhs)
{
    return { (int)(lhs._data.m128_i32[0] - rhs._data.m128_i32[0]),
             (int)(lhs._data.m128_i32[1] - rhs._data.m128_i32[1]) };
}

inline Vector2Int operator-(const Vector2Int& Vector, int Scalar)
{
    return { Vector - DirectX::XMVectorReplicateInt(Scalar) };
}

inline int operator*(const Vector2Int& lhs, const Vector2Int& rhs)
{
    return lhs.DotProduct(rhs);
}

inline Vector2Int operator*(int Scalar, const Vector2Int& Vector)
{
    return { (int)Vector._data.m128_i32[0] * Scalar,
             (int)Vector._data.m128_i32[1] * Scalar };
}

inline Vector2Int operator*(const Vector2Int& Vector, int Scalar)
{
    return Scalar * Vector;
}

inline bool operator==(const Vector2Int& lhs, const Vector2Int& rhs)
{
    return DirectX::XMVectorGetIntX(lhs._data) == DirectX::XMVectorGetIntX(rhs._data)
        && DirectX::XMVectorGetIntY(lhs._data) == DirectX::XMVectorGetIntY(rhs._data);
}

inline bool operator==(int Scalar, const Vector2Int& Vector)
{
    return Vector2Int(DirectX::XMVectorReplicateInt(Scalar)) == Vector;
}

inline bool operator==(const Vector2Int& Vector, int Scalar)
{
    return Scalar == Vector;
}

inline bool operator!=(const Vector2Int& lhs, const Vector2Int& rhs)
{
    return !(lhs == rhs);
}

inline bool operator!=(int Scalar, const Vector2Int& Vector)
{
    return !(Scalar == Vector);
}

inline bool operator!=(const Vector2Int& Vector, int Scalar)
{
    return !(Vector == Scalar);
}

#pragma endregion

}