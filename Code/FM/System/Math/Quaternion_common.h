#pragma once

#include <utility>
#include "System/GlobalInclude.h"

namespace fm
{
	class Vector3;
	class Vector4;
	class Matrix;

	class Quaternion final
	{
		friend class Matrix;
	public:
		static const Quaternion Identity;

		FORCEINLINE Quaternion() = default;
		FORCEINLINE explicit Quaternion(const float v);
		FORCEINLINE Quaternion(const float x, const float y, const float z, const float w);
		FORCEINLINE Quaternion(const Vector4& v);
		FORCEINLINE Quaternion(const Matrix& m);
		FORCEINLINE Quaternion(const Vector3& axis, const float angleRadian);
		FORCEINLINE Quaternion(const float* data);

		FORCEINLINE Quaternion operator+ (const Quaternion& q) const;
		FORCEINLINE Quaternion& operator+= (const Quaternion& q);
		FORCEINLINE Quaternion operator- (const Quaternion& q) const;
		FORCEINLINE Quaternion& operator-= (const Quaternion& q);
		FORCEINLINE Quaternion operator* (const Quaternion& q) const;
		FORCEINLINE Quaternion& operator*= (const Quaternion& q);
		FORCEINLINE Quaternion operator* (const float s) const;
		FORCEINLINE Quaternion& operator*= (const float s);
		FORCEINLINE Quaternion operator/ (const float s) const;
		FORCEINLINE Quaternion& operator/= (const float s);

		FORCEINLINE bool operator== (const Quaternion& q) const;
		FORCEINLINE bool operator!= (const Quaternion& q) const;

		FORCEINLINE float Length() const;
		FORCEINLINE float LengthSquared() const;
		FORCEINLINE float Dot(const Quaternion& q) const;
		FORCEINLINE Quaternion Normalize() const;
		FORCEINLINE Quaternion Invert() const;

		FORCEINLINE void ToAxisAndAngle(Vector3* const outAxis, float* const outAngleRadian) const;
		FORCEINLINE void FromAxisAndAngle(Vector3& axis, const float angleRadian);
		FORCEINLINE Matrix ToRotation() const;
		FORCEINLINE void FromRotation(const Matrix& rotation);

		FORCEINLINE static Quaternion SphericalLerp(const Quaternion& q1, const Quaternion& q2, const float t);
		FORCEINLINE static Quaternion SphericalSmoothStep(const Quaternion& q1, const Quaternion& q2, float t);

	private:
		QuaternionData mData;

		FORCEINLINE explicit Quaternion(QuaternionData&& data)
			: mData(std::move(data))
		{

		}
	};
}