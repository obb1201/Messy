#pragma once

#include "ICmpEntity.h"

namespace fm
{
	class CmpEntity final : public ICmpEntity
	{
	public:
		static IComponent* Create(hashid objectName);
		static void Clean(IComponent* const component);
		static void RegisterTypeInfo();

		virtual eMessageResult HandleMessage(eMessageType type, const void * const params) override;
		virtual void Update(const float elaspedTime) override;

	private:
		CmpEntity(hashid objectName);
		CmpEntity() = delete;
	};
}