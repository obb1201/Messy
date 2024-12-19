#pragma once


#include "Gameplay/Physics/Collidable.h"
#include "ICmpCollidable.h"
#include "System/Math/Vector3.h"

namespace fm
{
	class CmpCylinderCollidable : public ICmpCollidable
	{
	public:
		static IComponent* Create(hashid objectName);
		static void Clean(IComponent* const component);
		static void RegisterTypeInfo();

		CylinderCollidable Data;

		virtual eMessageResult HandleMessage(eMessageType type, const void * const params) override;
		virtual void Update(const float elaspedTime) override;

	private:
		CmpCylinderCollidable(hashid objectName);
		CmpCylinderCollidable() = delete;

		virtual void CheckCollision(ICmpCollidable* collidable2);
	};
}