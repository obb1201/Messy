#include "gameplay/componentsystem/icomponent.h"
#include "gameplay/componentsystem/gameobjectmanager.h"


namespace fm
{
	class ICmpLight : public IComponent
	{
	public:
		FORCEINLINE static void RegisterComponent(eComponentType compType);
	protected:
		ICmpLight() = delete;
		ICmpLight(eComponentType type, hashid objectName) : IComponent(type, objectName) {};

	};

	void ICmpLight::RegisterComponent(eComponentType compType)
	{
		GameObjectManager::GetInstance().RegisterComponent(eComponentInterface::Light, compType);
	}
}