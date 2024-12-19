#pragma once

#include "System/Math/Vector3.h"

namespace fm
{
	// Each component type is subscribe to 0-to-many message types
	enum class eMessageType
	{
		Update,
		Draw,
		ObjectCreated,
		AllObjectsCreated,
		Commit,
		Collide,
		UpdatePosition,

		Count,
	};

	// Each component's message handler returns one of these results
	enum class eMessageResult
	{
		Ignored,    // not processed
		Ok,         // processed
		error,      // error occured

		Count,
	};

	// Interface ID for Components
	// 
	// This is abstract layer for components, so Multiple components can have same interface id.
	// (e.g. AIChase and AIRunAway components both have the interface ID of AI
	enum class eComponentInterface
	{
		Entity,
		Renderable,
		AI,
		Collidable,
		Movable,
		Light,
		Controllable,
		Camera,

		Count,
	};

	// Component type enumeration
	// 
	// This is concrete type which implements InterfaceID.  There is one-to-one mapping between
	// this enumeration and concrete components.
	enum class eComponentType : uint8
	{
		Entity,
		Camera,
		StaticMesh,
		SkinnedMesh,
		SphereCollider,
		PlaneCollider,
		CylinderCollider,
		CapsuleCollider,
		PlayerCollider,
		MassMovable,

		Count,
		Invalid = Count,
	};

	struct CollisionInfo
	{
		Vector3 Position;
		Vector3 Normal;
		Vector3 OtherVelocity;
		float Elasticity;
		float DistanceToSurface;
		float OtherMass;
	};
}
