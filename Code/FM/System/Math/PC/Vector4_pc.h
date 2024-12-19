#pragma once

#include <DirectXMath.h>
#include "system/assert.h"
#include "system/math/Vector4.h"

using namespace DirectX;
namespace fm
{
	Vector4::Vector4(const float v)
	{
		mData = XMVectorSet(v, v, v, v);
	}

	Vector4::Vector4(const float x, const float y, const float z, const float w)
	{
		mData = XMVectorSet(x, y, z, w);
	}

	Vector4::Vector4(const float* const data)
	{
		mData = XMLoadFloat4((const XMFLOAT4* const)data);
	}

	float Vector4::GetX() const
	{
		return XMVectorGetX(mData);
	}

	float Vector4::GetY() const
	{
		return XMVectorGetY(mData);
	}

	float Vector4::GetZ() const
	{
		return XMVectorGetZ(mData);
	}

	float Vector4::GetW() const
	{
		return XMVectorGetW(mData);
	}

	void Vector4::SetX(const float x)
	{
		mData = XMVectorSetX(mData, x);
	}

	void Vector4::SetY(const float y)
	{
		mData = XMVectorSetY(mData, y);
	}

	void Vector4::SetZ(const float z)
	{
		mData = XMVectorSetZ(mData, z);
	}

	void Vector4::SetW(const float w)
	{
		mData = XMVectorSetW(mData, w);
	}

	void Vector4::Set(const float x, const float y, const float z, const float w)
	{
		mData = XMVectorSet(x, y, z, w);
	}

	bool Vector4::operator== (const Vector4& v) const
	{
		return XMVector4Equal(mData, v.mData);
	}

	bool Vector4::operator!= (const Vector4& v) const
	{
		return XMVector4NotEqual(mData, v.mData);
	}

	Vector4& Vector4::operator= (const Vector4& v)
	{
		mData = v.mData;
		return *this;
	}

	Vector4 Vector4::operator+ (const Vector4& v) const
	{
		return Vector4(XMVectorAdd(mData, v.mData));
	}

	Vector4 Vector4::operator* (const float s) const
	{
		return Vector4(XMVectorScale(mData, s));
	}

	Vector4& Vector4::operator+= (const Vector4& v)
	{
		mData = XMVectorAdd(mData, v.mData);
		return *this;
	}

	Vector4& Vector4::operator-= (const Vector4& v)
	{
		mData = XMVectorSubtract(mData, v.mData);
		return *this;
	}

	Vector4& Vector4::operator*= (const Vector4& v)
	{
		mData = XMVectorMultiply(mData, v.mData);
		return *this;
	}

	Vector4& Vector4::operator*= (float s)
	{
		mData = XMVectorScale(mData, s);
		return *this;
	}

	Vector4& Vector4::operator/= (float s)
	{
		Assert(s != 0.0f, "division by 0");
		mData = XMVectorScale(mData, 1.f / s);
		return *this;
	}

	bool Vector4::InBounds(const Vector4& bounds) const
	{
		return XMVector4InBounds(mData, bounds.mData);
	}

	float Vector4::Length() const
	{
		return XMVectorGetX(XMVector4Length(mData));
	}

	float Vector4::LengthSquared() const
	{
		return XMVectorGetX(XMVector4LengthSq(mData));
	}

	float Vector4::Dot(const Vector4& v) const
	{
		return XMVectorGetX(XMVector4Dot(mData, v.mData));
	}

	Vector4 Vector4::Cross(const Vector4& v1, const Vector4& v2) const
	{
		Vector4(XMVector4Cross(mData, v1.mData, v2.mData));
	}

	Vector4 Vector4::Normalize() const
	{
		Vector4(XMVector4Normalize(mData));
	}

	Vector4 Vector4::Clamp(const Vector4& vMin, const Vector4& vMax) const
	{
		Vector4(XMVectorClamp(mData, vMin.mData, vMax.mData));
	}

	float Vector4::Distance(const Vector4& v1, const Vector4& v2)
	{
		return XMVectorGetX(XMVector4Length(XMVectorSubtract(v2.mData, v1.mData)));
	}

	float Vector4::DistanceSquared(const Vector4& v1, const Vector4& v2)
	{
		return XMVectorGetX(XMVector4LengthSq(XMVectorSubtract(v2.mData, v1.mData)));
	}

	Vector4 Vector4::Min(const Vector4& v1, const Vector4& v2)
	{
		Vector4(XMVectorMin(v1.mData, v2.mData));
	}

	Vector4 Vector4::Max(const Vector4& v1, const Vector4& v2)
	{
		Vector4(XMVectorMax(v1.mData, v2.mData));
	}

	Vector4 Vector4::Lerp(const Vector4& v1, const Vector4& v2, const float t)
	{
		return Vector4(XMVectorLerp(v1.mData, v2.mData, t));
	}

	Vector4 Vector4::SmoothStep(const Vector4& v1, const Vector4& v2, float t)
	{
		Assert(t >= 0.f && t <= 1.f, "t should be between 0 and 1");
		
		t = t * t * (3.f - 2.f * t);
		return Vector4(XMVectorLerp(v1.mData, v2.mData, t));
	}

	Vector4 Vector4::Reflect(const Vector4& v, const Vector4& n)
	{
		return Vector4(XMVector4Reflect(v.mData, n.mData));
	}

	Vector4 Vector4::Negate() const
	{
		return Vector4(XMVectorNegate(mData));
	}
}