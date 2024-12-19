#include "system/hash.h"
#include "system/container/HashMap.h"
#include "system/flag64.h"
#include "ComponentDefine.h"

namespace fm
{
	class IComponent;

	typedef IComponent*(*ComponentCreator)(hashid objectName);
	typedef void(*ComponentCleaner)(IComponent* component);

	struct ComponentTypeInfo final
	{
		ComponentCreator creator;
		ComponentCleaner cleaner;
	};

	class ObjectManagerDB final
	{
	public:
		ObjectManagerDB();

	private:
		friend class GameObjectManager;

		enum { MaxCompTypesPerInterface = 5 };
		enum { MaxObjects = 1024 };

		ComponentTypeInfo mComponentTypeInfos[(uint32)eComponentType::Count];

		// Component types goruped by component interface
		eComponentType mComponentTypes[(uint32)eComponentInterface::Count][MaxCompTypesPerInterface];

		// each flag bit represents a component type that's subscribed to the message type
		Flag64 mSubscribedComponentTypes[(uint32)eMessageType::Count];

		HashMap<IComponent*, MaxObjects> mComponents[(uint32)eComponentType::Count];

		void Reset();
	};
}