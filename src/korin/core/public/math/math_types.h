#pragma once

#include "core_types.h"
#include "hal/platform_math.h"

namespace Math
{
	template<typename> struct Vec2;
	template<typename> struct Vec3;
	template<typename> struct Vec4;
	template<typename> struct Mat3;
	template<typename> struct Mat4;
	template<typename> struct Quat;
	                   struct TransformationMatrix;
	                   struct TranslationAndRotationMatrix;
	                   struct RotationMatrix;
	                   struct PerspectiveProjectionMatrix;
	                   struct OrthographicProjectionMatrix;
} // namespace Math

using vec2 = Math::Vec2<float32>;
using vec3 = Math::Vec3<float32>;
using vec4 = Math::Vec4<float32>;

using float2 = Math::Vec2<float32>;
using float3 = Math::Vec3<float32>;
using float4 = Math::Vec4<float32>;

using int2 = Math::Vec2<int32>;
using int3 = Math::Vec3<int32>;
using int4 = Math::Vec4<int32>;

using uint2 = Math::Vec2<uint32>;
using uint3 = Math::Vec3<uint32>;
using uint4 = Math::Vec4<uint32>;

using mat3 = Math::Mat3<float32>;
using mat4 = Math::Mat4<float32>;

using quat = Math::Quat<float32>;
