#pragma once

#include "ICmpCamera.h"
#include "System/Math/Vector3.h"

namespace fm
{
	class CmpCamera : public ICmpCamera
	{
	public:
		static IComponent* Create(hashid objectName);
		static void Clean(IComponent* const component);
		static void RegisterTypeInfo();

		FORCEINLINE const Vector3& GetPosition() const { return mPosition; }
		FORCEINLINE void SetPosition(const float x, const float y, const float z);
		FORCEINLINE void SetPosition(const Vector3& position);

		FORCEINLINE const Vector3& GetRight() const { return mRight; }
		FORCEINLINE const Vector3& GetUp() const { return mUp; }
		FORCEINLINE const Vector3& GetLook() const { return mLook; }

		FORCEINLINE const float GetNearZ() const { return mNearZ; }
		FORCEINLINE const float GetFarZ() const { return mFarZ; }
		FORCEINLINE const float GetAspect() const { return mAspect; }
		FORCEINLINE const float GetFovY() const { return mFovY; }
		FORCEINLINE float GetFovX() const;

		FORCEINLINE const float GetNearPlaneWidth() const { return mAspect * mNearPlaneHeight; }
		FORCEINLINE const float GetNearPlaneHeight() const { return mNearPlaneHeight; }
		FORCEINLINE const float GetFarPlaneWidth() const { return mAspect * mFarPlaneHeight; }
		FORCEINLINE const float GetFarPlaneHeight() const { return mFarPlaneHeight; }

		void SetLens(const float fovY, const float aspect, const float nearz, const float farz);
		void LookAt(const Vector3& position, const Vector3& target, const Vector3& worldUp);

		FORCEINLINE const Matrix& GetViewMatrix() const { return mView; }
		FORCEINLINE const Matrix& GetProjectionMatrix() const { return mProjection; }
		virtual const Matrix& GetViewProjectionMatrix() const { return mViewProjection; }

		FORCEINLINE void Strafe(const float distance);	// move rightward
		FORCEINLINE void Walk(const float distance);	// move forward

		void Pitch(const float angleRadian);	// around right vector axis

		virtual eMessageResult HandleMessage(eMessageType type, const void* const params) override;
		virtual void Update(const float elaspedTime) override;

	protected:
		Vector3 mPosition = Vector3::Zero;
		Vector3 mRight = Vector3::UnitX;
		Vector3 mUp = Vector3::UnitY;
		Vector3 mLook = Vector3::UnitZ;

		float mNearZ = 0.0f;
		float mFarZ = 0.0f;
		float mAspect = 0.0f;
		float mFovY = 0.0f;
		float mNearPlaneHeight = 0.0f;
		float mFarPlaneHeight = 0.0f;

		Matrix mView;
		Matrix mProjection;
		Matrix mViewProjection;

		bool mbMatrixDirty = false;

		// --- Methods
		CmpCamera(hashid objectName);
		CmpCamera() = delete;

		void updateViewMatrix();
	};

	void CmpCamera::SetPosition(const float x, const float y, const float z)
	{
		mbMatrixDirty = true;
		mPosition.Set(x, y, z);
	}

	void CmpCamera::SetPosition(const Vector3& position)
	{
		mbMatrixDirty = true;
		mPosition = position;
	}

	float CmpCamera::GetFovX() const
	{
		float halfWidth = 0.5f * GetNearPlaneWidth();
		return 2.0f * atanf(halfWidth / mNearZ);
	}


	void CmpCamera::Strafe(const float distance)
	{
		mbMatrixDirty = true;
		mPosition += mRight * distance;
	}

	void CmpCamera::Walk(const float distance)
	{
		mbMatrixDirty = true;
		mPosition += mLook * distance;
	}
}