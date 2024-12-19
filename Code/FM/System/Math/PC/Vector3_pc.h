#pragma once

#include <DirectXMath.h>
#include "system/assert.h"
#include "system/math/vector3.h"

using namespace DirectX;
namespace fm
{
	Vector3::Vector3(const float v)
	{
		mData = XMVectorSet(v, v, v, 0);
	}

	Vector3::Vector3(const float x, const float y, const float z)
	{
		mData = XMVectorSet(x, y, z, 0);
	}

	Vector3::Vector3(const float* const data)
	{
		mData = XMLoadFloat3((const XMFLOAT3* const)data);
	}

	float Vector3::GetX() const
	{
		return XMVectorGetX(mData);
	}

	float Vector3::GetY() const
	{
		return XMVectorGetY(mData);
	}

	float Vector3::GetZ() const
	{
		return XMVectorGetZ(mData);
	}

	void Vector3::SetX(const float x)
	{
		mData = XMVectorSetX(mData, x);
	}

	void Vector3::SetY(const float y)
	{
		mData = XMVectorSetY(mData, y);
	}

	void Vector3::SetZ(const float z)
	{
		mData = XMVectorSetY(mData, z);
	}

	void Vector3::Set(const float x, const float y, const float z)
	{
		mData = XMVectorSet(x, y, z, 0);
	}

	bool Vector3::operator== (const Vector3& v) const
	{
		return XMVector3Equal(mData, v.mData);
	}

	bool Vector3::operator!= (const Vector3& v) const
	{
		return XMVector3NotEqual(mData, v.mData);
	}

	Vector3& Vector3::operator= (const Vector3& v)
	{
		mData = v.mData;
		return *this;
	}

	Vector3 Vector3::operator+ (const Vector3& v) const
	{
		return Vector3(XMVectorAdd(mData, v.mData));
	}

	Vector3 Vector3::operator- (const Vector3& v) const
	{
		return Vector3(XMVectorSubtract(mData, v.mData));
	}

	Vector3 Vector3::operator* (const float s) const
	{
		return Vector3(XMVectorScale(mData, s));
	}

	Vector3 Vector3::operator/ (const float s) const
	{
		return Vector3(XMVectorScale(mData, 1.0f / s ));
	}

	Vector3& Vector3::operator+= (const Vector3& v)
	{
		mData = XMVectorAdd(mData, v.mData);
		return *this;
	}

	Vector3& Vector3::operator-= (const Vector3& v)
	{
		mData = XMVectorSubtract(mData, v.mData);
		return *this;
	}

	Vector3& Vector3::operator*= (const Vector3& v)
	{
		mData = XMVectorMultiply(mData, v.mData);
		return *this;
	}

	Vector3& Vector3::operator*= (float s)
	{
		mData = XMVectorScale(mData, s);
		return *this;
	}

	Vector3& Vector3::operator/= (float s)
	{
		Assert(s != 0.0f, "division by 0");
		mData = XMVectorScale(mData, 1.f / s);
		return *this;
	}

	bool Vector3::InBounds(const Vector3& bounds) const
	{
		return XMVector3InBounds(mData, bounds.mData);
	}

	float Vector3::Length() const
	{
		return XMVectorGetX(XMVector3Length(mData));
	}

	float Vector3::LengthSquared() const
	{
		return XMVectorGetX(XMVector3LengthSq(mData));
	}

	float Vector3::Dot(const Vector3& v) const
	{
		return XMVectorGetX(XMVector3Dot(mData, v.mData));
	}

	Vector3 Vector3::Cross(const Vector3& v) const
	{
		return Vector3(XMVector3Cross(mData, v.mData));
	}

	Vector3 Vector3::Normalize() const
	{
		return Vector3(XMVector3Normalize(mData));
	}

	Vector3 Vector3::Clamp(const Vector3& vMin, const Vector3& vMax) const
	{
		Vector3(XMVectorClamp(mData, vMin.mData, vMax.mData));
	}

	float Vector3::Distance(const Vector3& v1, const Vector3& v2)
	{
		return XMVectorGetX(XMVector3Length(XMVectorSubtract(v2.mData, v1.mData)));
	}

	float Vector3::DistanceSquared(const Vector3& v1, const Vector3& v2)
	{
		return XMVectorGetX(XMVector3LengthSq(XMVectorSubtract(v2.mData, v1.mData)));
	}

	Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(XMVectorMin(v1.mData, v2.mData));
	}

	Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(XMVectorMax(v1.mData, v2.mData));
	}

	Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, const float t)
	{
		return Vector3(XMVectorLerp(v1.mData, v2.mData, t));
	}

	Vector3 Vector3::SmoothStep(const Vector3& v1, const Vector3& v2, float t)
	{
		Assert(t >= 0.f && t <= 1.f, "t should be between 0 and 1");
		
		t = t * t * (3.f - 2.f * t);
		return Vector3(XMVectorLerp(v1.mData, v2.mData, t));
	}

	Vector3 Vector3::Reflect(const Vector3& v, const Vector3& n)
	{
		return Vector3(XMVector3Reflect(v.mData, n.mData));
	}
	
	Vector3 Vector3::Negate() const
	{
		return Vector3(XMVectorNegate(mData));
	}
}