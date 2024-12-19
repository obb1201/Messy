#pragma once

//--------------------------------------------
// Matrix::multiply is able to multiply Vector4 by Matrix :
// (Vector4) = (Vector4) * (Matrix)
//--------------------------------------------
#include <utility>
#include "system/globalinclude.h"


namespace fm
{
	class Vector3;
	class Vector4;
	class Quaternion;

	class Matrix final
	{
		friend class Quaternion;
	public:
		// Constants
		const static Matrix Identity;

		enum class eAxis
		{
			X,
			Y,
			Z,
			W,
		};

		FORCEINLINE Matrix() = default;
		FORCEINLINE Matrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
		FORCEINLINE Matrix(const float *data);

		// Comparision operators
		FORCEINLINE bool operator== (const Matrix& m) const;
		FORCEINLINE bool operator!= (const Matrix& m) const;

		// Assignment operators
		FORCEINLINE Matrix& operator= (const Matrix& m);
		FORCEINLINE Matrix& operator*= (const Matrix& m);
		FORCEINLINE Matrix operator* (const Matrix& m);

		// Matrix operations
		FORCEINLINE Vector3 GetTranslation() const;
		FORCEINLINE void SetTranslation(const Vector3& translation);
		FORCEINLINE void Build(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
		FORCEINLINE bool Decompose(Vector3& scale, Vector4& rotation, Vector3& translation) const;
		FORCEINLINE void Transpose();
		FORCEINLINE Matrix GetTranspose() const;
		FORCEINLINE void Invert();
		FORCEINLINE Matrix GetInvert() const;

		// Matrix rotate
		FORCEINLINE void RotateAxis(const Vector3& axis, const float angleRadian);
		FORCEINLINE void MakeRotateX(const float angleRadian);
		FORCEINLINE void MakeRotateY(const float angleRadian);
		FORCEINLINE void MakeRotateZ(const float angleRadian);

		FORCEINLINE float Determinant() const;

		// Set an axis of matrix
		FORCEINLINE void SetAxis(const eAxis axis, const Vector4& v);

		// Static functions
		FORCEINLINE static Matrix Lerp(const Matrix& m1, const Matrix& m2, float t);
		FORCEINLINE static Vector4 Multiply(const Vector4& v, const Matrix& m);

		// Miscellaneous
		FORCEINLINE void PerspectiveFovLH(const float fovAngleY, const float aspect, const float nearZ, const float farZ);

	private:
		MatrixData mData;

		FORCEINLINE explicit Matrix(MatrixData&& data) 
			: mData(std::move(data))
		{

		}
	};
}