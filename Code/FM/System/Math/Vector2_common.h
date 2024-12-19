#pragma once

#include <utility>
#include "System/GlobalInclude.h"

namespace fm
{
	class Vector2 final
	{
	public:
		static const Vector2 Zero;
		static const Vector2 One;
		static const Vector2 UnitX;
		static const Vector2 UnitY;

		FORCEINLINE Vector2() = default;
		FORCEINLINE explicit Vector2(const float v);
		FORCEINLINE Vector2(const float x, const float y);
		FORCEINLINE Vector2(const float* data);

		FORCEINLINE float GetX() const;
		FORCEINLINE float GetY() const;

		FORCEINLINE void SetX(float x);
		FORCEINLINE void SetY(float y);
		FORCEINLINE void Set(float x, float y);

		// Comparision operators
		FORCEINLINE bool operator== (const Vector2& v) const;
		FORCEINLINE bool operator!= (const Vector2& v) const;

		// Assignment operators
		FORCEINLINE Vector2& operator= (const Vector2& v);
		FORCEINLINE Vector2& operator+= (const Vector2& v);
		FORCEINLINE Vector2& operator-= (const Vector2& v);
		FORCEINLINE Vector2& operator*= (const Vector2& v);
		FORCEINLINE Vector2& operator*= (float s);
		FORCEINLINE Vector2& operator/= (float s);

		FORCEINLINE Vector2 Negate() const;

		// Vector operations
		FORCEINLINE bool InBounds(const Vector2& bounds) const;

		FORCEINLINE float Length() const;
		FORCEINLINE float LengthSquared() const;

		FORCEINLINE float Dot(const Vector2& v) const;
		FORCEINLINE Vector2 Cross(const Vector2& v) const;

		FORCEINLINE Vector2 Normalize() const;
		FORCEINLINE Vector2 Clamp(const Vector2& vMin, const Vector2& vMax) const;

		// Static functions
		FORCEINLINE static float Distance(const Vector2& v1, const Vector2& v2);
		FORCEINLINE static float DistanceSquared(const Vector2& v1, const Vector2& v2);

		FORCEINLINE static Vector2 Min(const Vector2& v1, const Vector2& v2);
		FORCEINLINE static Vector2 Max(const Vector2& v1, const Vector2& v2);

		FORCEINLINE static Vector2 Lerp(const Vector2& v1, const Vector2& v2, const float t);
		FORCEINLINE static Vector2 SmoothStep(const Vector2& v1, const Vector2& v2, float t);

		FORCEINLINE static Vector2 Reflect(const Vector2& v, const Vector2& n);

	private:
		Vector2Data mData;

		FORCEINLINE explicit Vector2(Vector2Data&& data);
	};

	Vector2::Vector2(Vector2Data&& data)
		: mData(std::move(data))
	{
	}
}