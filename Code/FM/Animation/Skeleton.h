#pragma once

#include "System/Hash.h"
#include "System/GlobalInclude.h"
#include "System/Container/Vector.h"

#include "System/Math/Quaternion.h"
#include "System/Math/Vector3.h"
#include "System/Math/Matrix.h"

#include "AnimationDefine.h"

namespace fm
{
	class Skeleton final
	{
		friend class Model;
		friend class Animation;
		friend class ModelManager;
	public:

	private:
		struct BoneNode
		{
			uint8 ID;
			uint8 ParentID;
			Quaternion Rotation;	// does it need?
			Vector3 Translation;	// does it need?
			Matrix OffsetTransform;	// original pose
		};

		Vector<BoneNode, MAXIMUM_BONES> mBones;
		Vector<Matrix, MAXIMUM_BONES> mFinalBoneTransforms;

		// --- methods
		Skeleton() = default;
		~Skeleton();
	};
}
