#pragma once

#include "templates/utility.h"
#include "math_types.h"
#include "vec4.h"

namespace Math
{
	/**
	 * @brief A templated 4 by 4 matrix.
	 * 
	 * @tparam T the type of the matrix
	 */
	template<typename T>
	struct alignas(4 * sizeof(T)) Mat4
	{
		union
		{
			/* Matrix data as 2D array. */
			T data[4][4];

			/* Matrix data as row vectors. */
			Vec4<T> rows[4];
		};

		/**
		 * @brief Construct a zero-initialized
		 * matrix.
		 */
		constexpr FORCE_INLINE Mat4()
			: data{}
		{
			//
		}

		/**
		 * @brief Construct a new matrix and set all
		 * values equal to a scalar value.
		 * 
		 * @param s a scalar value
		 */
		constexpr FORCE_INLINE Mat4(T const& s)
			: data{{s, s, s, s},
			       {s, s, s, s},
				   {s, s, s, s},
				   {s, s, s, s}}
		{
			//
		}

		/**
		 * @brief Construct a new matrix with the
		 * given values.
		 */
		constexpr FORCE_INLINE Mat4(T const& a, T const& b, T const& c, T const& d,
		                            T const& e, T const& f, T const& g, T const& h,
		                            T const& i, T const& j, T const& k, T const& l,
		                            T const& m, T const& n, T const& o, T const& p)
			: data{{a, b, c, d},
			       {e, f, g, h},
				   {i, j, k, l},
				   {m, n, o, p}}
		{
			//
		}

		/**
		 * @brief Construct a new matrix and set the
		 * upper left values equal to the given 3 by
		 * 3 matrix.
		 * 
		 * @param other a 3 by 3 matrix
		 */
		constexpr FORCE_INLINE Mat4(Mat3<T> const& other)
			: data{{other.data[0][0], other.data[0][1], other.data[0][2]},
			       {other.data[1][0], other.data[1][1], other.data[1][2]},
			       {other.data[2][0], other.data[2][1], other.data[2][2]}}
		{
			//
		}

		/**
		 * @brief Returns a pointer to the matrix
		 * data.
		 * @{
		 */
		constexpr FORCE_INLINE T const* operator*() const
		{
			return *data;
		}

		constexpr FORCE_INLINE T* operator*()
		{
			return *data;
		}
		/** @} */

		/**
		 * @brief Returns the i-th row of the matrix
		 * as a 4D vector.
		 * 
		 * @param idx row index
		 * @return ref to matrix row vector
		 * @{
		 */
		constexpr FORCE_INLINE Vec4<T> const& operator[](int32 idx) const
		{
			return rows[idx];
		}

		constexpr FORCE_INLINE Vec4<T>& operator[](int32 idx)
		{
			return rows[idx];
		}
		/** @} */
		
		/**
		 * @brief Transpose the matrix in place.
		 */
		constexpr FORCE_INLINE Mat4& transpose()
		{
			swap(data[0][1], data[1][0]);
			swap(data[0][2], data[2][0]);
			swap(data[1][2], data[2][1]);
			swap(data[0][3], data[3][0]);
			swap(data[1][3], data[3][1]);
			swap(data[2][3], data[3][2]);

			return *this;
		}

		/**
		 * @brief Returns a transposed copy of
		 * this matrix.
		 */
		constexpr FORCE_INLINE Mat4 getTransposed() const
		{
			return Mat4{*this}.transpose();
		}

		/**
		 * @brief Sum another matrix element by element.
		 * 
		 * @param other another matrix
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Mat4& operator+=(Mat4 const& other)
		{
			for (int32 i = 0; i < 16; ++i)
			{
				(*data)[i] += (*other.data)[i];
			}

			return *this;
		}

		/**
		 * @brief Subtract another matrix element by element.
		 * 
		 * @param other another matrix
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Mat4& operator-=(Mat4 const& other)
		{
			for (int32 i = 0; i < 16; ++i)
			{
				(*data)[i] -= (*other.data)[i];
			}

			return *this;
		}

		/**
		 * @brief Multiply by another matrix element
		 * by element.
		 * 
		 * @param other another matrix
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Mat4& operator*=(Mat4 const& other)
		{
			for (int32 i = 0; i < 16; ++i)
			{
				(*data)[i] *= (*other.data)[i];
			}

			return *this;
		}

		/**
		 * @brief Divide by another matrix element
		 * by element.
		 * 
		 * @param other another matrix
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Mat4& operator/=(Mat4 const& other)
		{
			for (int32 i = 0; i < 16; ++i)
			{
				(*data)[i] /= (*other.data)[i];
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
		constexpr FORCE_INLINE Mat4 operator+(Mat4 const& other) const
		{
			return Mat4{*this} += other;
		}

		/**
		 * @brief Compute the element-wise sub of
		 * two matrices.
		 * 
		 * @param other another matrix
		 * @return new matrix 
		 */
		constexpr FORCE_INLINE Mat4 operator-(Mat4 const& other) const
		{
			return Mat4{*this} -= other;
		}

		/**
		 * @brief Compute the element-wise product of
		 * two matrices.
		 * 
		 * @param other another matrix
		 * @return new matrix 
		 */
		constexpr FORCE_INLINE Mat4 operator*(Mat4 const& other) const
		{
			return Mat4{*this} *= other;
		}

		/**
		 * @brief Compute the element-wise division of
		 * two matrices.
		 * 
		 * @param other another matrix
		 * @return new matrix 
		 */
		constexpr FORCE_INLINE Mat4 operator/(Mat4 const& other) const
		{
			return Mat4{*this} /= other;
		}

		/**
		 * @brief Compute the dot product between this
		 * matrix and a vector.
		 * 
		 * If the matri decribes a transformation, the
		 * vector is transformed accordingly.
		 * 
		 * @param v a 4D vector
		 * @return transformed vector
		 */
		constexpr FORCE_INLINE Vec4<T> dot(Vec4<T> const& v) const
		{
			return {
				rows[0].dot(v),
				rows[1].dot(v),
				rows[2].dot(v),
				rows[3].dot(v)
			};
		}
		
		/**
		 * @brief Compute the dot product of two matrices.
		 * 
		 * If both matrices describe a transformation, the
		 * resulting matrix will also describe a
		 * transformation.
		 * 
		 * @param other another matrix
		 * @return new matrix
		 */
		constexpr FORCE_INLINE Mat4 dot(Mat4 const& other) const
		{
			return Mat4{*this}.multiplyTransposed(other.getTransposed());
		}

		/**
		 * @brief Return the inverse of this matrix.
		 * 
		 * If this matrix describes a transformation,
		 * the inverse will describe the inverse
		 * transformation.
		 * 
		 * @return new matrix
		 */
		constexpr FORCE_INLINE Mat4 operator!() const
		{
			Mat4 comp = getComplementsMatrix();
			T const invdet = T{1} / rows[0].dot(comp.rows[0]);
			return comp.transpose() *= invdet;
		}

		/**
		 * @brief Invert this matrix in place.
		 */
		constexpr FORCE_INLINE Mat4& invert()
		{
			return *this = !*this;
		}

		/**
		 * @brief Returns the upper left 3 by 3 matrix.
		 */
		constexpr FORCE_INLINE operator Mat3<T>() const
		{
			return Mat3{data[0][0], data[0][1], data[0][2],
			            data[1][0], data[1][1], data[1][2],
			            data[2][0], data[2][1], data[2][2]};
		}

		/**
		 * @brief Create a new matrix and set the
		 * values of the diagonal equal to the
		 * given vector coordinates.
		 * 
		 * @param v diagonal vector
		 * @return new matrix
		 */
		static constexpr FORCE_INLINE Mat4 diag(Vec4<T> const& v)
		{
			return {v.x, T{}, T{}, T{},
			        T{}, v.y, T{}, T{},
			        T{}, T{}, v.z, T{},
			        T{}, T{}, T{}, v.w};
		}

	private:
		/**
		 * @brief Internal method that multiply this
		 * matrix by a transposed matrix.
		 * 
		 * This is equivalent to `m @ n.T`.
		 * 
		 * The memory layout of the transposed matrix
		 * allows for optimizations.
		 * 
		 * @param other a transposed matrix
		 * @return ref to self
		 */
		constexpr Mat4& multiplyTransposed(Mat4 const& other)
		{
			for (int32 i = 0; i < 4; ++i)
			{
				rows[i] = other.dot(rows[i]);
			}

			return *this;
		}

		/**
		 * @brief Returns the matrix of the algebraic
		 * complements of this matrix.
		 * 
		 * The result is used to compute the inverse
		 * matrix.
		 */
		constexpr Mat4 getComplementsMatrix() const
		{
			T const afbe = data[0][0] * data[1][1] - data[0][1] * data[1][0],
			        agce = data[0][0] * data[1][2] - data[0][2] * data[1][0],
			        ahde = data[0][0] * data[1][3] - data[0][3] * data[1][0],
			        bgcf = data[0][1] * data[1][2] - data[0][2] * data[1][1],
			        bhdf = data[0][1] * data[1][3] - data[0][3] * data[1][1],
			        chdg = data[0][2] * data[1][3] - data[0][3] * data[1][2],
			        injm = data[2][0] * data[3][1] - data[2][1] * data[3][0],
			        iokm = data[2][0] * data[3][2] - data[2][2] * data[3][0],
			        iplm = data[2][0] * data[3][3] - data[2][3] * data[3][0],
			        jokn = data[2][1] * data[3][2] - data[2][2] * data[3][1],
			        jpln = data[2][1] * data[3][3] - data[2][3] * data[3][1],
			        kplo = data[2][2] * data[3][3] - data[2][3] * data[3][2];
			
			return {
				+ data[1][1] * kplo - data[1][2] * jpln + data[1][3] * jokn,
				- data[1][0] * kplo + data[1][2] * iplm - data[1][3] * iokm,
				+ data[1][0] * jpln - data[1][1] * iplm + data[1][3] * injm,
				- data[1][0] * jokn + data[1][1] * iokm - data[1][2] * injm,

				- data[0][1] * kplo + data[0][2] * jpln - data[0][3] * jokn,
				+ data[0][0] * kplo - data[0][2] * iplm + data[0][3] * iokm,
				- data[0][0] * jpln + data[0][1] * iplm - data[0][3] * injm,
				+ data[0][0] * jokn - data[0][1] * iokm + data[0][2] * injm,
				
				+ data[3][1] * chdg - data[3][2] * bhdf + data[3][3] * bgcf,
				- data[3][0] * chdg + data[3][2] * ahde - data[3][3] * agce,
				+ data[3][0] * bhdf - data[3][1] * ahde + data[3][3] * afbe,
				- data[3][0] * bgcf + data[3][1] * agce - data[3][2] * afbe,

				- data[2][1] * chdg + data[2][2] * bhdf - data[2][3] * bgcf,
				+ data[2][0] * chdg - data[2][2] * ahde + data[2][3] * agce,
				- data[2][0] * bhdf + data[2][1] * ahde - data[2][3] * afbe,
				+ data[2][0] * bgcf - data[2][1] * agce + data[2][2] * afbe
			};
		}
	};
} // namespace Math
