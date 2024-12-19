#pragma once

#include "Gameplay/Componentsystem/IComponent.h"
#include "Gameplay/Componentsystem/GameObjectManager.h"
#include "System/Math/Vector3.h"
#include "System/Math/Quaternion.h"
#include "System/Math/Matrix.h"

namespace fm
{
	class ICmpEntity : public IComponent
	{
	public:
		FORCEINLINE static void RegisterComponent(eComponentType compType);

		// HACK: for now we will have only one entity class, so move everything to ICmpEntity to avoid virtual
		FORCEINLINE const Vector3& GetPosition() const { return mPosition; }
		FORCEINLINE void SetPosition(const Vector3& v) { mMatrixDirty = true; mPosition = v; }

		FORCEINLINE const Quaternion& GetRotation() const { return mRotation; }
		FORCEINLINE void SetRotation(const Quaternion& v) { mMatrixDirty = true; mRotation = v; }

		FORCEINLINE const Vector3& GetScale() const { return mScale; }
		FORCEINLINE void SetScale(const Vector3& v) { mMatrixDirty = true; mScale = v; }

		void RebuildTransform();
		FORCEINLINE const Matrix& GetTransform() const { Assert(!mMatrixDirty, "call rebuildTransform before"); return mTransform; }

	protected:
		Vector3 mPosition = Vector3::Zero;
		Vector3 mScale = Vector3::One;
		Quaternion mRotation = Quaternion::Identity;
		Matrix mTransform;
		bool mMatrixDirty = true;

		ICmpEntity() = delete;
		ICmpEntity(eComponentType type, hashid objectName) : IComponent(type, objectName) {};
	};

	void ICmpEntity::RegisterComponent(eComponentType compType)
	{
		GameObjectManager::GetInstance().RegisterComponent(eComponentInterface::Entity, compType);
	}
}
