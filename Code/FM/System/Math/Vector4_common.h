#pragma once

#include <utility>

#include "system/globalinclude.h"

namespace fm
{
	class Vector4 final
	{
		friend class Matrix;
	public:
		// Constants
		static const Vector4 Zero;
		static const Vector4 One;
		static const Vector4 UnitX;
		static const Vector4 UnitY;
		static const Vector4 UnitZ;
		static const Vector4 UnitW;

		FORCEINLINE Vector4() = default;
		FORCEINLINE explicit Vector4(const float v);
		FORCEINLINE Vector4(const float x, const float y, const float z, const float w);
		FORCEINLINE Vector4(const float* const data);

		FORCEINLINE float GetX() const;
		FORCEINLINE float GetY() const;
		FORCEINLINE float GetZ() const;
		FORCEINLINE float GetW() const;

		FORCEINLINE void SetX(const float x);
		FORCEINLINE void SetY(const float y);
		FORCEINLINE void SetZ(const float z);
		FORCEINLINE void SetW(const float w);
		FORCEINLINE void Set(const float x, const float y, const float z, const float w);

		// Comparision operators
		FORCEINLINE bool operator== (const Vector4& v) const;
		FORCEINLINE bool operator!= (const Vector4& v) const;

		// Assignment operators
		FORCEINLINE Vector4& operator= (const Vector4& v);
		FORCEINLINE Vector4 operator+ (const Vector4& v) const;
		FORCEINLINE Vector4 operator* (const float s) const;
		FORCEINLINE Vector4& operator+= (const Vector4& v);
		FORCEINLINE Vector4& operator-= (const Vector4& v);
		FORCEINLINE Vector4& operator*= (const Vector4& v);
		FORCEINLINE Vector4& operator*= (float s);
		FORCEINLINE Vector4& operator/= (float s);

		FORCEINLINE Vector4 Negate() const;

		// Vector operations
		FORCEINLINE bool InBounds(const Vector4& bounds) const;

		FORCEINLINE float Length() const;
		FORCEINLINE float LengthSquared() const;

		FORCEINLINE float Dot(const Vector4& v) const;
		FORCEINLINE Vector4 Cross(const Vector4& v1, const Vector4& v2) const;

		FORCEINLINE Vector4 Normalize() const;
		FORCEINLINE Vector4 Clamp(const Vector4& vMin, const Vector4& vMax) const;

		// Static functions
		FORCEINLINE static float Distance(const Vector4& v1, const Vector4& v2);
		FORCEINLINE static float DistanceSquared(const Vector4& v1, const Vector4& v2);

		FORCEINLINE static Vector4 Min(const Vector4& v1, const Vector4& v2);
		FORCEINLINE static Vector4 Max(const Vector4& v1, const Vector4& v2);

		FORCEINLINE static Vector4 Lerp(const Vector4& v1, const Vector4& v2, const float t);
		FORCEINLINE static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t);

		FORCEINLINE static Vector4 Reflect(const Vector4& v, const Vector4& n);

	private:
		Vector4Data mData;

		FORCEINLINE explicit Vector4::Vector4(Vector4Data&& data);
	};

	Vector4::Vector4(Vector4Data&& data)
		: mData(std::move(data))
	{
	}
}

