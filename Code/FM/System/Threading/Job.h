#pragma once
#include "System\GlobalInclude.h"

#include "Gameplay\ComponentSystem\IComponent.h"

namespace fm
{
	struct Job;

	typedef void(*JobUpdater)(const Job&);

	//Job is a task to update various components. 
	struct Job
	{
		eComponentType Type;
		JobUpdater Update;
		void* Object;
		float ElapsedTime;
	};

	void JobUpdate(const Job& job);
}
