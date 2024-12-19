#pragma once

#include "System/Math/Vector3.h"

namespace fm
{
	struct SphereCollidable
	{
		Vector3 Position;
		float Radius;
	};

	struct PlaneCollidable
	{
		Vector3 Position;
		Vector3 Normal;
		float Width;
		float Height;
	};

	struct CylinderCollidable
	{
		Vector3 Position;
		Vector3 Normal;
		float Radius;
		float Height;
	};

	struct CapsuleCollidable
	{
		SphereCollidable Top;
		CylinderCollidable Middle;
		SphereCollidable Bottom;
	};
}
