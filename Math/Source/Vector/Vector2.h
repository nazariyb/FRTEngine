#pragma once

#include "MathLibInternal.h"
#include <string>
#include <vector>

namespace PROJECT_NAMESPACE
{
class MATHLIB_API Vector2
{
    friend class Vector2Int;
    friend class Vector3Int;
    friend class Vector4Int;

    friend class Vector3;
    friend class Vector4;

public:
    Vector2();
    Vector2(const Vector2& Other);
    Vector2(const class Vector2Int& Other);
    Vector2(const class Vector3& Other);
    Vector2& operator=(const Vector2& Other);

    explicit Vector2(const float Value);
    Vector2(const float X, const float Y);
    explicit Vector2(const std::pair<float, float>& DataSource);
    explicit Vector2(const float DataSource[]);
    explicit Vector2(const std::tuple<float, float>& DataSource);
    explicit Vector2(const std::vector<float>& DataSource);
    Vector2(const DirectX::XMVECTOR& DataSource);
    explicit Vector2(const DirectX::XMFLOAT2& DataSource);

    virtual ~Vector2();

public:
    float GetX() const;
    float GetY() const;

    void SetX(float Value);
    void SetY(float Value);

public:
    float DotProduct(const Vector2& Other) const;

    Vector2 GetPseudoCrossProduct() const;

    virtual float GetMagnitude() const;

    virtual float GetMagnitudeSquared() const;

    virtual float GetMagnitudeEstimated() const;

    Vector2 GetNormalized() const;
    
    Vector2 Normalize();

    virtual std::string GetAsString(int Presicion=3) const;

    float GetAngleInDegreesWith(const Vector2& Other) const;

    float GetAngleInRadiansWith(const Vector2& Other) const;

    static Vector2 GetZeroVector();

    static Vector2 GetSplatOneVector();

    static float DotProduct(const Vector2& lhs, const Vector2& rhs);

    static Vector2 GetPseudoCrossProduct(const Vector2& lhs, const Vector2& rhs);

    static float GetAngleInDegreesBetween(const Vector2& lhs, const Vector2& rhs);

    static float GetAngleInRadiansBetween(const Vector2& lhs, const Vector2& rhs);

    friend Vector2 MATHLIB_API operator+(const Vector2& lhs, const Vector2& rhs);

    friend Vector2 MATHLIB_API operator+(float Scalar, const Vector2& Vector);

    friend Vector2 MATHLIB_API operator+(const Vector2& Vector, float Scalar);

    friend Vector2 MATHLIB_API operator-(const Vector2& lhs, const Vector2& rhs);

    friend Vector2 MATHLIB_API operator-(const Vector2& Vector, float Scalar);

    friend float MATHLIB_API operator*(const Vector2& lhs, const Vector2& rhs);

    friend Vector2 MATHLIB_API operator*(float Scalar, const Vector2& Vector);

    friend Vector2 MATHLIB_API operator*(const Vector2& Vector, float Scalar);

    friend bool MATHLIB_API operator==(const Vector2& lhs, const Vector2& rhs);

    friend bool MATHLIB_API operator==(float Scalar, const Vector2& Vector);

    friend bool MATHLIB_API operator==(const Vector2& Vector, float Scalar);

    friend bool MATHLIB_API operator!=(const Vector2& lhs, const Vector2& rhs);

    friend bool MATHLIB_API operator!=(float Scalar, const Vector2& Vector);

    friend bool MATHLIB_API operator!=(const Vector2& Vector, float Scalar);

protected:
    DirectX::XMVECTOR _data;

protected:
    float GetSingleValueFromReplicatedVector(const DirectX::XMVECTOR& Vector) const;
};
}