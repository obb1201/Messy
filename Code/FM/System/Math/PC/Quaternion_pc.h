#pragma once

#include <DirectXMath.h>
#include "System/Assert.h"
#include "System/Math/Matrix.h"

using namespace DirectX;

namespace fm
{
	Quaternion::Quaternion(const float v)
	{
		mData = XMVectorSet(v, v, v, v);
	}

	Quaternion::Quaternion(const float x, const float y, const float z, const float w)
	{
		mData = XMVectorSet(x, y, z, w);
	}

	Quaternion::Quaternion(const Vector4& v)
	{
		mData = XMVectorSet(v.GetX(), v.GetY(), v.GetZ(), v.GetW());
	}

	Quaternion::Quaternion(const Matrix& m)
	{
		//This function only uses the upper 3x3 portion of the XMMATRIX
		mData = XMQuaternionRotationMatrix(m.mData);
	}

	Quaternion::Quaternion(const Vector3& axis, const float angleRadian)
	{
		mData = XMQuaternionRotationAxis(axis.mData, angleRadian);
	}

	Quaternion::Quaternion(const float* data)
	{
		mData = XMLoadFloat4((const XMFLOAT4* const)data);
	}

	Quaternion Quaternion::operator+ (const Quaternion& q) const
	{
		return Quaternion(XMVectorAdd(mData, q.mData));
	}

	Quaternion& Quaternion::operator+= (const Quaternion& q)
	{
		mData = XMVectorAdd(mData, q.mData);
		return *this;
	}

	Quaternion Quaternion::operator- (const Quaternion& q) const
	{
		return Quaternion(XMVectorSubtract(mData, q.mData));
	}

	Quaternion& Quaternion::operator-= (const Quaternion& q)
	{
		mData = XMVectorSubtract(mData, q.mData);
		return *this;
	}

	Quaternion Quaternion::operator* (const Quaternion& q) const
	{
		return Quaternion(XMQuaternionMultiply(mData, q.mData));
	}

	Quaternion& Quaternion::operator*= (const Quaternion& q)
	{
		mData = XMQuaternionMultiply(mData, q.mData);
		return *this;
	}

	Quaternion Quaternion::operator* (const float s) const
	{
		return Quaternion(XMVectorScale(mData, s));
	}

	Quaternion& Quaternion::operator*= (const float s)
	{
		mData = XMVectorScale(mData, s);
		return *this;
	}

	Quaternion Quaternion::operator/ (const float s) const
	{
		Assert(s != 0.0f, "division by 0");
		return Quaternion(XMVectorScale(mData, 1.f / s));
	}

	Quaternion& Quaternion::operator/= (const float s)
	{
		Assert(s != 0.0f, "division by 0");
		mData = XMVectorScale(mData, 1.f / s);
		return *this;
	}

	bool Quaternion::operator== (const Quaternion& q) const
	{
		return XMQuaternionEqual(mData, q.mData);
	}

	bool Quaternion::operator!= (const Quaternion& q) const
	{
		return XMQuaternionNotEqual(mData, q.mData);
	}

	float Quaternion::Length() const
	{
		return XMVectorGetX(XMQuaternionLength(mData));
	}

	float Quaternion::LengthSquared() const
	{
		return XMVectorGetX(XMQuaternionLengthSq(mData));
	}

	float Quaternion::Dot(const Quaternion& q) const
	{
		return XMVectorGetX(XMQuaternionDot(mData, q.mData));
	}

	Quaternion Quaternion::Normalize() const
	{
		return Quaternion(XMQuaternionNormalize(mData));
	}

	Quaternion Quaternion::Invert() const
	{
		return Quaternion(XMQuaternionInverse(mData));
	}

	void Quaternion::ToAxisAndAngle(Vector3* const outAxis, float* const outAngleRadian) const
	{
		XMQuaternionToAxisAngle(&outAxis->mData, outAngleRadian, mData);
	}

	void Quaternion::FromAxisAndAngle(Vector3& axis, const float angleRadian)
	{
		mData = XMQuaternionRotationAxis(axis.mData, angleRadian);
	}

	Matrix Quaternion::ToRotation() const
	{
		return Matrix(XMMatrixRotationQuaternion(mData));
	}

	void Quaternion::FromRotation(const Matrix& rotation)
	{
		mData = XMQuaternionRotationMatrix(rotation.mData);
	}

	Quaternion Quaternion::SphericalLerp(const Quaternion& v1, const Quaternion& v2, const float t)
	{
		return Quaternion(XMQuaternionSlerp(v1.mData, v2.mData, t));
	}

	Quaternion Quaternion::SphericalSmoothStep(const Quaternion& v1, const Quaternion& v2, float t)
	{
		Assert(t >= 0.f && t <= 1.f, "t should be between 0 and 1");

		t = t * t * (3.f - 2.f * t);
		return Quaternion(XMQuaternionSlerp(v1.mData, v2.mData, t));
	}
}