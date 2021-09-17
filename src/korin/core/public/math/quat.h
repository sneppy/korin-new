#pragma once

#include "math_types.h"
#include "vec4.h"

namespace Math
{
	struct Quat : protected Vec4<float32>
	{
		using Super = Vec4<float32>;

		constexpr FORCE_INLINE Quat()
			: Vec4{0.f, 0.f, 0.f, 1.f}
		{
			//
		}

		constexpr FORCE_INLINE Quat(Vec3<float32> const& axis, float32 angle)
		{
			
		}

		constexpr FORCE_INLINE Quat(Vec3<float32> const& phyrot);
		constexpr FORCE_INLINE Vec3<float32> getAxis() const;
		constexpr FORCE_INLINE float32 getAngle() const;
		constexpr FORCE_INLINE void getAxisAndAngle(Vec3<float32>& outAxis, float32 outAngle);
		constexpr FORCE_INLINE Quat operator!() const;
		constexpr FORCE_INLINE Quat& operator*=(Quat const& other);
		constexpr FORCE_INLINE Quat operator*(Quat const& other) const;
		constexpr FORCE_INLINE Vec3<float32> operator*(Vec3<float32> const& v) const;
		constexpr FORCE_INLINE Vec3<float32> rotateVector(Vec3<float32> const& v) const;

	private:
		using Vec4::x;
		using Vec4::y;
		using Vec4::z;
		using Vec4::w;
	};
} // namespace Math

