#include "ObjectManagerDB.h"
#include "system/memory.h"

namespace fm
{
	ObjectManagerDB::ObjectManagerDB()
	{
		MemSet(mComponentTypeInfos, 0, sizeof(mComponentTypeInfos));
		MemSet(mComponentTypes, (uint8)eComponentType::Invalid, sizeof(mComponentTypes));

		Reset();
	}

	void ObjectManagerDB::Reset()
	{
		for (auto i = 0; i < StaticArraySize(mComponents); ++i)
		{
			mComponents[i].RemoveAll();
		}
	}
}