
#include "ICmpCollidable.h"
#include "CmpCapsuleCollidable.h"
#include "CmpCylinderCollidable.h"
#include "CmpPlaneCollidable.h"
#include "CmpSphereCollidable.h"

namespace fm
{
	void ICmpCollidable::CheckAllConnectedCollision()
	{
		for (auto i = 0u; i < mConnectedList.GetSize(); ++i)
		{
			for (auto j = i + 1; j < mConnectedList.GetSize(); ++j)
			{
				mConnectedList[i]->CheckCollision(mConnectedList[j]);
			}
		}
	}
}