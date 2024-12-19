#pragma once

#include "Gameplay/ComponentSystem/IComponent.h"

#include "System/Hash.h"
#include "System/GlobalInclude.h"
#include "System/Container/Vector.h"
#include "System/Math/Vector3.h"
#include "System/Math/Quaternion.h"

namespace fm
{
	class Ball final
	{
	public:
		Ball() = default;
		void Initialize(const Vector3& initPosition, const char* BallName);

		COPY_CONSTRUCTOR(Ball) = delete;
		ASSIGN_OPERATOR(Ball) = delete;

		// HACK - test
		void Update();

		void SetVelocity(const Vector3& velocity);
		void SetPosition(const Vector3& position, const Quaternion& rotation);

	private:
		hashid mObjectID;
		Vector3 mInitPosition;

		// HACK
		Handle mPackageHandle = INVALID_HANDLE;
		bool mbLoadHasCompleted = false;
	};
}