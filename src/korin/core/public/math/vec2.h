#pragma once

#include "math_types.h"

namespace Math
{
	/**
	 * @brief A templated 2-coordinates
	 * vector.
	 *
	 * @tparam T the type of the coordinates
	 */
	template<typename T>
	struct Vec2
	{
		union
		{
			/* Vector as C array. */
			T data[2];

			struct
			{
				/* Position coordinates. */
				T x, y;
			};

			struct
			{
				/* Texture coordinates. */
				T u, v;
			};
		};

		/**
		 * @brief Construct a zero vector.
		 */
		constexpr FORCE_INLINE Vec2()
			: data{}
		{
			//
		}

		/**
		 * @brief Construct a vector with the
		 * given coordinates.
		 *
		 * @param inX,inY coordinates values
		 */
		constexpr FORCE_INLINE Vec2(T const& inX, T const& inY)
			: data{inX, inY}
		{
			//
		}

		/**
		 * @brief Construct a vector and set
		 * coordinates equal to the given
		 * scalar value.
		 *
		 * @param s a scalar value
		 */
		constexpr FORCE_INLINE Vec2(T const& s)
			: Vec2{s, s}
		{
			//
		}

		/**
		 * @brief Return the i-th coordinate of
		 * the vector.
		 *
		 * @param idx the index of the coordinate.
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
		 * @param other another 2D vector
		 * @return sum of the element-wise multiplication
		 * of two vectors
		 */
		constexpr FORCE_INLINE T dot(Vec2 const& other) const
		{
			return x * other.x + y * other.y;
		}

		/**
		 * @brief Return a vector with same length
		 * but opposite direction.
		 */
		constexpr FORCE_INLINE Vec2 operator-() const
		{
			return {-x, -y};
		}

		/**
		 * @brief Add the coordinates of another
		 * vector to this vector.
		 *
		 * @param other another 2D vector
		 * @return ref to self
		 */
		constexpr FORCE_INLINE Vec2& operator+=(Vec2 const& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		/**
		 * @brief Subtract the coordinates of another
		 * vector from this vector.
		 *
		 * @param other another 2D vector
		 * @return ref to self
		 */
		constexpr FORCE_INLINE Vec2& operator-=(Vec2 const& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		/**
		 * @brief Multiply the coordinates of this
		 * vector by another vector.
		 *
		 * @param other another 2D vector
		 * @return ref to self
		 */
		constexpr FORCE_INLINE Vec2& operator*=(Vec2 const& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}

		/**
		 * @brief Divide the coordinates of this
		 * vector by another vector.
		 *
		 * @param other another 2D vector
		 * @return ref to self
		 */
		constexpr FORCE_INLINE Vec2& operator/=(Vec2 const& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		/**
		 * @brief Compute the element-wise sum
		 * of this vector with another vector.
		 *
		 * @param other another 2D vector
		 * @return new vector
		 */
		constexpr FORCE_INLINE Vec2 operator+(Vec2 const& other) const
		{
			return Vec2{*this} += other;
		}

		/**
		 * @brief Compute the element-wise difference
		 * of this vector with another vector.
		 *
		 * @param other another 2D vector
		 * @return new vector
		 */
		constexpr FORCE_INLINE Vec2 operator-(Vec2 const& other) const
		{
			return Vec2{*this} -= other;
		}

		/**
		 * @brief Compute the element-wise multiplication
		 * of this vector by another vector.
		 *
		 * @param other another 2D vector
		 * @return new vector
		 */
		constexpr FORCE_INLINE Vec2 operator*(Vec2 const& other) const
		{
			return Vec2{*this} *= other;
		}

		/**
		 * @brief Compute the element-wise division
		 * of this vector by another vector.
		 *
		 * @param other another 2D vector
		 * @return new vector
		 */
		constexpr FORCE_INLINE Vec2 operator/(Vec2 const& other) const
		{
			return Vec2{*this} /= other;
		}

		/**
		 * @brief Return a new vector whose coordinates
		 * are the sum of the coordinates of this vector
		 * with a scalar value.
		 *
		 * @param s a scalar value
		 * @param other a 2D vector
		 * @return new vector
		 */
		friend constexpr FORCE_INLINE Vec2 operator+(T const& s, Vec2 const& other)
		{
			return other + s;
		}

		/**
		 * @brief Return a vector whose equal to the
		 * element-wise difference between a scalar
		 * and this vector
		 *
		 * @param s a scalar value
		 * @param other a 2D vector
		 * @return new vector
		 */
		constexpr FORCE_INLINE friend Vec2 operator-(T const& s, Vec2 const& other)
		{
			return {s - other.x, s - other.y};
		}

		/**
		 * @brief Return a new vector whose coordinates
		 * are equal to the product of the coordinates
		 * of this vector with a scalar value.
		 *
		 * @param s a scalar value
		 * @param other a 2D vector
		 * @return new vector
		 */
		friend constexpr FORCE_INLINE Vec2 operator*(T const& s, Vec2 const& other)
		{
			return other * s;
		}

		/**
		 * @brief Return a vector whose equal to the
		 * element-wise division between a scalar
		 * and this vector
		 *
		 * @param s a scalar value
		 * @param other a 2D vector
		 * @return new vector
		 */
		constexpr FORCE_INLINE friend Vec2 operator/(T const& s, Vec2 const& other)
		{
			return {s / other.x, s / other.y};
		}
	};
} // namespace Math
