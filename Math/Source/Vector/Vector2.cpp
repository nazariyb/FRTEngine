#include "Vector/Vector2.h"
#include "Vector/Vector2Int.h"

#include <sstream>
#include <iomanip>


namespace PROJECT_NAMESPACE
{

Vector2::Vector2()
    : Vector2(0.f)
{}

Vector2::Vector2(const Vector2& Other)
{
    _data = Other._data;
}

Vector2::Vector2(const Vector2Int& Other)
{
    _data = DirectX::XMVectorSet(Other.GetX(), Other.GetY(), 0.f, 0.f);
}

Vector2::Vector2(const class Vector3& Other)
{
    // TODO:
}

Vector2& Vector2::operator=(const Vector2& Other)
{
    _data = Other._data;
    return *this;
}

Vector2::Vector2(const float Value)
{
    _data = DirectX::XMVectorReplicate(Value);
}

Vector2::Vector2(const float X, const float Y)
{
    _data = DirectX::XMVectorSet(X, Y, 0.f, 0.f);
}

Vector2::Vector2(const std::pair<float, float>& DataSource)
    : Vector2(DataSource.first, DataSource.second)
{}

Vector2::Vector2(const float DataSource[])
    : Vector2(DataSource[0], DataSource[1])
{}

Vector2::Vector2(const std::tuple<float, float>& DataSource)
    : Vector2(std::get<0>(DataSource), std::get<1>(DataSource))
{}

Vector2::Vector2(const std::vector<float>& DataSource)
{
    // TODO: assert size of DataSource
    _data = DirectX::XMVectorSet(DataSource[0], DataSource[1], 0.f, 0.f);
}

Vector2::Vector2(const DirectX::XMVECTOR& DataSource)
{
    _data = DataSource;
}

Vector2::Vector2(const DirectX::XMFLOAT2& DataSource)
    : Vector2(DataSource.x, DataSource.y)
{}

Vector2::~Vector2()
{}

float Vector2::GetX() const
{
    return DirectX::XMVectorGetX(_data);
}

float Vector2::GetY() const
{
    return DirectX::XMVectorGetY(_data);
}

void Vector2::SetX(float Value)
{
    _data = DirectX::XMVectorSetX(_data, Value);
}

void Vector2::SetY(float Value)
{
    _data = DirectX::XMVectorSetY(_data, Value);
}

float Vector2::DotProduct(const Vector2& Other) const
{
    return GetSingleValueFromReplicatedVector(DirectX::XMVector2Dot(_data, Other._data));
}

Vector2 Vector2::GetPseudoCrossProduct() const
{
    return {};
}

Vector2 Vector2::GetNormalized() const
{
    return { DirectX::XMVector2Normalize(_data) };
}

Vector2 Vector2::Normalize()
{
    _data = DirectX::XMVector2Normalize(_data);
    return *this;
}

std::string Vector2::GetAsString(int Presicion/*=3*/) const
{
    std::stringstream stream;
    
    stream << std::fixed << std::setprecision(Presicion) << "<"
        << GetX() << ", "
        << GetY() << ">";

    return stream.str();
}

float Vector2::GetAngleInDegreesWith(const Vector2& Other) const
{
    return DirectX::XMConvertToDegrees(GetAngleInRadiansWith(Other));
}

float Vector2::GetAngleInRadiansWith(const Vector2& Other) const
{
    return GetSingleValueFromReplicatedVector(DirectX::XMVector2AngleBetweenVectors(_data, Other._data));
}

float Vector2::GetMagnitude() const
{
    return {};
    //return std::sqrt(*this * *this);
}

float Vector2::GetMagnitudeSquared() const
{
    return {};
    //return *this * *this;
}

float Vector2::GetMagnitudeEstimated() const
{
    return GetSingleValueFromReplicatedVector(DirectX::XMVector2Length(_data));
}

float Vector2::DotProduct(const Vector2& lhs, const Vector2& rhs)
{
    return {};
    //return lhs * rhs;
}

//Vector2 Vector2::GetPseudoCrossProduct(const Vector2& lhs, const Vector2& rhs)
//{
//    return lhs.GetPseudoCrossProduct();
//}

float Vector2::GetAngleInDegreesBetween(const Vector2& lhs, const Vector2& rhs)
{
    return lhs.GetAngleInDegreesWith(rhs);
}

float Vector2::GetAngleInRadiansBetween(const Vector2& lhs, const Vector2& rhs)
{
    return lhs.GetAngleInRadiansWith(rhs);
}

float Vector2::GetSingleValueFromReplicatedVector(const DirectX::XMVECTOR& Vector) const
{
    return DirectX::XMVectorGetX(Vector);
}

Vector2 Vector2::GetZeroVector()
{
    return { DirectX::XMVectorZero() };
}

Vector2 Vector2::GetSplatOneVector()
{
    return { DirectX::XMVectorSplatOne() };
}

}