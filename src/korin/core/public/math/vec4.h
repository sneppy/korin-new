#pragma once

#include "math_types.h"
#include "vec3.h"

namespace Math
{
	/**
	 * @brief A templated vectot with 4 coordinates.
	 * 
	 * @tparam T the type of the coordinates
	 */
	template<typename T>
	struct alignas(4 * sizeof(T)) Vec4
	{
		union
		{
			/* Vector data as C array. */
			T data[4];

			struct
			{
				/* Position coordinates. */
				T x, y, z, w;
			};

			struct
			{
				/* Color coordinates. */
				T r, g, b;
			};
		};

		/**
		 * @brief Construct a zero-vector.
		 */
		constexpr FORCE_INLINE Vec4()
			: data{}
		{
			//
		}

		/**
		 * @brief Construct a new vector with the
		 * given coordinates.
		 * 
		 * @param inX,inY,inZ,inW the coordinates values
		 */
		constexpr FORCE_INLINE Vec4(T const& inX, T const& inY, T const& inZ, T const& inW)
			: data{inX, inY, inZ, inW}
		{
			//
		}

		/**
		 * @brief Construct a new vector with all
		 * coordinates equal to a scalar value.
		 * 
		 * @param s a scalar value
		 */
		constexpr FORCE_INLINE Vec4(T const& s)
			: Vec4{s, s, s, s}
		{
			//
		}

		/**
		 * @brief Construct a new vector with the
		 * coordinates of a 3D vector and default-
		 * initialize the W coordinate.
		 * 
		 * @param other a 3D vector
		 */
		constexpr FORCE_INLINE explicit Vec4(Vec3<T> const& other)
			: data{other.x, other.y, other.z}
		{
			//
		}

		/**
		 * @brief Construct a new vector with the
		 * coordinates of the given 3D vector and
		 * the given W coordinate.
		 * 
		 * @param other a 3D vector
		 * @param inW the fourth coordinate
		 */
		constexpr FORCE_INLINE Vec4(Vec3<T> const& other, T const& inW)
			: Vec4{other.x, other.y, other.z, inW}
		{
			//
		}

		/**
		 * @brief Return the value of the i-th coordinate.
		 * 
		 * @param idx index of the coordinate
		 * @return ref to coordinate
		 */
		constexpr FORCE_INLINE T const& operator[](int32 idx) const
		{
			return data[idx];
		}

		constexpr FORCE_INLINE T& operator[](int32 idx)
		{
			return data[idx];
		}
		/** @} */

		/**
		 * @brief Compute the dot product between this
		 * vector and another vector
		 * 
		 * @param other another vector
		 * @return dot product of two vectors
		 */
		constexpr FORCE_INLINE T dot(Vec4 const& other) const
		{
			return x * other.x + y * other.y + z * other.z + w * other.w;
		}
		
		/**
		 * @brief Returns a vector with same length
		 * but opposite direction.
		 */
		constexpr FORCE_INLINE Vec4 operator-() const
		{
			return {-x, -y, -z, -w};
		}
		
		/**
		 * @brief Add the coordinates of another
		 * vector to the coordinates of this
		 * vector
		 * 
		 * @param other another vector
		 * @return ref to self
		 */
		constexpr FORCE_INLINE Vec4& operator+=(Vec4 const& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		
		/**
		 * @brief Subtract the coordinates of another
		 * vector from the coordinates of this vector
		 * 
		 * @param other another vector
		 * @return ref to self
		 */
		constexpr FORCE_INLINE Vec4& operator-=(Vec4 const& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		
		/**
		 * @brief Multiply the coordinates of this
		 * vector by the coordinates of another
		 * vector
		 * 
		 * @param other another vector
		 * @return ref to self
		 */
		constexpr FORCE_INLINE Vec4& operator*=(Vec4 const& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}
		
		/**
		 * @brief Divide the coordinates of this
		 * vector by the coordinates of another
		 * vector
		 * 
		 * @param other another vector
		 * @return ref to self
		 */
		constexpr FORCE_INLINE Vec4& operator/=(Vec4 const& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
			return *this;
		}
		
		/**
		 * @brief Compute the element-wise sum
		 * of this vector with another vector
		 * 
		 * @param other another vector
		 * @return new vector 
		 */
		constexpr FORCE_INLINE Vec4 operator+(Vec4 const& other) const
		{
			return Vec4{*this} += other;
		}
		
		/**
		 * @brief Compute the element-wise difference
		 * between this vector and another vector
		 * 
		 * @param other another vector
		 * @return new vector 
		 */
		constexpr FORCE_INLINE Vec4 operator-(Vec4 const& other) const
		{
			return Vec4{*this} -= other;
		}
		
		/**
		 * @brief Compute the element-wise multiplication
		 * of this vector by another vector
		 * 
		 * @param other another vector
		 * @return new vector 
		 */
		constexpr FORCE_INLINE Vec4 operator*(Vec4 const& other) const
		{
			return Vec4{*this} *= other;
		}
		
		/**
		 * @brief Compute the element-wise division
		 * of this vector by another vector
		 * 
		 * @param other another vector
		 * @return new vector 
		 */
		constexpr FORCE_INLINE Vec4 operator/(Vec4 const& other) const
		{
			return Vec4{*this} /= other;
		}
		
		/**
		 * @brief Returns a new vector whose coordinates
		 * are equal to the sum of the coordinates of
		 * this vector with a scalar value
		 * 
		 * @param s a scalar value
		 * @param other a vector
		 * @return new vector
		 */
		constexpr FORCE_INLINE friend Vec4 operator+(T const& s, Vec4 const& other)
		{
			return other + s;
		}
		
		/**
		 * @brief Returns a new vector whose coordinates
		 * are equal to the multiplication of the
		 * coordinates of this vector by a scalar value
		 * 
		 * @param s a scalar value
		 * @param other a vector
		 * @return new vector
		 */
		constexpr FORCE_INLINE friend Vec4 operator*(T const& s, Vec4 const& other)
		{
			return other * s;
		}

		/**
		 * @brief Returns a 3D vector with the first
		 * coordinates of this vector.
		 */
		constexpr FORCE_INLINE operator Vec3<T>() const
		{
			return {x, y, z};
		}
	};
} // namespace Math

