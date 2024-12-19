#include "Job.h"

namespace fm
{
	void JobUpdate(const Job& job)
	{
		switch (job.Type)
		{
		case eComponentType::Entity:
			FALLTHROUGH
		case eComponentType::Camera:
			FALLTHROUGH
		case eComponentType::MassMovable:
			FALLTHROUGH
		case eComponentType::PlaneCollider:
			FALLTHROUGH
		case eComponentType::SphereCollider:
			FALLTHROUGH
		case eComponentType::SkinnedMesh:
			FALLTHROUGH
		case eComponentType::StaticMesh:
		{
			//TODO : in the future, we might separate them into each type to cast. 
			IComponent* cmp = (IComponent*)job.Object;
			cmp->Update(job.ElapsedTime);
			break;
		}
		default:
			Assert(false, "unknown data type");
			break;
		}
	}
}