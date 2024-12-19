#include "CmpCamera.h"
#include "Graphics\Renderer.h"
#include "System\Math\Vector4.h"

namespace fm
{
	const static eComponentType sType = eComponentType::Camera;

	IComponent* CmpCamera::Create(hashid objectName)
	{
		// TODO(pope) mempoo
		return fmNew(eMemPool::Gameplay, "CmpCamera") CmpCamera(objectName);
	}

	void CmpCamera::Clean(IComponent* component)
	{
		fmDelete(component);
	}

	void CmpCamera::RegisterTypeInfo()
	{
		ICmpCamera::RegisterComponent(sType);
		GameObjectManager& gom = GameObjectManager::GetInstance();

		gom.RegisterComponentTypeInfo(sType, Create, Clean);
	}


	CmpCamera::CmpCamera(hashid objectName)
		: ICmpCamera(sType, objectName)
	{
	}


	void CmpCamera::SetLens(const float fovY, const float aspect, const float nearz, const float farz)
	{
		mbMatrixDirty = true;

		mFovY = fovY;
		mAspect = aspect;
		mNearZ = nearz;
		mFarZ = farz;

		mNearPlaneHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
		mFarPlaneHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

		mProjection.PerspectiveFovLH(fovY, aspect, nearz, farz);
	}

	void CmpCamera::LookAt(const Vector3& position, const Vector3& target, const Vector3& worldUp)
	{
		mbMatrixDirty = true;

		Vector3 look = target - position;
		look.Normalize();
		Vector3 right = worldUp.Cross(look);
		right.Normalize();
		Vector3 up = look.Cross(right);

		mPosition = position;
		mLook = look;
		mRight = right;
		mUp = up;
	}

	void CmpCamera::updateViewMatrix()
	{
		if (mbMatrixDirty)
		{
			mLook.Normalize();
			mUp = mLook.Cross(mRight);
			mUp.Normalize();
			mRight = mUp.Cross(mLook);

			float x = -mPosition.Dot(mRight);
			float y = -mPosition.Dot(mUp);
			float z = -mPosition.Dot(mLook);

			mView.SetAxis(Matrix::eAxis::X, Vector4(mRight.GetX(), mUp.GetX(), mLook.GetX(), 0.0f));
			mView.SetAxis(Matrix::eAxis::Y, Vector4(mRight.GetY(), mUp.GetY(), mLook.GetY(), 0.0f));
			mView.SetAxis(Matrix::eAxis::Z, Vector4(mRight.GetZ(), mUp.GetZ(), mLook.GetZ(), 0.0f));
			mView.SetAxis(Matrix::eAxis::W, Vector4(x, y, z, 1.0f));

			mViewProjection = mView * mProjection;

			mbMatrixDirty = false;
		}
	}

	eMessageResult CmpCamera::HandleMessage(eMessageType type, const void* const params)
	{
		return eMessageResult::Ok;
	}

	void CmpCamera::Update(const float elaspedTime)
	{
		updateViewMatrix();
	}

	void CmpCamera::Pitch(const float angleRadian)
	{
		mbMatrixDirty = true;

		Matrix rightMatrix;
		rightMatrix.RotateAxis(mRight, angleRadian);

		Vector4 up = Matrix::Multiply(Vector4(mUp.GetX(), mUp.GetY(), mUp.GetZ(), 0.0f), rightMatrix);
		Vector4 look = Matrix::Multiply(Vector4(mLook.GetX(), mLook.GetY(), mLook.GetZ(), 0.0f), rightMatrix);

		mUp = Vector3(up.GetX(), up.GetY(), up.GetZ());
		mLook = Vector3(look.GetX(), look.GetY(), look.GetZ());
	}
}