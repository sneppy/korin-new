#pragma once

#include "templates/types.h"
#include "math_types.h"
#include "vec4.h"

namespace Math
{
	/**
	 * @brief A quaternion mainly used to decribe
	 * 3D rotations.
	 * 
	 * @tparam T a floating point type
	 */
	template<typename T>
	struct Quat : protected Vec4<T>
	{
		static_assert(IsFloating<T>::value, "T must be either float32 or float64");

		/**
		 * @brief Construct a zero-rotation quaternion.
		 */
		constexpr FORCE_INLINE Quat()
			: Vec4<T>{0.f, 0.f, 0.f, 1.f}
		{
			//
		}

		/**
		 * @brief Construct a unit quaternion that
		 * describes a rotation around the given
		 * axis with the given angle.
		 * 
		 * @param angle angle of the rotation
		 * @param axis axis of the rotation
		 */
		constexpr FORCE_INLINE Quat(T angle, Vec3<T> const& axis)
		{
			// TODO: Tolerance for float tests
			CHECK(axis.dot(axis) == 1.f)

			T t = angle * 0.5f;
			T s = PlatformMath::sin(t);

			x = axis.x * s;
			y = axis.y * s;
			z = axis.z * s;
			w = PlatformMath::cos(t);
		}

		/**
		 * @brief Construct a unit quaternion that
		 * describes a rotation around the axis and
		 * angle given by the direction and length
		 * of the physics rotation vector.
		 * 
		 * @param phyrot rotation vector 
		 */
		constexpr FORCE_INLINE Quat(Vec3<T> const& phyrot)
		{
			CHECK(!(phyrot.x == 0.f && phyrot.y == 0.f && phyrot.z == 0.f))

			T angle = PlatformMath::sqrt(phyrot.dot(phyrot));
			T t = angle * 0.5f;
			T s = PlatformMath::sin(t) / angle;

			x = phyrot.x * s;
			y = phyrot.y * s;
			z = phyrot.z * s;
			w = PlatformMath::cos(t);
		}

		/**
		 * @brief Returns the rotation axis. 
		 */
		constexpr FORCE_INLINE Vec3<T> getAxis() const
		{
			Vec3<T> axis{*this};
			T c = PlatformMath::sqrt(axis.dot(axis));
			return axis / c;
		}

		/**
		 * @brief Get the angle of the rotation.
		 */
		constexpr FORCE_INLINE T getAngle() const
		{
			return PlatformMath::acos(w) * 2.f;
		}

		/**
		 * @brief Returns the axis and angle of
		 * the rotation.
		 * 
		 * @param outAxis return value of the axis
		 * @param outAngle return value of the angle
		 */
		constexpr FORCE_INLINE void getAxisAndAngle(Vec3<T>& outAxis, T& outAngle)
		{
			Vec3<T> axis{*this};
			T s = PlatformMath::sqrt(axis.dot(axis));
			T angle = PlatformMath::atan2(s, w);

			outAxis = axis / s;
			outAngle = angle * 2.f;
		}

		/**
		 * @brief Return a new quaternion that describes
		 * an inverse rotation.
		 */
		constexpr FORCE_INLINE Quat operator!() const
		{
			return Quat{x, y, z, -w};
		}

		/**
		 * @brief Compute the hamilton product between
		 * two quaternions.
		 * 
		 * If the quaternions describe a rotation, the
		 * resulting quaternion also describes a rotation
		 * whose application is equivalent to the
		 * application of the two rotations, right to left
		 * (e.g. `(x * y).rotate(v) => x.rotate(y.rotate(v))`) 
		 * 
		 * @param other another quaternion
		 * @return new quaternion
		 */
		constexpr FORCE_INLINE Quat operator*(Quat const& other) const
		{
			/* ai, bj, ck, d *
			   ei, fj, gk, h =
			   ---------------
			   - ae + afk - agj + ahi
			   - bek - bf + bgi + bhj
			   + cej - cfi - cg + chk
			   + dei + dfj + dgk + hh
   
			   (+ ah + bg - cf + de) i
			   (- ag + bh + ce + df) j
			   (+ af - be + ch + dg) k
			   (- ae - bf - cg + hh) */
			T a, b, c, d,
			        e, f, g, h;
			
			a = x;
			b = y;
			c = z; 
			d = w;
			e = other.x;
			f = other.y;
			g = other.z; 
			h = other.w;

			return Quat{
				+ a * h + b * g - c * f + d * e,
				- a * g + b * h + c * e + d * f,
				+ a * f - b * e + c * h + d * g,
				- a * e - b * f - c * g + d * h
			};
		}

		/**
		 * @brief Compute the hamilton product between
		 * this quaternion and another quaternion and
		 * update this quaternion in place.
		 * 
		 * @param other another quaternion
		 * @return ref to self 
		 */
		constexpr FORCE_INLINE Quat& operator*=(Quat const& other)
		{
			*this = (*this * other);
			return *this;
		}

		/**
		 * @brief Rotate a 3D vector around the axis
		 * and by the angle described by this quaternion.
		 * 
		 * @param v vector to rotate
		 * @return new vector
		 */
		constexpr FORCE_INLINE Vec3<T> operator*(Vec3<T> const& v) const
		{
			// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
			const Vec3<T> q{*this};
			const Vec3<T> t = 2.f * (q.cross(v));
			return v + (t * w) + (q.cross(t));
		}

		/**
		 * @brief Alias for operator*(Vec3<T> const&) const
		 */
		constexpr FORCE_INLINE Vec3<T> rotateVector(Vec3<T> const& v) const
		{
			return *this * v;
		}

	private:
		using Vec4<T>::x;
		using Vec4<T>::y;
		using Vec4<T>::z;
		using Vec4<T>::w;
		using Vec4<T>::Vec4;
	};
} // namespace Math
