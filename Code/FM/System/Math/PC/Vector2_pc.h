#pragma once

#include <DirectXMath.h>
#include "system/assert.h"
#include "system/math/vector2.h"

using namespace DirectX;
namespace fm
{
	Vector2::Vector2(float v)
	{
		mData = XMVectorSet(v, v, 0, 0);
	}

	Vector2::Vector2(float x, float y)
	{
		mData = XMVectorSet(x, y, 0, 0);
	}
	
	Vector2::Vector2(const float* data)
	{
		mData = XMLoadFloat2((XMFLOAT2*)data);
	}

	float Vector2::GetX() const
	{
		return XMVectorGetX(mData);
	}

	float Vector2::GetY() const
	{
		return XMVectorGetY(mData);
	}

	void Vector2::SetX(const float x)
	{
		mData = XMVectorSetX(mData, x);
	}

	void Vector2::SetY(const float y)
	{
		mData = XMVectorSetY(mData, y);
	}

	void Vector2::Set(const float x, const float y)
	{
		mData = XMVectorSet(x, y, 0, 0);
	}

	bool Vector2::operator== (const Vector2& v) const
	{
		return XMVector2Equal(mData, v.mData);
	}

	bool Vector2::operator!= (const Vector2& v) const
	{
		return XMVector2NotEqual(mData, v.mData);
	}

	Vector2& Vector2::operator= (const Vector2& v)
	{
		mData = v.mData;
		return *this;
	}

	Vector2& Vector2::operator+= (const Vector2& v)
	{
		mData = XMVectorAdd(mData, v.mData);
		return *this;
	}

	Vector2& Vector2::operator-= (const Vector2& v)
	{
		mData = XMVectorSubtract(mData, v.mData);
		return *this;
	}

	Vector2& Vector2::operator*= (const Vector2& v)
	{
		mData = XMVectorMultiply(mData, v.mData);
		return *this;
	}

	Vector2& Vector2::operator*= (const float s)
	{
		mData = XMVectorScale(mData, s);
		return *this;
	}
	
	Vector2& Vector2::operator/= (const float s)
	{
		Assert(s != 0.0f, "division by 0");
		mData = XMVectorScale(mData, 1.f / s);
		return *this;
	}

	bool Vector2::InBounds(const Vector2& bounds) const
	{
		return XMVector2InBounds(mData, bounds.mData);
	}

	float Vector2::Length() const
	{
		return XMVectorGetX(XMVector2Length(mData));
	}

	float Vector2::LengthSquared() const
	{
		return XMVectorGetX(XMVector2LengthSq(mData));
	}

	float Vector2::Dot(const Vector2& v) const
	{
		return XMVectorGetX(XMVector2Dot(mData, v.mData));
	}

	Vector2 Vector2::Cross(const Vector2& v) const
	{
		return Vector2(XMVector2Cross(mData, v.mData));
	}

	Vector2 Vector2::Normalize() const
	{
		return Vector2(XMVector2Normalize(mData));
	}

	Vector2 Vector2::Clamp(const Vector2& vMin, const Vector2& vMax) const
	{
		return Vector2(XMVectorClamp(mData, vMin.mData, vMax.mData));
	}

	float Vector2::Distance(const Vector2& v1, const Vector2& v2)
	{
		return XMVectorGetX(XMVector2Length( XMVectorSubtract(v2.mData, v1.mData) ));
	}

	float Vector2::DistanceSquared(const Vector2& v1, const Vector2& v2)
	{
		return XMVectorGetX(XMVector2LengthSq(XMVectorSubtract(v2.mData, v1.mData)));
	}

	Vector2 Vector2::Min(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(XMVectorMin(v1.mData, v2.mData));
	}

	Vector2 Vector2::Max(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(XMVectorMax(v1.mData, v2.mData));
	}

	Vector2 Vector2::Lerp(const Vector2& v1, const Vector2& v2, const float t)
	{
		return Vector2(XMVectorLerp(v1.mData, v2.mData, t));
	}

	Vector2 Vector2::SmoothStep(const Vector2& v1, const Vector2& v2, float t)
	{
		Assert(t >= 0.f && t <= 1.f, "t should be between 0 and 1");
		
		t = t * t * (3.f - 2.f * t);
		return Vector2(XMVectorLerp(v1.mData, v2.mData, t));
	}

	Vector2 Vector2::Reflect(const Vector2& v, const Vector2& n)
	{
		return Vector2(XMVector2Reflect(v.mData, n.mData));
	}

	Vector2 Vector2::Negate() const
	{
		return Vector2(XMVectorNegate(mData));
	}
}