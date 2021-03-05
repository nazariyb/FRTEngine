#pragma once
#include "MathLibInternal.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <string>

namespace PROJECT_NAMESPACE
{

#pragma region ForwardDeclarations

template<typename ScalarType, typename SizeType = unsigned int>
class MATHLIB_API Matrix;

template<typename ScalarType, typename SizeType = unsigned int>
Matrix<ScalarType, SizeType> operator+(const Matrix<ScalarType, SizeType>& lhs, const Matrix<ScalarType, SizeType>& rhs);

template<typename ScalarType, typename SizeType = unsigned int>
Matrix<ScalarType, SizeType> operator-(const Matrix<ScalarType, SizeType>& lhs, const Matrix<ScalarType, SizeType>& rhs);

#pragma endregion

template<typename ScalarType, typename SizeType=unsigned int>
class MATHLIB_API Matrix
{
public:

#pragma region MatrixRow

    class Row
    {
    public:
        Row(Matrix<ScalarType, SizeType>& matrix, SizeType row)
            : _matrix(matrix), _row(row) {}

        ScalarType& operator[](SizeType column)
            { return _matrix(_row, column); }

    private:
        Matrix& _matrix;
        SizeType _row;
    };

    //class ConstRow
    //{
    //public:
    //    ConstRow(Matrix<ScalarType, SizeType>& matrix, SizeType row)
    //        : _matrix(matrix), _row(row) {}

    //    const ScalarType& operator[](SizeType column) const
    //    { return _matrix(_row, column); }

    //private:
    //    Matrix& _matrix;
    //    SizeType _row;
    //};
#pragma endregion

#pragma region MatrixDeclaration

public:
    Matrix(): Matrix<ScalarType, SizeType>(3) {}

    explicit Matrix(SizeType size) : Matrix<ScalarType, SizeType>(size, size) {}

    Matrix(SizeType size, std::vector<ScalarType> data) : Matrix<ScalarType, SizeType>(size, size, data) {}

    Matrix(SizeType rows, SizeType columns) : _rows(rows), _columns(columns)
    {
        _data.reserve(_rows * _columns);
    }

    Matrix(SizeType rows, SizeType columns, std::vector<ScalarType> data)
        : _rows(rows), _columns(columns), _data(data)
    {}


    ScalarType& operator()(SizeType row, SizeType column)
    {
        return _data[(_columns - 1) * row + (_rows - 1) * column + 1];
    }

    const ScalarType& operator()(SizeType row, SizeType column) const
    {
        return operator()(row, column);
    }

    Row operator[](SizeType row)
    {
        return Matrix::Row{ *this, row };
    }

    //const ConstRow& operator[](SizeType row) const
    //{
    //    return Matrix::ConstRow{ *this, row };
    //}

    friend Matrix<ScalarType, SizeType> operator+<>(const Matrix<ScalarType, SizeType>& lhs, const Matrix<ScalarType, SizeType>& rhs);
    friend Matrix<ScalarType, SizeType> operator-<>(const Matrix<ScalarType, SizeType>& lhs, const Matrix<ScalarType, SizeType>& rhs);

    Matrix<ScalarType, SizeType> operator*(const Matrix<ScalarType, SizeType>& other) const
    {
        // TODO: dot product
        return *this;
    }

    Matrix<ScalarType, SizeType>& operator+=(const Matrix<ScalarType, SizeType>& other)
    {
        _data = operator+(other)._data;
        return *this;
    }

    Matrix<ScalarType, SizeType>& operator-=(const Matrix<ScalarType, SizeType>& other)
    {
        _data = operator-(other)._data;
        return *this;
    }

    Matrix<ScalarType, SizeType>& operator*=(const Matrix<ScalarType, SizeType>& other)
    {
        // TODO
        return *this;
    }

    //const Matrix& operator+=(const Matrix& other)
    //{
    //    return this += other;
    //}

    //const Matrix& operator-=(const Matrix& other)
    //{
    //    return this -= other;
    //}

    //const Matrix& operator*=(const Matrix& other)
    //{
    //    // TODO
    //    return *this;
    //}

    bool operator==(const Matrix<ScalarType, SizeType>& other) const
    {
        return _data == other._data;
    }

    bool operator!=(const Matrix<ScalarType, SizeType>& other) const
    {
        return _data != other._data;
    }

    //friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
    std::string GetAsString()
    {
        std::string result{};
        for (SizeType row = 0, i = 0; row < _rows; ++row)
        {
            for (SizeType column = 0; column < _columns; ++column, ++i)
            {
                result += std::to_string(_data[i]) + " ";
            }
            result += "\n";
        }
        return result;
    }

private:
    std::vector<ScalarType> _data;
    SizeType _rows;
    SizeType _columns;

    template<typename _Fn>
    static Matrix<ScalarType, SizeType> AriphmeticOperationInner(const Matrix& lhs, const Matrix& rhs, _Fn _Func)
    {
        // TODO
        //assert(_data.size() == other._data.size());

        std::vector<ScalarType> result;
        result.reserve(lhs._data.size());

        std::transform(lhs._data.begin(), lhs._data.end(), rhs._data.begin(),
                       std::back_inserter(result), _Func);

        return { (SizeType)lhs._rows, (SizeType)lhs._rows, result };
    }
};

#pragma endregion

#pragma region MatrixDefinition

template<typename ScalarType, typename SizeType=unsigned int>
Matrix<ScalarType, SizeType> operator+(const Matrix<ScalarType, SizeType>& lhs, const Matrix<ScalarType, SizeType>& rhs)
{
    return Matrix<ScalarType, SizeType>::AriphmeticOperationInner(lhs, rhs, std::plus<ScalarType>());
}

template<typename ScalarType, typename SizeType = unsigned int>
Matrix<ScalarType, SizeType> operator-(const Matrix<ScalarType, SizeType>& lhs, const Matrix<ScalarType, SizeType>& rhs)
{

    return Matrix<ScalarType, SizeType>::AriphmeticOperationInner(lhs, rhs, std::minus<ScalarType>());
}

#pragma endregion

} // end namespace
