#pragma once

#include "templates/utility.h"
#include "math_types.h"
#include "vec3.h"

namespace Math
{
	/**
	 * @brief A templated 3 by 3 matrix.
	 * 
	 * @tparam T the type of the values
	 */
	template<typename T>
	struct Mat3
	{
		union
		{
			/* Matrix data as 2D array. */
			T data[3][3];

			/* Matrix data as row vectors. */
			Vec3<T> rows[3];
		};
		
		/**
		 * @brief Construct a zero-initialized matrix.
		 */
		constexpr FORCE_INLINE Mat3()
			: data{}
		{
			//
		}

		/**
		 * @brief Construct a matrix with the given
		 * values.
		 * 
		 * @param a,b,c,d,e,f,g,h,i  matrix values
		 */
		constexpr FORCE_INLINE Mat3(T const& a, T const& b, T const& c,
		                            T const& d, T const& e, T const& f,
		                            T const& g, T const& h, T const& i)
			: data{{a, b, c},
			       {d, e, f},
				   {g, h, i}}
		{
			//
		}

		/**
		 * @brief Construct a matrix and set all values
		 * equal to a scalar value.
		 * 
		 * @param s the scalar value
		 */
		constexpr FORCE_INLINE Mat3(T const& s)
			: data{{s, s, s},
			       {s, s, s},
				   {s, s, s}}
		{
			//
		}

		/**
		 * @brief Returns the i-th row vector of the
		 * matrix.
		 * 
		 * @param idx row index
		 * @return ref to row vector
		 * @{
		 */
		constexpr FORCE_INLINE Vec3<T> const& operator[](int32 idx) const
		{
			return rows[idx];
		}

		constexpr FORCE_INLINE Vec3<T>& operator[](int32 idx)
		{
			return rows[idx];
		}
		/** @} */

		/**
		 * @brief Returns a pointer to the memory
		 * that contains the matrix values.
		 * 
		 * The values are not guaranteed to be
		 * contiguous.
		 * @{
		 */
		constexpr FORCE_INLINE T const* operator*() const
		{
			return &data[0][0];
		}

		constexpr FORCE_INLINE T* operator*()
		{
			return &data[0][0];
		}
		/** @} */

		/**
		 * @brief Transpose this matrix in place.
		 */
		constexpr FORCE_INLINE Mat3& transpose()
		{
			swap(data[0][1], data[1][0]);
			swap(data[0][2], data[2][0]);
			swap(data[1][2], data[2][1]);
			return *this;
		}

		/**
		 * @brief Returns a transposed copy of the matrix.
		 */
		constexpr FORCE_INLINE Mat3 getTransposed() const
		{
			return Mat3{*this}.transpose();
		}

		/**
		 * @brief Add the values of another matrix to
		 * this matrix.
		 * 
		 * @param other another matrix
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Mat3& operator+=(Mat3 const& other)
		{
			for (int32 i = 0; i < 9; ++i)
			{
				(**this)[i] += (*other)[i];
			}

			return *this;
		}

		/**
		 * @brief Subtract the values of another matrix
		 * from this matrix.
		 * 
		 * @param other another matrix
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Mat3& operator-=(Mat3 const& other)
		{
			for (int32 i = 0; i < 9; ++i)
			{
				(**this)[i] -= (*other)[i];
			}

			return *this;
		}

		/**
		 * @brief Multiply this matrix by another matrix,
		 * element-wise.
		 * 
		 * @param other another matrix
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Mat3& operator*=(Mat3 const& other)
		{
			for (int32 i = 0; i < 9; ++i)
			{
				(**this)[i] *= (*other)[i];
			}

			return *this;
		}

		/**
		 * @brief Divide this matrix by another matrix,
		 * element-wise.
		 * 
		 * @param other another matrix
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Mat3& operator/=(Mat3 const& other)
		{
			for (int32 i = 0; i < 9; ++i)
			{
				(**this)[i] /= (*other)[i];
			}

			return *this;
		}

		/**
		 * @brief Compute the element-wise sum of
		 * two matrices.
		 * 
		 * @param other another matrix
		 * @return new matrix 
		 */
		constexpr FORCE_INLINE Mat3 operator+(Mat3 const& other) const
		{
			return Mat3{*this} += other;
		}

		/**
		 * @brief Compute the element-wise difference
		 * between two matrices.
		 * 
		 * @param other another matrix
		 * @return new matrix 
		 */
		constexpr FORCE_INLINE Mat3 operator-(Mat3 const& other) const
		{
			return Mat3{*this} -= other;
		}

		/**
		 * @brief Compute the element-wise product
		 * of two matrices.
		 * 
		 * @param other another matrix
		 * @return new matrix 
		 */
		constexpr FORCE_INLINE Mat3 operator*(Mat3 const& other) const
		{
			return Mat3{*this} *= other;
		}

		/**
		 * @brief Compute the element-wise diovision
		 * between two matrices.
		 * 
		 * @param other another matrix
		 * @return new matrix 
		 */
		constexpr FORCE_INLINE Mat3 operator/(Mat3 const& other) const
		{
			return Mat3{*this} /= other;
		}

		/**
		 * @brief Invert this matrix in place.
		 */
		constexpr FORCE_INLINE Mat3& invert()
		{
			return *this = !*this;
		}
		
		/**
		 * @brief Compute the inverse of this matrix.
		 * 
		 * If this matrix describes a rotation or a
		 * scale transformation, the inverted matrix
		 * describes the inverse transformation.
		 */
		constexpr FORCE_INLINE Mat3 operator!() const
		{
			Mat3 comp = getComplementsMatrix();
			T const invdet = T{1} / comp.rows[0].dot(rows[0]);
			return comp.transpose() * invdet;
		}

		/**
		 * @brief Compute the dot product between this
		 * matrix and antoher matrix.
		 * 
		 * If both matrices describe a transformation,
		 * the resulting matrix describe a transformation
		 * equal to the application of the original
		 * transformations:
		 * 
		 * (X * Y) * v = X * (Y * v)
		 * 
		 * @param other another matrix
		 * @return new matrix 
		 */
		constexpr FORCE_INLINE Mat3 dot(Mat3 const& other) const
		{
			Mat3 res{*this};
			multiplyTransposed(res, other.getTransposed());
			return res;
		}

		/**
		 * @brief Compute the dot product between this
		 * matrix and a 3D vector.
		 * 
		 * If this matrix a rotation or a scale
		 * transformation, the vector is transformed
		 * accordingly.
		 * 
		 * @param v a 3D vector
		 * @return transformed vector 
		 */
		constexpr FORCE_INLINE Vec3<T> dot(Vec3<T> const& v) const
		{
			return {rows[0].dot(v), rows[1].dot(v), rows[2].dot(v)};
		}

	protected:
		/**
		 * @brief Multiplies (dot product) the first
		 * matrix with the second matrix, where the
		 * second matrix is assumed to be transposed.
		 * 
		 * lhs = lhs * (rhs.T).T
		 * 
		 * @param lhs modified matrix
		 * @param rhs another matrix
		 */
		static constexpr void multiplyTransposed(Mat3& lhs, Mat3 const& rhs)
		{
			for (int32 i = 0; i < 3; ++i)
			{
				lhs.rows[i] = rhs.dot(lhs.rows[i]);
			}
		}

		/**
		 * @brief Returns the matrix of the algebraic
		 * complements of this matrix.
		 */
		constexpr Mat3 getComplementsMatrix() const
		{
			return {
				data[1][1] * data[2][2] - data[1][2] * data[2][1],
				data[1][2] * data[2][0] - data[1][0] * data[2][2],
				data[1][0] * data[2][1] - data[1][1] * data[2][0],

				data[0][2] * data[2][1] - data[0][1] * data[2][2],
				data[0][0] * data[2][2] - data[0][2] * data[2][0],
				data[0][1] * data[2][0] - data[0][0] * data[2][1],

				data[0][1] * data[1][2] - data[0][2] * data[1][1],
				data[0][2] * data[1][0] - data[0][0] * data[1][2],
				data[0][0] * data[1][1] - data[0][1] * data[1][0]
			};
		}
	};
} // namespace Math
