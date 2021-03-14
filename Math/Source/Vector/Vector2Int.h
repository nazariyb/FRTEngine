#pragma once

#include "MathLibInternal.h"
#include <string>
#include <vector>

namespace PROJECT_NAMESPACE
{

class MATHLIB_API Vector2Int
{
    friend class Vector2;

    friend class Vector3Int;
    friend class Vector4Int;

public:
    Vector2Int();
    Vector2Int(const class Vector2& Other, bool bRound=false);
    Vector2Int(const Vector2Int& Other);
    Vector2Int(const class Vector3Int& Other);
    Vector2Int& operator=(const Vector2Int& Other);

    explicit Vector2Int(const int Value);
    Vector2Int(const int X, const int Y);
    explicit Vector2Int(const std::pair<int, int>& DataSource);
    explicit Vector2Int(const int DataSource[]);
    explicit Vector2Int(const std::tuple<int, int>& DataSource);
    explicit Vector2Int(const std::vector<int>& DataSource);
    Vector2Int(const DirectX::XMVECTOR& DataSource);
    explicit Vector2Int(const DirectX::XMINT2& DataSource);

    virtual ~Vector2Int();

public:
    int GetX() const;
    int GetY() const;

    void SetX(int Value);
    void SetY(int Value);

public:
    int DotProduct(const Vector2Int& Other) const;

    Vector2Int GetPseudoCrossProduct() const;

    virtual float GetMagnitude() const;

    virtual float GetMagnitudeSquared() const;
    
    virtual float GetMagnitudeEstimated() const;

    Vector2Int GetNormalized() const;

    virtual std::string GetAsString() const;

    float GetAngleInDegreesWith(const Vector2Int& Other) const;

    float GetAngleInRadiansWith(const Vector2Int& Other) const;

    static Vector2Int GetZeroVector();

    static Vector2Int GetSplatOneVector();

    static int DotProduct(const Vector2Int& lhs, const Vector2Int& rhs);

    static Vector2Int GetPseudoCrossProduct(const Vector2Int& lhs, const Vector2Int& rhs);

    static float GetAngleInDegreesBetween(const Vector2Int& lhs, const Vector2Int& rhs);

    static float GetAngleInRadiansBetween(const Vector2Int& lhs, const Vector2Int& rhs);

    friend Vector2Int MATHLIB_API operator+(const Vector2Int & lhs, const Vector2Int & rhs);

    friend Vector2Int MATHLIB_API operator+(int Scalar, const Vector2Int & Vector);

    friend Vector2Int MATHLIB_API operator+(const Vector2Int & Vector, int Scalar);

    friend Vector2Int MATHLIB_API operator-(const Vector2Int & lhs, const Vector2Int & rhs);

    friend Vector2Int MATHLIB_API operator-(const Vector2Int & Vector, int Scalar);

    friend int MATHLIB_API operator*(const Vector2Int& lhs, const Vector2Int& rhs);

    friend Vector2Int MATHLIB_API operator*(int Scalar, const Vector2Int& Vector);

    friend Vector2Int MATHLIB_API operator*(const Vector2Int& Vector, int Scalar);

    friend bool MATHLIB_API operator==(const Vector2Int & lhs, const Vector2Int & rhs);

    friend bool MATHLIB_API operator==(int Scalar, const Vector2Int & Vector);

    friend bool MATHLIB_API operator==(const Vector2Int & Vector, int Scalar);

    friend bool MATHLIB_API operator!=(const Vector2Int & lhs, const Vector2Int & rhs);

    friend bool MATHLIB_API operator!=(int Scalar, const Vector2Int & Vector);

    friend bool MATHLIB_API operator!=(const Vector2Int& Vector, int Scalar);

protected:
    DirectX::XMVECTOR _data;

protected:
    float GetSingleValueFromReplicatedVector(const DirectX::XMVECTOR& Vector) const;
};
}

