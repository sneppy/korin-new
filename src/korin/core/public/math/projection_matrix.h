#pragma once

#include "math_types.h"

namespace Math
{
	/**
	 * @brief This matrix implements a perspectice projection transformation.
	 */
	struct PerspectiveProjectionMatrix : Mat4<float32>
	{
		using Mat4::operator[];

		/**
		 * @brief Construct a new perspective projection from screen
		 * coordainates (e.g. right, left, top, bottom).
		 * 
		 * @param r,l right and left screen coordinates
		 * @param t,b top and bottom screen coordinates
		 * @param f,n the distance from the camera of the far and near planes
		 */
		constexpr PerspectiveProjectionMatrix(float32 r, float32 l, float32 t, float32 b, float32 f, float32 n)
			: Mat4{2.f * n / (r - l), 0.f, -(r + l) / (r - l), 0.f,
			       0.f, 2.f * n / (t - b), -(t + b) / (t - b), 0.f,
				   0.f, 0.f, -(n + f) / (n - f), 2.f * n * f / (n - f),
				   0.f, 0.f, 1.f, 0.f}
		{
			//
		}

		/**
		 * @brief Construct a new perpective projection with the given field of
		 * view and projection ratio.
		 * 
		 * @param fov the field of view angle in radians
		 * @param ratio the projection width-to-height ratio (e.g. 16:9)
		 * @param f,n the distance from the camera of the far and near planes
		 */
		constexpr PerspectiveProjectionMatrix(float32 fov, float32 ratio, float32 f, float32 n)
			: PerspectiveProjectionMatrix{0, 1.f / PlatformMath::tan(fov / 2.f), 1.f / ratio, f, n}
		{
			//
		}

		/**
		 * @brief Construct a new perspective projection with 90 deg FOV and
		 * 16:9 projection ratio.
		 */
		constexpr PerspectiveProjectionMatrix()
			: PerspectiveProjectionMatrix{PlatformMath::degToRad(90.f), 16.f / 9.f, 1000.f, 0.5f}
		{
			//
		}

	private:
		/**
		 * @brief Internal use only.
		 * 
		 * @param _ unused
		 * @param nr the ratio between the right screen coordinate and the
		 * distance of the near plane. This is equal to the inverse tan of half
		 * the FOV angle.
		 * @param ratio the projection width-to-height ratio
		 * @param f,n the distance from the camera of the far and near planes
		 */
		constexpr PerspectiveProjectionMatrix(int _, float32 nr, float32 ratio, float32 f, float32 n)
			: Mat4{nr, 0.f, 0.f, 0.f,
			       0.f, nr * ratio, 0.f, 0.f,
				   0.f, 0.f, -(n + f) / (n - f), 2.f * n * f / (n - f),
				   0.f, 0.f, 1.f, 0.f}
		{
			//
		}
	};
} // namespace Math
