#pragma once

#include "MathLibInternal.h"
#include "Vector/Vector2Int.h"

namespace PROJECT_NAMESPACE
{

class MATHLIB_API Vector3Int :
    public Vector2Int
{
public:
    Vector3Int();
    Vector3Int(const Vector2Int& XYVector, int Z = 0);
    Vector3Int(int X, const Vector2Int& YZVector);
    Vector3Int(const Vector3Int& Other);
    Vector3Int& operator=(const Vector3Int Other);

    Vector3Int(int Value);
    Vector3Int(int X, int Y, int Z=0);
    Vector3Int(const int DataSource[]);
    Vector3Int(const std::pair<int, int>& XYDataSource, int Z = 0);
    Vector3Int(int X, const std::pair<int, int>& YZDataSource);
    Vector3Int(const std::tuple<int, int>& XYDataSource, int Z = 0);
    Vector3Int(int X, const std::tuple<int, int>& YZDataSource);
    Vector3Int(const std::tuple<int, int, int>& DataSource);
    Vector3Int(const std::vector<int>& DataSource);
    Vector3Int(const DirectX::XMVECTOR& DataSource);
    Vector3Int(const DirectX::XMINT3& DataSource);

    virtual ~Vector3Int() override;

public:
    int GetZ() const;
    void SetZ(int Value);

public:
    int DotProduct(const Vector3Int& Other) const;

    Vector3Int CrossProduct(const Vector3Int& Other) const;

    //virtual float GetMagnitude() const override;

    //virtual float GetMagnitudeSquared() const override;

    //virtual float GetMagnitudeEstimated() const override;

    Vector3Int GetNormalized() const;

    virtual std::string GetAsString() const override;

    float GetAngleInDegreesWith(const Vector3Int& Other) const;

    float GetAngleInRadiansWith(const Vector3Int& Other) const;

    static Vector3Int GetZeroVector();

    static Vector3Int GetSplatOneVector();

    static int DotProduct(const Vector3Int& lhs, const Vector3Int& rhs);

    static Vector3Int CrossProduct(const Vector3Int& lhs, const Vector3Int& rhs);

    static float GetAngleInDegreesBetween(const Vector3Int& lhs, const Vector3Int& rhs);

    static float GetAngleInRadiansBetween(const Vector3Int& lhs, const Vector3Int& rhs);

    friend Vector3Int MATHLIB_API operator+(const Vector3Int& lhs, const Vector3Int& rhs);

    friend Vector3Int MATHLIB_API operator+(int Scalar, const Vector3Int& Vector);

    friend Vector3Int MATHLIB_API operator+(const Vector3Int& Vector, int Scalar);

    friend Vector3Int MATHLIB_API operator-(const Vector3Int& lhs, const Vector3Int& rhs);

    friend Vector3Int MATHLIB_API operator-(const Vector3Int& Vector, int Scalar);

    friend int MATHLIB_API operator*(const Vector3Int& lhs, const Vector3Int& rhs);

    friend Vector3Int MATHLIB_API operator*(int Scalar, const Vector3Int& Vector);

    friend Vector3Int MATHLIB_API operator*(const Vector3Int& Vector, int Scalar);

    friend bool MATHLIB_API operator==(const Vector3Int& lhs, const Vector3Int& rhs);

    friend bool MATHLIB_API operator==(int Scalar, const Vector3Int& Vector);

    friend bool MATHLIB_API operator==(const Vector3Int& Vector, int Scalar);

    friend bool MATHLIB_API operator!=(const Vector3Int& lhs, const Vector3Int& rhs);

    friend bool MATHLIB_API operator!=(int Scalar, const Vector3Int& Vector);

    friend bool MATHLIB_API operator!=(const Vector3Int& Vector, int Scalar);

};
}

