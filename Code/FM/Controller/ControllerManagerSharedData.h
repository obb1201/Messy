// TODO: do we really need this file? it can go into different platform files

#pragma once

#include "system/hash.h"

namespace fm
{
	// TODO: move this to player related class in gameplay
	constexpr int MAX_PLAYERS = 4;

	enum class MouseKey
	{
		Left,
		Middle,
		Right,

		// TODO: wtf?
		Pos,
		DeltaPos,

		ButtonLast = Right,
	};

	enum class Keyboard //TODO: fill these in some time in the future
	{
		KeyNone,//default value
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Space,
		Escape,
		BackSpace,
		Tab,
		RightShift,
		LeftShift,
		RightControl,
		LeftControl,
		RightALT,
		LeftALT,
		ArrowLeft,
		ArrowRight,
		ArrowUp,
		ArrowDown,
	};

	enum class GamePad
	{
		None,//default value
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,
		Start,
		Back,
		LeftThumb,
		RightThumb,
		LeftShoulder,
		RightShoulder,
		A,
		B,
		X,
		Y,
		LeftTrigger,
		RightTrigger,
		LeftThumbDirection,
		RightThumbDirection,
	};

	struct KeyboardBinding
	{
		Keyboard Key;
		GamePad ControllerKey;
		bool Repeat;
		bool Value;
	};

	struct MouseBinding
	{
		uint32 ActionID;
		MouseKey Key;
		bool Repeat;
		bool Value;
	};

	// TODO: rename to GamePad
	struct ControllerBinding
	{
		uint32 ActionID;
		uint32 ControllerID;
		GamePad Key;
		bool Repeat;
		bool Value;
		float Deadzone;
	};

	struct Binding
	{
		int Index;
		bool Value;
		float Dx; 
		float Dy; //need better name, but it's just for thumbstick value and I guess mouse eventually
	};

}