#pragma once


#include "Gameplay/Physics/Collidable.h"
#include "ICmpCollidable.h"
#include "System/Math/Vector3.h"

namespace fm
{
	class CmpCapsuleCollidable : public ICmpCollidable
	{
	public:
		static IComponent* Create(hashid objectName);
		static void Clean(IComponent* const component);
		static void RegisterTypeInfo();

		CapsuleCollidable Data;

		virtual eMessageResult HandleMessage(eMessageType type, const void * const params) override;
		virtual void Update(const float elaspedTime) override;

	private:
		CmpCapsuleCollidable(hashid objectName);
		CmpCapsuleCollidable() = delete;

		virtual void CheckCollision(ICmpCollidable* collidable2);
	};
}