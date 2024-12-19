#pragma once

#include "ICmpMovable.h"
#include "Gameplay/ComponentSystem/ComponentDefine.h"
#include "System/Container/Vector.h"

namespace fm
{
	class CmpMassMovable final : public ICmpMovable
	{
	public:
		static IComponent* Create(hashid objectName);
		static void Clean(IComponent* const component);
		static void RegisterTypeInfo();

		float Gravity;
		float Mass;

		virtual eMessageResult HandleMessage(eMessageType type, const void * const params) override;
		virtual void Update(const float elaspedTime) override;

	private:
		Vector<CollisionInfo, 4> mCollisionInfos;

		CmpMassMovable(hashid objectName);
		CmpMassMovable() = delete;

		FORCEINLINE float GetPosition(float acceleration, float prevVelocity, float prevPosition, float time);
	};

	float CmpMassMovable::GetPosition(float acceleration, float prevVelocity, float prevPosition, float time)
	{
		float ret = prevPosition + (prevVelocity + acceleration * acceleration * 0.5f) * time;
		return ret;
	}
}