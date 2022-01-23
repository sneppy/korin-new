#pragma once

/**
 * For reference:
 *
 * {1 0 0 Tx}   {Rxx Rxy Rxz 0}   {Rxx Rxy Rxz Tx}
 * {0 1 0 Ty}   {Ryx Ryy Ryz 0}   {Ryx Ryy Ryz Ty}
 * {0 0 1 Tz} * {Rzx Rzy Rzz 0} = {Rzx Rzy Rzz Tz}
 * {0 0 0  1}   {  0   0   0 1}   {  0   0   0  1}
 *
 * {Rxx Rxy Rxz 0}   {1 0 0 Tx}   {Rxx Rxy Rxz (RxxTx + RxyTy + RxzTz)}
 * {Ryx Ryy Ryz 0}   {0 1 0 Ty}   {Ryx Ryy Ryz (RyxTx + RyyTy + RyzTz)}
 * {Rzx Rzy Rzz 0} * {0 0 1 Tz} = {Rzx Rzy Rzz (RzxTx + RzyTy + RzzTz)}
 * {  0   0   0 1}   {0 0 0  1}   {  0   0   0                       1}
 */

#include "math_types.h"
#include "mat4.h"

namespace Math
{
	/**
	 * @brief A view matrix is a 4 by 4 matrix
	 * that describes a linear transformation
	 * that transforms position from local space
	 * to world space.
	 *
	 * Conceptually, the view matrix is just a
	 * transformation matrix that first applies
	 * a rotation and then a translation.
	 *
	 * The rotation is equal to the inverse
	 * rotation of the view, and the translation
	 * is the opposite of the view position,
	 * meaning that a cube centered and aligned
	 * with the view would be transformed in a
	 * cube centered at the origin and aligned
	 * with the axes.
	 */
	struct ViewMatrix : public Mat4<float32>
	{
		/**
		 * @brief Construct a view matrix for a camera
		 * centered in the origin facing right.
		 */
		constexpr FORCE_INLINE ViewMatrix()
			: Mat4{Mat4::diag(1.f)}
		{
			//
		}

		/**
		 * @brief Construct a view matrix with the
		 * given position and rotation.
		 *
		 * @param viewPos world position of the view
		 * @param viewRot rotation of the view (right
		 * is zero)
		 */
		constexpr ViewMatrix(Vec3<float32> const& viewPos, Quat<float32> const& viewRot)
			: ViewMatrix{}
		{
			Vec3<float32> const translation = -viewPos;
			Quat<float32> const rotation = !viewRot;

			// Set rotation
			const float32 rotX2 = rotation.x * rotation.x,
			              rotXY = rotation.x * rotation.y,
						  rotXZ = rotation.x * rotation.z,
						  rotXW = rotation.x * rotation.w,
						  rotY2 = rotation.y * rotation.y,
						  rotYZ = rotation.y * rotation.z,
						  rotYW = rotation.y * rotation.w,
						  rotZ2 = rotation.z * rotation.z,
						  rotZW = rotation.z * rotation.w;

			data[0][0] = 1.f - 2.f * (rotY2 + rotZ2);
			data[0][1] = 2.f * (rotXY - rotZW);
			data[0][2] = 2.f * (rotXZ + rotYW);
			data[1][0] = 2.f * (rotXY + rotZW);
			data[1][1] = 1.f - 2.f * (rotX2 + rotZ2);
			data[1][2] = 2.f * (rotYZ - rotXW);
			data[2][0] = 2.f * (rotXZ - rotYW);
			data[2][1] = 2.f * (rotYZ + rotXW);
			data[2][2] = 1.f - 2.f * (rotX2 + rotY2);

			// Set translation
			data[0][3] = translation.dot(rows[0]);
			data[1][3] = translation.dot(rows[1]);
			data[2][3] = translation.dot(rows[2]);
		}

		/**
		 * @brief Construct a view matrix with the
		 * given view position, facing right.
		 *
		 * @param viewPos world position of the view
		 */
		constexpr FORCE_INLINE ViewMatrix(Vec3<float32> const& viewPos)
			: Mat4{1.f, 0.f, 0.f, -viewPos.x,
			       0.f, 1.f, 0.f, -viewPos.y,
				   0.f, 0.f, 1.f, -viewPos.z,
				   0.f, 0.f, 0.f, 1.f}
		{
			//
		}

		/**
		 * @brief Returns the position of the view.
		 *
		 * This is not the same value as the translation
		 * of a regular transformation matrix.
		 */
		constexpr Vec3<float32> getViewPosition() const
		{
			Vec3<float32> const translation = {data[0][3], data[1][3], data[2][3]};
			Mat4 const inverseRotation = getTransposed();

			return {
				-translation.dot(inverseRotation.rows[0]),
				-translation.dot(inverseRotation.rows[1]),
				-translation.dot(inverseRotation.rows[2])
			};
		}

		/**
		 * @brief Returns the rotation of the view.
		 */
		constexpr FORCE_INLINE Quat<float32> getViewRotation() const
		{
			// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
			float32 const quatW = PlatformMath::sqrt(1.f + data[0][0] + data[1][1] + data[2][2]) * 0.5f;
			float32 const invQuatW = 0.25f / -quatW; // Mind the minus
			float32 const quatX = (data[2][1] - data[1][2]) * invQuatW;
			float32 const quatY = (data[0][2] - data[2][0]) * invQuatW;
			float32 const quatZ = (data[1][0] - data[0][1]) * invQuatW;

			return Quat<float32>{quatX, quatY, quatZ, quatW};
		}

		/**
		 * @brief Update the view position without
		 * changing its rotation.
		 *
		 * @param newViewPos new world position
		 * @return ref to self
		 */
		constexpr FORCE_INLINE ViewMatrix& setViewPosition(Vec3<float32> const& newViewPos)
		{
			Vec3<float32> translation = -newViewPos;
			data[0][3] = translation.dot(rows[0]);
			data[1][3] = translation.dot(rows[1]);
			data[2][3] = translation.dot(rows[2]);

			return *this;
		}

		constexpr ViewMatrix& setViewRotation(Quat<float32> const& newViewRot);
		/**
		 * @brief Overload dot operator between two
		 * view matrices.
		 *
		 * The resulting matrix is a view matrix as
		 * well.
		 *
		 * @param other another view matrix
		 * @return new view matrix
		 */
		constexpr FORCE_INLINE ViewMatrix dot(ViewMatrix const& other) const
		{
			return reinterpret_cast<ViewMatrix&&>(Mat4::dot(other));
		}

		/**
		 * @brief Apply the transformation to a 3D
		 * position.
		 *
		 * @param pos the world position
		 * @return transformed position
		 */
		constexpr FORCE_INLINE Vec3<float32> transformPosition(Vec3<float32> const& pos) const
		{
			return {
				rows[0].dot({pos, 1.f}),
				rows[1].dot({pos, 1.f}),
				rows[2].dot({pos, 1.f})
			};
		}

		constexpr FORCE_INLINE Vec3<float32> transformNormal(Vec3<float32> const& pos) const;
		/**
		 * @brief Returns a copy of the inverse
		 * view matrix.
		 */
		constexpr FORCE_INLINE ViewMatrix operator!() const
		{
			return ViewMatrix{*this}.invert();
		}

		/**
		 * @brief Invert matrix in place.
		 *
		 * The inverted matrix is still a view matrix.
		 *
		 * @return ref to self
		 */
		constexpr FORCE_INLINE ViewMatrix& invert()
		{
			Vec3<float32> translation = {data[0][3], data[1][3], data[2][3]};

			// Transpose rotation matrix
			swap(data[0][1], data[1][0]);
			swap(data[0][2], data[2][0]);
			swap(data[1][2], data[2][1]);

			// Compute camera position
			data[0][3] = -translation.dot(rows[0]);
			data[1][3] = -translation.dot(rows[1]);
			data[2][3] = -translation.dot(rows[2]);

			return *this;
		}

	protected:
		using Mat4::data;
		using Mat4::rows;
	};
} // namespace Math
