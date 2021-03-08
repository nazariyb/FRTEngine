#pragma once

#include "MathLibInternal.h"
#include "Vector/Vector3Int.h"

namespace PROJECT_NAMESPACE
{

class MATHLIB_API Vector4Int :
    public Vector3Int
{
public:
    Vector4Int();
    Vector4Int(const Vector2Int& XYVector, const Vector2Int& ZWVector);
    Vector4Int(const Vector3Int& XYZVector, int W = 0);
    Vector4Int(int X, const Vector3Int& YZWVector);
    Vector4Int(const Vector4Int& Other);
    Vector4Int& operator=(const Vector4Int Other);

    Vector4Int(int Value);
    Vector4Int(int X, int Y, int Z = 0, int W = 0);
    Vector4Int(const int DataSource[]);
    Vector4Int(const std::pair<int, int>& XYDataSource, const std::pair<int, int>& ZWDataSource);
    Vector4Int(const std::tuple<int, int>& XYDataSource, const std::tuple<int, int>& ZWDataSource);
    Vector4Int(const std::vector<int>& DataSource);
    Vector4Int(const DirectX::XMVECTOR& DataSource);
    Vector4Int(const DirectX::XMINT4& DataSource);

    virtual ~Vector4Int() override;

public:
    int GetW() const;
    void SetW(int Value);

public:
    int DotProduct(const Vector4Int& Other) const;

    Vector4Int CrossProduct(const Vector4Int& Other) const;

    //virtual float GetMagnitude() const override;

    //virtual float GetMagnitudeSquared() const override;

    //virtual float GetMagnitudeEstimated() const override;

    Vector4Int GetNormalized() const;

    virtual std::string GetAsString() const override;

    float GetAngleInDegreesWith(const Vector4Int& Other) const;

    float GetAngleInRadiansWith(const Vector4Int& Other) const;

    static Vector4Int GetZeroVector();

    static Vector4Int GetSplatOneVector();

    static int DotProduct(const Vector4Int& lhs, const Vector4Int& rhs);

    static Vector4Int CrossProduct(const Vector4Int& lhs, const Vector4Int& rhs);

    static float GetAngleInDegreesBetween(const Vector4Int& lhs, const Vector4Int& rhs);

    static float GetAngleInRadiansBetween(const Vector4Int& lhs, const Vector4Int& rhs);

    friend Vector4Int MATHLIB_API operator+(const Vector4Int& lhs, const Vector4Int& rhs);

    friend Vector4Int MATHLIB_API operator+(int Scalar, const Vector4Int& Vector);

    friend Vector4Int MATHLIB_API operator+(const Vector4Int& Vector, int Scalar);

    friend Vector4Int MATHLIB_API operator-(const Vector4Int& lhs, const Vector4Int& rhs);

    friend Vector4Int MATHLIB_API operator-(const Vector4Int& Vector, int Scalar);

    friend int MATHLIB_API operator*(const Vector4Int& lhs, const Vector4Int& rhs);

    friend Vector4Int MATHLIB_API operator*(int Scalar, const Vector4Int& Vector);

    friend Vector4Int MATHLIB_API operator*(const Vector4Int& Vector, int Scalar);

    friend bool MATHLIB_API operator==(const Vector4Int& lhs, const Vector4Int& rhs);

    friend bool MATHLIB_API operator==(int Scalar, const Vector4Int& Vector);

    friend bool MATHLIB_API operator==(const Vector4Int& Vector, int Scalar);

    friend bool MATHLIB_API operator!=(const Vector4Int& lhs, const Vector4Int& rhs);

    friend bool MATHLIB_API operator!=(int Scalar, const Vector4Int& Vector);

    friend bool MATHLIB_API operator!=(const Vector4Int& Vector, int Scalar);

};
}

