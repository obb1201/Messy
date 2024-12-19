#pragma once

#include "Gameplay/Physics/Collidable.h"
#include "ICmpCollidable.h"
#include "System/Math/Vector3.h"

namespace fm
{
	class CmpPlaneCollidable : public ICmpCollidable
	{
	public:
		static IComponent* Create(hashid objectName);
		static void Clean(IComponent* const component);
		static void RegisterTypeInfo();

		PlaneCollidable Data;

		virtual eMessageResult HandleMessage(eMessageType type, const void * const params) override;
		virtual void Update(const float elaspedTime) override;

	private:
		CmpPlaneCollidable(hashid objectName);
		CmpPlaneCollidable() = delete;

		virtual void CheckCollision(ICmpCollidable* collidable2);
	};
}
