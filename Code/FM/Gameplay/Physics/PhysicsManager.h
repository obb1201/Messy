#pragma once

#include "System/Assert.h"
#include "System/GlobalInclude.h"
#include "System/Container/Vector.h"

namespace fm
{
	class ICmpCollidable;
	class PhysicsManager final
	{
	public:
		// singleton-related
		static void CreateInstance();
		static void DeleteInstance();
		static PhysicsManager& GetInstance() { Assert(mInstance != nullptr); return *mInstance; }

		COPY_CONSTRUCTOR(PhysicsManager) = delete;
		ASSIGN_OPERATOR(PhysicsManager) = delete;

		void AddCollidable(ICmpCollidable* collidable);

		void Update();

	private:
		static PhysicsManager* mInstance;
		static const uint32 MaxObject = 1024;

		Vector<ICmpCollidable*, MaxObject> mCollidables;
		
		PhysicsManager();
		~PhysicsManager();

		void MakeCollidableGroup(ICmpCollidable* const master, uint32 index);
	};
}
