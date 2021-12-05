#pragma once

/**
 * For reference:
 * 
 * {1 0 0 Tx}   {Rxx Rxy Rxz 0}   {Sx  0  0 0}   {RxxSx RxySy RxzSz Tx}
 * {0 1 0 Ty}   {Ryx Ryy Ryz 0}   { 0 Sy  0 0}   {RyxSx RyySy RyzSz Ty}
 * {0 0 1 Tz} * {Rzx Rzy Rzz 0} * { 0  0 Sz 0} = {RzxSx RzySy RzzSz Tz}
 * {0 0 0  1}   {  0   0   0 1}   { 0  0  0 1}   {    0     0     0  1}
 * 
 * {Sx  0  0 0}   {Rxx Rxy Rxz 0}   {1 0 0 Tx}   {RxxSx RxySx RxzSx (RxxSxTx + RxySxTy + RxzSxTz)}
 * { 0 Sy  0 0}   {Ryx Ryy Ryz 0}   {0 1 0 Ty}   {RyxSy RyySy RyzSy (RyxSxTx + RyySxTy + RyzSxTz)}
 * { 0  0 Sz 0} * {Rzx Rzy Rzz 0} * {0 0 1 Tz} = {RzxSz RzySz RzzSz (RzxSxTx + RzySxTy + RzzSxTz)}
 * { 0  0  0 1}   {  0   0   0 1}   {0 0 0  1}   {    0     0     0 1}
 */

#include "math_types.h"
#include "quat.h"
#include "mat4.h"

namespace Math
{
	/**
	 * @brief A 4 by 4 matrix used to describe
	 * a combination of translation, rotation
	 * and scale transformations.
	 */
	struct TransformationMatrix : public Mat4<float32>
	{
		using Mat4::dot;

		/**
		 * @brief Construct a transformation with
		 * zero-translation, zero-rotation and
		 * unit-scale.
		 */
		constexpr FORCE_INLINE TransformationMatrix()
			: Mat4{Mat4::diag(1.f)} // TODO: Replace with eye when possible
		{
			//
		}

		/**
		 * @brief Construct a transformation with
		 * given translation, rotation and scale.
		 * 
		 * @param inTranslation translation vector
		 * @param inRotation rotation quaternion
		 * @param inScale scale vector
		 */
		constexpr TransformationMatrix(Vec3<float32> const& inTranslation, Quat<float32> const& inRotation, Vec3<float32> const& inScale = 1.f)
			: TransformationMatrix{}
		{
			// Set rotation
			const float32 rotX2 = inRotation.x * inRotation.x,
			              rotXY = inRotation.x * inRotation.y,
						  rotXZ = inRotation.x * inRotation.z,
						  rotXW = inRotation.x * inRotation.w,
						  rotY2 = inRotation.y * inRotation.y,
						  rotYZ = inRotation.y * inRotation.z,
						  rotYW = inRotation.y * inRotation.w,
						  rotZ2 = inRotation.z * inRotation.z,
						  rotZW = inRotation.z * inRotation.w;
			
			// Rotation
			data[0][0] = 1.f - 2.f * (rotY2 + rotZ2);
			data[0][1] = 2.f * (rotXY - rotZW);
			data[0][2] = 2.f * (rotXZ + rotYW);
			data[1][0] = 2.f * (rotXY + rotZW);
			data[1][1] = 1.f - 2.f * (rotX2 + rotZ2);
			data[1][2] = 2.f * (rotYZ - rotXW);
			data[2][0] = 2.f * (rotXZ - rotYW);
			data[2][1] = 2.f * (rotYZ + rotXW);
			data[2][2] = 1.f - 2.f * (rotX2 + rotY2);

			// Scale
			for (int32 i = 0; i < 3; ++i)
			{
				data[i][0] *= inScale.x;
				data[i][1] *= inScale.y;
				data[i][2] *= inScale.z;
			}

			// Translation
			data[0][3] = inTranslation.x;
			data[1][3] = inTranslation.y;
			data[2][3] = inTranslation.z;
		}

		/**
		 * @brief Construct a transformation with
		 * given translation, zero-rotation and
		 * unit scale.
		 * 
		 * @param inTranslation translation vector
		 */
		constexpr FORCE_INLINE TransformationMatrix(Vec3<float32> const& inTranslation)
			: Mat4{1.f, 0.f, 0.f, inTranslation.x,
			       0.f, 1.f, 0.f, inTranslation.y,
			       0.f, 0.f, 1.f, inTranslation.z,
			       0.f, 0.f, 0.f, 1.f}
		{
			//
		}

		/**
		 * @brief Returns the read-only translation
		 * component of the transformation.
		 */
		constexpr FORCE_INLINE Vec3<float32> getTranslation() const
		{
			return {data[0][3], data[1][3], data[2][3]};
		}

		/**
		 * @brief Returns the read-only rotation
		 * component of the transformation.
		 */
		constexpr FORCE_INLINE Quat<float32> getRotation() const
		{
			// Get transform scale
			Vec3<float32> const invscale = 1.f / getScale();

			// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
			float32 const quatW = PlatformMath::sqrt(1.f + data[0][0] * invscale[0] + data[1][1] * invscale[1] + data[2][2] * invscale[2]) * 0.5f;
			float32 const quatX = (data[2][1] * invscale[1] - data[1][2] * invscale[2]) / (4.f * quatW);
			float32 const quatY = (data[0][2] * invscale[2] - data[2][0] * invscale[0]) / (4.f * quatW);
			float32 const quatZ = (data[1][0] * invscale[0] - data[0][1] * invscale[1]) / (4.f * quatW);

			return Quat<float32>{quatX, quatY, quatZ, quatW};
		}

		/**
		 * @brief Returns the read-only scale
		 * component of the transformation.
		 */
		constexpr FORCE_INLINE Vec3<float32> getScale() const
		{
			const float32 scaleX = PlatformMath::sqrt(rows[0].x * rows[0].x + rows[1].x * rows[1].x + rows[2].x * rows[2].x);
			const float32 scaleY = PlatformMath::sqrt(rows[0].y * rows[0].y + rows[1].y * rows[1].y + rows[2].y * rows[2].y);
			const float32 scaleZ = PlatformMath::sqrt(rows[0].z * rows[0].z + rows[1].z * rows[1].z + rows[2].z * rows[2].z);

			return {scaleX, scaleY, scaleZ};	
		}

		/**
		 * @brief Set the translation component
		 * of this transformation.
		 * 
		 * @param newTranslation new translation
		 * vector
		 * @return ref to self
		 */
		constexpr FORCE_INLINE TransformationMatrix& setTranslation(Vec3<float32> const& newTranslation)
		{
			data[0][3] = newTranslation.x;
			data[1][3] = newTranslation.y;
			data[2][3] = newTranslation.z;

			return *this;
		}

		/**
		 * @brief Set the rotation component
		 * of this transformation.
		 * 
		 * @param newRotation new rotation
		 * quaternion
		 * @return ref to self
		 */
		constexpr FORCE_INLINE TransformationMatrix& setRotation(Quat<float32> const& newRotation)
		{
			Vec3<float32> scale = getScale();

			// Set rotation
			const float32 rotX2 = newRotation.x * newRotation.x,
			              rotXY = newRotation.x * newRotation.y,
						  rotXZ = newRotation.x * newRotation.z,
						  rotXW = newRotation.x * newRotation.w,
						  rotY2 = newRotation.y * newRotation.y,
						  rotYZ = newRotation.y * newRotation.z,
						  rotYW = newRotation.y * newRotation.w,
						  rotZ2 = newRotation.z * newRotation.z,
						  rotZW = newRotation.z * newRotation.w,
						  rotW2 = newRotation.w * newRotation.w;
			
			data[0][0] = scale.x * 1.f - 2.f * (rotY2 + rotZ2);
			data[0][1] = scale.y * 2.f * (rotXY - rotZW);
			data[0][2] = scale.z * 2.f * (rotXZ + rotYW);
			data[1][0] = scale.x * 2.f * (rotXY + rotZW);
			data[1][1] = scale.y * 1.f - 2.f * (rotX2 + rotZ2);
			data[1][2] = scale.z * 2.f * (rotYZ - rotXW);
			data[2][0] = scale.x * 2.f * (rotXZ - rotYW);
			data[2][1] = scale.y * 2.f * (rotYZ + rotXW);
			data[2][2] = scale.z * 1.f - 2.f * (rotX2 + rotY2);

			return *this;
		}

		/**
		 * @brief Set the scale component of
		 * this transformation
		 * 
		 * @param newScale new scale vector
		 * @return ref to self
		 */
		constexpr FORCE_INLINE TransformationMatrix& setScale(Vec3<float32> const& newScale)
		{
			Vec3<float32> s = newScale / getScale();
			for (int32 i = 0; i < 3; ++i)
			{
				data[i][0] *= s.x;
				data[i][1] *= s.y;
				data[i][2] *= s.z;
			}

			return *this;
		}

		/**
		 * @brief Overloaded dot product between two
		 * linear transformation matrices.
		 * 
		 * The resulting matrix too describes a
		 * transformation.
		 * 
		 * @param other another transformation matrix
		 * @return new tranformation matrix
		 */
		constexpr FORCE_INLINE TransformationMatrix dot(TransformationMatrix const& other) const
		{
			return reinterpret_cast<TransformationMatrix&&>(Mat4::dot(other));
		}

		/**
		 * @brief Apply the transformation to a
		 * 3D vector.
		 * 
		 * @param v vector to transform
		 * @return transformed vector 
		 */
		constexpr FORCE_INLINE Vec3<float32> transformVector(Vec3<float32> const& v) const
		{
			return {
				rows[0].dot({v, 1.f}),
				rows[1].dot({v, 1.f}),
				rows[2].dot({v, 1.f})
			};
		}

		/**
		 * @brief Returns a new matrix that describes
		 * the inverse transformation. 
		 */
		constexpr FORCE_INLINE TransformationMatrix operator!() const
		{
			return TransformationMatrix{*this}.invert();
		}

		/**
		 * @brief Invert this transformation matrix
		 * in place.
		 * 
		 * @return ref to self
		 */
		constexpr FORCE_INLINE TransformationMatrix& invert()
		{
			Vec3<float32> const invTranslation = -getTranslation();

			// Invert scale
			float32 const invScaleX2 = 1.f / (rows[0].x * rows[0].x + rows[1].x * rows[1].x + rows[2].x * rows[2].x);
			float32 const invScaleY2 = 1.f / (rows[0].y * rows[0].y + rows[1].y * rows[1].y + rows[2].y * rows[2].y);
			float32 const invScaleZ2 = 1.f / (rows[0].z * rows[0].z + rows[1].z * rows[1].z + rows[2].z * rows[2].z);

			for (int32 i = 0; i < 3; ++i)
			{
				data[i][0] *= invScaleX2;
				data[i][1] *= invScaleY2;
				data[i][2] *= invScaleZ2;
			}

			// Transpose rotation matrix
			swap(data[0][1], data[1][0]);
			swap(data[0][2], data[2][0]);
			swap(data[1][2], data[2][1]);

			// Compute inverse translation
			data[0][3] = invTranslation.dot(rows[0]);
			data[1][3] = invTranslation.dot(rows[1]);
			data[2][3] = invTranslation.dot(rows[2]);

			return *this;
		}

	protected:
		using Mat4::data;
		using Mat4::rows;
		using Mat4::Mat4;
	};

	/**
	 * @brief A 4 by 4 matrix used to describe
	 * a combination of translation and rotation.
	 */
	struct TranslationAndRotationMatrix : public TransformationMatrix
	{
		using TransformationMatrix::TransformationMatrix;
		using TransformationMatrix::dot;

		/**
		 * @copydoc TransformationMatrix::getRotation() const
		 */
		constexpr FORCE_INLINE Quat<float32> getRotation() const
		{
			// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
			float32 const quatW = PlatformMath::sqrt(1.f + data[0][0] + data[1][1] + data[2][2]) * 0.5f;
			float32 const invQuatW = 0.25f / quatW;
			float32 const quatX = (data[2][1] - data[1][2]) * invQuatW;
			float32 const quatY = (data[0][2] - data[2][0]) * invQuatW;
			float32 const quatZ = (data[1][0] - data[0][1]) * invQuatW;

			return Quat<float32>{quatX, quatY, quatZ, quatW};
		}

		/**
		 * @copydoc TransformationMatrix::setRotation(Quat<float32> const&) 
		 */
		constexpr FORCE_INLINE TranslationAndRotationMatrix& setRotation(Quat<float32> const& newRotation)
		{
			const float32 rotX2 = newRotation.x * newRotation.x,
			              rotXY = newRotation.x * newRotation.y,
						  rotXZ = newRotation.x * newRotation.z,
						  rotXW = newRotation.x * newRotation.w,
						  rotY2 = newRotation.y * newRotation.y,
						  rotYZ = newRotation.y * newRotation.z,
						  rotYW = newRotation.y * newRotation.w,
						  rotZ2 = newRotation.z * newRotation.z,
						  rotZW = newRotation.z * newRotation.w,
						  rotW2 = newRotation.w * newRotation.w;
			
			data[0][0] = 1.f - 2.f * (rotY2 + rotZ2);
			data[0][1] = 2.f * (rotXY - rotZW);
			data[0][2] = 2.f * (rotXZ + rotYW);
			data[1][0] = 2.f * (rotXY + rotZW);
			data[1][1] = 1.f - 2.f * (rotX2 + rotZ2);
			data[1][2] = 2.f * (rotYZ - rotXW);
			data[2][0] = 2.f * (rotXZ - rotYW);
			data[2][1] = 2.f * (rotYZ + rotXW);
			data[2][2] = 1.f - 2.f * (rotX2 + rotY2);

			return *this;
		}
		
		/**
		 * @brief Overload the dot product between two
		 * translation and rotation only matrices.
		 * 
		 * The resulting matrix too describes a
		 * transformation with unit scale.
		 * 
		 * @param other another translation and rotation
		 * matrix
		 * @return new translation and rotation matrix
		 */
		constexpr FORCE_INLINE TranslationAndRotationMatrix dot(TranslationAndRotationMatrix& other) const
		{
			return reinterpret_cast<TranslationAndRotationMatrix&&>(TransformationMatrix::dot(other));
		}

		/**
		 * @copydoc TransformationMatrix::operator!() const
		 */
		constexpr FORCE_INLINE TranslationAndRotationMatrix operator!() const
		{
			return TranslationAndRotationMatrix{*this}.invert();
		}

		/**
		 * @copydoc TransformationMatrix::invert()
		 */
		constexpr FORCE_INLINE TranslationAndRotationMatrix& invert()
		{
			Vec3<float32> const invTranslation = -getTranslation();

			// Transpose rotation matrix
			swap(data[0][1], data[1][0]);
			swap(data[0][2], data[2][0]);
			swap(data[1][2], data[2][1]);

			// Compute inverse translation
			data[0][3] = invTranslation.dot(rows[0]);
			data[1][3] = invTranslation.dot(rows[1]);
			data[2][3] = invTranslation.dot(rows[2]);

			return *this;
		}
		
	protected:
		using TransformationMatrix::data;
		using TransformationMatrix::rows;

	private:
		TranslationAndRotationMatrix(Vec3<float32> const&, Quat<float32> const&, Vec3<float32> const&) = delete;
		TransformationMatrix& setScale(Vec3<float32> const&) = delete;
		Vec3<float32> getScale() const = delete;
	};

	/**
	 * @brief A 3 by 3 matrix used to describe
	 * 3D rotations.
	 */
	struct RotationMatrix : public Mat3<float32>
	{
		/**
		 * @brief Construct a new matrix with zero-rotation.
		 */
		constexpr FORCE_INLINE RotationMatrix()
			: Mat3{1.f, 0.f, 0.f,
			       0.f, 1.f, 0.f,
				   0.f, 0.f, 1.f}
		{
			//
		}

		/**
		 * @brief Contruct a rotation matrix with the given
		 * rotation.
		 * 
		 * @param inRotation rotation quaternion
		 */
		constexpr FORCE_INLINE RotationMatrix(Quat<float32> const& inRotation)
			: Mat3{}
		{
			setRotation(inRotation);
		}

		/**
		 * @brief Returns the rotation component as a
		 * quaternion.
		 */
		constexpr Quat<float32> getRotation() const
		{
			// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
			float32 const quatW = PlatformMath::sqrt(1.f + data[0][0] + data[1][1] + data[2][2]) * 0.5f;
			float32 const invQuatW = 1.f / (quatW / 4.f);
			float32 const quatX = (data[2][1] - data[1][2]) * invQuatW;
			float32 const quatY = (data[0][2] - data[2][0]) * invQuatW;
			float32 const quatZ = (data[1][0] - data[0][1]) * invQuatW;

			return Quat<float32>{quatX, quatY, quatZ, quatW};
		}

		/**
		 * @brief Update the rotation component.
		 * 
		 * @param newRotation new rotation quaternion
		 * @return ref to self
		 */
		constexpr RotationMatrix& setRotation(Quat<float32> const& newRotation)
		{
			const float32 rotX2 = newRotation.x * newRotation.x,
			              rotXY = newRotation.x * newRotation.y,
						  rotXZ = newRotation.x * newRotation.z,
						  rotXW = newRotation.x * newRotation.w,
						  rotY2 = newRotation.y * newRotation.y,
						  rotYZ = newRotation.y * newRotation.z,
						  rotYW = newRotation.y * newRotation.w,
						  rotZ2 = newRotation.z * newRotation.z,
						  rotZW = newRotation.z * newRotation.w,
						  rotW2 = newRotation.w * newRotation.w;
			
			data[0][0] = 1.f - 2.f * (rotY2 + rotZ2);
			data[0][1] = 2.f * (rotXY - rotZW);
			data[0][2] = 2.f * (rotXZ + rotYW);
			data[1][0] = 2.f * (rotXY + rotZW);
			data[1][1] = 1.f - 2.f * (rotX2 + rotZ2);
			data[1][2] = 2.f * (rotYZ - rotXW);
			data[2][0] = 2.f * (rotXZ - rotYW);
			data[2][1] = 2.f * (rotYZ + rotXW);
			data[2][2] = 1.f - 2.f * (rotX2 + rotY2);

			return *this;
		}

		/**
		 * @brief Overload the dot product to return
		 * a rotation matrix.
		 * 
		 * The resulting matrix will describe a
		 * rotation equal to applying the original
		 * rotations.
		 * 
		 * (R * Q) * v = (R * (Q * v))
		 * 
		 * @param other another rotation matrix
		 * @return new rotation matrix
		 */
		constexpr FORCE_INLINE RotationMatrix dot(RotationMatrix const& other) const
		{
			return reinterpret_cast<RotationMatrix&&>(Mat3::dot(other));
		}

		/**
		 * @brief Rotate a 3D vector.
		 * 
		 * @param v vector to rotate
		 * @return transformed vector 
		 */
		constexpr FORCE_INLINE Vec3<float32> rotateVector(Vec3<float32> const& v) const
		{
			return Mat3::dot(v);
		}

		/**
		 * @brief Invert this rotation matrix in place.
		 */
		constexpr FORCE_INLINE RotationMatrix& invert()
		{
			// Simply transpose rotation matrix
			transpose();
			return *this;
		}

		/**
		 * @brief Returns a copy of the inverse matrix.
		 * 
		 * The inverted matrix describes the opposite
		 * rotation.
		 */
		constexpr FORCE_INLINE RotationMatrix operator!() const
		{
			return RotationMatrix{*this}.invert();
		}

		/**
		 * @brief Returns a transformation matrix with
		 * zero-translation and rotation equal to the
		 * rotation of this matrix.
		 */
		constexpr FORCE_INLINE operator TranslationAndRotationMatrix() const
		{
			return reinterpret_cast<TranslationAndRotationMatrix&&>(static_cast<Mat4<float32>>(*this));
		}
	};
} // namespace Math
