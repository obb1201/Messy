#pragma once

#include <DirectXMath.h>
#include "system/assert.h"

#include "system/math/vector3.h"
#include "system/math/vector4.h"

#include "System/Math/Quaternion.h"

using namespace DirectX;
namespace fm
{
	Matrix::Matrix(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		mData = XMMatrixSet(m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33);
	}

	Matrix::Matrix(const float* data)
	{
		Assert(data != nullptr);

		mData.r[0] = XMLoadFloat4((const XMFLOAT4*)data);
		mData.r[1] = XMLoadFloat4((const XMFLOAT4*)(data + 4));
		mData.r[2] = XMLoadFloat4((const XMFLOAT4*)(data + 8));
		mData.r[3] = XMLoadFloat4((const XMFLOAT4*)(data + 12));
	}

	bool Matrix::operator== (const Matrix& m) const
	{
		return (XMVector4Equal(mData.r[0], m.mData.r[0])
			&& XMVector4Equal(mData.r[1], m.mData.r[1])
			&& XMVector4Equal(mData.r[2], m.mData.r[2])
			&& XMVector4Equal(mData.r[3], m.mData.r[3])) != 0;
	}

	bool Matrix::operator!= (const Matrix& m) const
	{
		return (XMVector4NotEqual(mData.r[0], m.mData.r[0])
			&& XMVector4NotEqual(mData.r[1], m.mData.r[1])
			&& XMVector4NotEqual(mData.r[1], m.mData.r[2])
			&& XMVector4NotEqual(mData.r[1], m.mData.r[3])) != 0;
	}

	Matrix& Matrix::operator= (const Matrix& m)
	{
		mData = m.mData;

		return *this;
	}

	Matrix& Matrix::operator*= (const Matrix& m)
	{
		mData = XMMatrixMultiply(mData, m.mData);
		return *this;
	}

	Matrix Matrix::operator* (const Matrix& m)
	{
		return Matrix(XMMatrixMultiply(mData, m.mData));
	}

	Vector3 Matrix::GetTranslation() const
	{
		return Vector3(mData.r[3].m128_f32);
	}

	void Matrix::SetTranslation(const Vector3& translation)
	{
		//[4,4] to Matrix must be 1.0f.
		mData.r[3] = XMVectorSet(translation.GetX(), translation.GetY(), translation.GetZ(), 1.0f);
	}

	void Matrix::Build(const Vector3& scale, const Quaternion& rotation, const Vector3& translation)
	{
		XMMATRIX matrixScale = XMMatrixScalingFromVector(scale.mData);
		XMMATRIX matrixRotation = XMMatrixRotationQuaternion(rotation.mData);
		mData = XMMatrixMultiply(matrixScale, matrixRotation);
		SetTranslation(translation);
	}

	bool Matrix::Decompose(Vector3& scale, Vector4& rotation, Vector3& translation) const
	{
		XMVECTOR s, r, t;

		if (!XMMatrixDecompose(&s, &r, &t, mData))
			return false;

		scale = Vector3(s.m128_f32);
		rotation = Vector4(r.m128_f32);
		translation = Vector3(r.m128_f32);

		return true;
	}

	Matrix Matrix::GetTranspose() const
	{
		Matrix ret;
		ret.mData = XMMatrixTranspose(mData);
		return ret;
	}

	void Matrix::Transpose()
	{
		mData = XMMatrixTranspose(mData);
	}

	Matrix Matrix::GetInvert() const
	{
		XMVECTOR det;
		Matrix ret;
		ret.mData = XMMatrixInverse(&det, mData);

		return ret;
	}

	void Matrix::Invert()
	{
		XMVECTOR det;
		mData = XMMatrixInverse(&det, mData);
	}

	void Matrix::RotateAxis(const Vector3& axis, const float angleRadian)
	{
		mData = XMMatrixRotationAxis(axis.mData, angleRadian);
	}

	void Matrix::MakeRotateX(const float angleRadian)
	{
		mData = XMMatrixRotationX(angleRadian);
	}

	void Matrix::MakeRotateY(const float angleRadian)
	{
		mData = XMMatrixRotationY(angleRadian);
	}

	void Matrix::MakeRotateZ(const float angleRadian)
	{
		mData = XMMatrixRotationZ(angleRadian);
	}

	float Matrix::Determinant() const
	{
		return XMVectorGetX(XMMatrixDeterminant(mData));
	}

	void Matrix::SetAxis(const eAxis axis, const Vector4& v)
	{
		mData.r[(uint32)axis] = v.mData;
	}

	Matrix Matrix::Lerp(const Matrix& m1, const Matrix& m2, float t)
	{
		Matrix ret;
		ret.mData.r[0] = XMVectorLerp(m1.mData.r[0], m2.mData.r[0], t);
		ret.mData.r[1] = XMVectorLerp(m1.mData.r[1], m2.mData.r[1], t);
		ret.mData.r[2] = XMVectorLerp(m1.mData.r[2], m2.mData.r[2], t);
		ret.mData.r[3] = XMVectorLerp(m1.mData.r[3], m2.mData.r[3], t);
		return ret;
	}

	Vector4 Matrix::Multiply(const Vector4& v, const Matrix& m)
	{
		Matrix matCol = m.GetTranspose();

		Vector4 ret;
		ret.SetX(v.Dot(matCol.mData.r[0].m128_f32));
		ret.SetY(v.Dot(matCol.mData.r[1].m128_f32));
		ret.SetZ(v.Dot(matCol.mData.r[2].m128_f32));
		ret.SetW(v.Dot(matCol.mData.r[3].m128_f32));

		return ret;
	}

	void Matrix::PerspectiveFovLH(const float fovAngleY, const float aspect, const float nearZ, const float farZ)
	{
		mData = XMMatrixPerspectiveFovLH(fovAngleY, aspect, nearZ, farZ);
	}
}