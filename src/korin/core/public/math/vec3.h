#pragma once

#include "math_types.h"
#include "vec2.h"

namespace Math
{
	/**
	 * @brief A template vector with three coordinates.
	 * 
	 * @tparam T the type of the coordinates
	 */
	template<typename T>
	struct Vec3
	{
		union
		{
			/* Vector data as C array. */
			T data[3];

			struct
			{
				/* Position coordinates. */
				T x, y, z;
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
		constexpr FORCE_INLINE Vec3()
			: data{}
		{
			//
		}

		/**
		 * @brief Construct a new vector with the
		 * given coordinates.
		 * 
		 * @param inX,inY,inZ coordinates values
		 */
		constexpr FORCE_INLINE Vec3(T const& inX, T const& inY, T const& inZ)
			: data{inX, inY, inZ}
		{
			//
		}

		/**
		 * @brief Construct a new vector with all
		 * coordinates equal to the given scalar
		 * value.
		 * 
		 * @param s a scalar value
		 */
		constexpr FORCE_INLINE Vec3(T const& s)
			: Vec3{s, s, s}
		{
			//
		}

		/**
		 * @brief Construct a new vector with the
		 * X and Y coordinates of the given 2D vector
		 * and default-initialize the Z coordinate.
		 * 
		 * @param other a 2D vector
		 */
		constexpr FORCE_INLINE explicit Vec3(Vec2<T> const& other)
			: data{other.x, other.y}
		{
			//
		}

		/**
		 * @brief Construct a new vector with the
		 * coordinates of the given 2D vector and
		 * the given Z coordinate.
		 * 
		 * @param other a 2D vector
		 * @param inZ value for Z coordinate
		 */
		constexpr FORCE_INLINE Vec3(Vec2<T> const& other, T const& inZ)
			: Vec3{other.x, other.y, inZ}
		{
			//
		}

		/**
		 * @brief Returns the i-th coordinate of
		 * this vector.
		 * 
		 * @param idx vector coordinate
		 * @return ref to coordinate value
		 * @{
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
		 * @brief Computes the dot product of this
		 * vector with another vector.
		 * 
		 * @param other another vector
		 * @return dot product of two vectors
		 */
		constexpr FORCE_INLINE T dot(Vec3<T> const& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		/**
		 * @brief Returns a new vector with the same
		 * length but opposite direction.
		 */
		constexpr FORCE_INLINE Vec3 operator-() const
		{
			return {-x, -y, -z};
		}

		/**
		 * @brief Sum the coordinates of another vector
		 * to the coordinates of this vector.
		 * 
		 * @param other another vector
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Vec3& operator+=(Vec3 const& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		/**
		 * @brief Subtract the coordinates of another vector
		 * from the coordinates of this vector.
		 * 
		 * @param other another vector
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Vec3& operator-=(Vec3 const& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		/**
		 * @brief Multiply the coordinates of this vector
		 * by the coordinates of another vector.
		 * 
		 * @param other another vector
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Vec3& operator*=(Vec3 const& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		/**
		 * @brief Divide the coordinates of this vector
		 * by the coordinates of another vector.
		 * 
		 * @param other another vector
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Vec3& operator/=(Vec3 const& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}

		/**
		 * @brief Compute the element-wise sum
		 * of this vector with another vector.
		 * 
		 * @param other another vector
		 * @return new vector 
		 */
		constexpr FORCE_INLINE Vec3 operator+(Vec3 const& other) const
		{
			return Vec3{*this} += other;
		}

		/**
		 * @brief Compute the element-wise difference
		 * of this vector with another vector.
		 * 
		 * @param other another vector
		 * @return new vector 
		 */
		constexpr FORCE_INLINE Vec3 operator-(Vec3 const& other) const
		{
			return Vec3{*this} -= other;
		}

		/**
		 * @brief Compute the element-wise multiplication
		 * of this vector by another vector.
		 * 
		 * @param other another vector
		 * @return new vector 
		 */
		constexpr FORCE_INLINE Vec3 operator*(Vec3 const& other) const
		{
			return Vec3{*this} *= other;
		}

		/**
		 * @brief Compute the element-wise division
		 * of this vector by another vector.
		 * 
		 * @param other another vector
		 * @return new vector 
		 */
		constexpr FORCE_INLINE Vec3 operator/(Vec3 const& other) const
		{
			return Vec3{*this} /= other;
		}

		/**
		 * @brief Return a vector whose coordinates are
		 * equal to the sum of a scalar value with the
		 * coordinates of this vector.
		 * 
		 * @param s a scalar value
		 * @param other another 3D vector
		 * @return new vector 
		 */
		constexpr FORCE_INLINE friend Vec3 operator+(T const& s, Vec3 const& other)
		{
			return other + s;
		}

		/**
		 * @brief Return a vector whose coordinates are
		 * equal to the multiplication of a scalar value
		 * with the coordinates of this vector.
		 * 
		 * @param s a scalar value
		 * @param other another 3D vector
		 * @return new vector 
		 */
		constexpr FORCE_INLINE friend Vec3 operator*(T const& s, Vec3 const& other)
		{
			return other * s;
		}

		/**
		 * @brief Computes the cross product between
		 * this vector and another vector.
		 * 
		 * @param other another 2D vector
		 * @return cross product of two vectors 
		 */
		constexpr FORCE_INLINE Vec3 cross(Vec3 const& other) const
		{
			return {
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
			};
		}

		/**
		 * @brief Returns a 2D vector with the first
		 * two coordinates of this vector.
		 */
		constexpr FORCE_INLINE operator Vec2<T>() const
		{
			return {x, y};
		}
	};
} // namespace Math
