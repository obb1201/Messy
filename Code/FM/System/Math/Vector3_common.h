#pragma once

#include <utility>
#include "system/globalinclude.h"

namespace fm
{
	class Vector3 final
	{
		friend class Matrix;
		friend class Quaternion;
	public:
		// Constants
		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 UnitX;
		static const Vector3 UnitY;
		static const Vector3 UnitZ;

		FORCEINLINE Vector3() = default;
		FORCEINLINE explicit Vector3(const float v);
		FORCEINLINE Vector3(const float x, const float y, const float z);
		FORCEINLINE Vector3(const float* data);

		FORCEINLINE float GetX() const;
		FORCEINLINE float GetY() const;
		FORCEINLINE float GetZ() const;

		FORCEINLINE void SetX(const float x);
		FORCEINLINE void SetY(const float y);
		FORCEINLINE void SetZ(const float z);
		FORCEINLINE void Set(const float x, const float y, const float z);

		// Comparision operators
		FORCEINLINE bool operator== (const Vector3& v) const;
		FORCEINLINE bool operator!= (const Vector3& v) const;

		// Assignment operators
		FORCEINLINE Vector3& operator= (const Vector3& v);
		FORCEINLINE Vector3 operator+ (const Vector3& v) const;
		FORCEINLINE Vector3 operator- (const Vector3& v) const;
		FORCEINLINE Vector3 operator* (const float s) const;
		FORCEINLINE Vector3 operator/ (const float s) const;
		FORCEINLINE Vector3& operator+= (const Vector3& v);
		FORCEINLINE Vector3& operator-= (const Vector3& v);
		FORCEINLINE Vector3& operator*= (const Vector3& v);
		FORCEINLINE Vector3& operator*= (const float s);
		FORCEINLINE Vector3& operator/= (const float s);

		FORCEINLINE Vector3 Negate() const;

		// Vector operations
		FORCEINLINE bool InBounds(const Vector3& bounds) const;

		FORCEINLINE float Length() const;
		FORCEINLINE float LengthSquared() const;

		FORCEINLINE float Dot(const Vector3& v) const;
		FORCEINLINE Vector3 Cross(const Vector3& v) const;

		FORCEINLINE Vector3 Normalize() const;
		FORCEINLINE Vector3 Clamp(const Vector3& vMin, const Vector3& vMax) const;

		// Static functions
		FORCEINLINE static float Distance(const Vector3& v1, const Vector3& v2);
		FORCEINLINE static float DistanceSquared(const Vector3& v1, const Vector3& v2);

		FORCEINLINE static Vector3 Min(const Vector3& v1, const Vector3& v2);
		FORCEINLINE static Vector3 Max(const Vector3& v1, const Vector3& v2);

		FORCEINLINE static Vector3 Lerp(const Vector3& v1, const Vector3& v2, const float t);
		FORCEINLINE static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t);

		FORCEINLINE static Vector3 Reflect(const Vector3& v, const Vector3& n);

		// Binary operators for Vector3
	private:
		Vector3Data mData;

		FORCEINLINE explicit Vector3::Vector3(Vector3Data&& data);
	};

	Vector3::Vector3(Vector3Data&& data)
		: mData(std::move(data))
	{
	}
}