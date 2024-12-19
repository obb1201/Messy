#include "Model.h"

#include "System/Memory.h"
#include "Animation/Skeleton.h"

namespace fm
{
	Model::~Model()
	{
		for (uint32 i = 0; i < mMeshes.GetSize(); ++i)
		{
			SafeDelete(mMeshes[i]);
		}
		mMeshes.RemoveAll();

		if (mSkeleton)
		{
			SafeDelete(mSkeleton);
		}
	}
}
