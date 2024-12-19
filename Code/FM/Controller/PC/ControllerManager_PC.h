#pragma once

// TODO: horrible code everywhere. refactor it

#include "controller/ControllerManagerSharedData.h"
#include <windows.h>
#include <Xinput.h>

// TODO: make it static const inside the class

#define TriggerMaxValue 255
#define ThumbStickMaxValue 32767

namespace fm
{
	// NOTE: this is used as int below. so cannot use class
	// TODO: can we make it as enum class?
	enum KeyType
	{
		END = -1,
		SPECIAL = -2,
	};

	struct KeyboardInputToVirtualkey
	{
		Keyboard Input;
		int VirtualKey;
	};

	struct ControllerInputToControllerKey
	{
		GamePad Input;
		int Controllerkey;
	};

	static const KeyboardInputToVirtualkey mVirtualKeyLookUp[] =
	{
		{ Keyboard::A, 0x41 },
		{ Keyboard::B, 0x42 },
		{ Keyboard::C, 0x43 },
		{ Keyboard::D, 0x44 },
		{ Keyboard::E, 0x45 },
		{ Keyboard::F, 0x46 },
		{ Keyboard::G, 0x47 },
		{ Keyboard::H, 0x48 },
		{ Keyboard::I, 0x49 },
		{ Keyboard::J, 0x4A },
		{ Keyboard::K, 0x4B },
		{ Keyboard::L, 0x4C },
		{ Keyboard::M, 0x4D },
		{ Keyboard::N, 0x4E },
		{ Keyboard::O, 0x4F },
		{ Keyboard::P, 0x50 },
		{ Keyboard::Q, 0x51 },
		{ Keyboard::R, 0x52 },
		{ Keyboard::S, 0x53 },
		{ Keyboard::T, 0x54 },
		{ Keyboard::U, 0x55 },
		{ Keyboard::V, 0x56 },
		{ Keyboard::W, 0x57 },
		{ Keyboard::X, 0x58 },
		{ Keyboard::Y, 0x59 },
		{ Keyboard::Z, 0x5A },
		{ Keyboard::BackSpace, VK_BACK },
		{ Keyboard::Tab, VK_TAB },
		{ Keyboard::RightShift, VK_RSHIFT },
		{ Keyboard::LeftShift, VK_LSHIFT },
		{ Keyboard::RightControl, VK_RCONTROL },
		{ Keyboard::LeftControl, VK_LCONTROL },
		{ Keyboard::RightALT, VK_RMENU },
		{ Keyboard::LeftALT, VK_LMENU },
		{ Keyboard::Space, VK_SPACE },
		{ Keyboard::Escape, VK_ESCAPE },
		{ Keyboard::ArrowUp, VK_UP },
		{ Keyboard::ArrowDown, VK_DOWN },
		{ Keyboard::ArrowLeft, VK_LEFT },
		{ Keyboard::ArrowRight, VK_RIGHT },
		{ Keyboard::KeyNone, int(END) }, //this marks the end of the array
	};

	static const ControllerInputToControllerKey mControllerKeyLookUp[] =
	{
		{ GamePad::DPadUp, XINPUT_GAMEPAD_DPAD_UP },
		{ GamePad::DPadDown, XINPUT_GAMEPAD_DPAD_DOWN },
		{ GamePad::DPadLeft, XINPUT_GAMEPAD_DPAD_LEFT },
		{ GamePad::DPadRight, XINPUT_GAMEPAD_DPAD_RIGHT },
		{ GamePad::Start, XINPUT_GAMEPAD_START },
		{ GamePad::Back, XINPUT_GAMEPAD_BACK },
		{ GamePad::LeftThumb, XINPUT_GAMEPAD_LEFT_THUMB },
		{ GamePad::RightThumb, XINPUT_GAMEPAD_RIGHT_THUMB },
		{ GamePad::LeftShoulder, XINPUT_GAMEPAD_LEFT_SHOULDER },
		{ GamePad::RightShoulder, XINPUT_GAMEPAD_RIGHT_SHOULDER },
		{ GamePad::A, XINPUT_GAMEPAD_A },
		{ GamePad::B, XINPUT_GAMEPAD_B },
		{ GamePad::X, XINPUT_GAMEPAD_X },
		{ GamePad::Y, XINPUT_GAMEPAD_Y },
		{ GamePad::LeftTrigger, SPECIAL },
		{ GamePad::RightTrigger, SPECIAL },
		{ GamePad::RightThumbDirection, SPECIAL },
		{ GamePad::LeftThumbDirection, SPECIAL },
		{ GamePad::None, END }, //this marks the end of the array
	};

	class ControllerManagerPC
	{
	public:
		static constexpr int MAX_CHARS_PER_FRAME = 64;

		void SetWindowHandle(const HWND hwnd);
		void HandleMouse(const WPARAM wParam, const LPARAM lParam);
		void HandleMouseDown(const MouseKey key, const bool bDown);
		void HandleInput();

		// TODO: bad function name
		bool ProcessDeadZone(const double deadzone, const double value);

		// TODO: bad function name fix it
		void GetTextBuffer(char* const outBuffer, const uint32 buffsize, uint32& outNumCharsToDelete) const;

	protected:
		static constexpr int MAX_KEYS = 256;

		HWND mWindowHandle = nullptr;
		bool mKeyboardPress[MAX_KEYS];
		XINPUT_STATE mControllerStates[MAX_PLAYERS];
		float mControllerConnection[MAX_PLAYERS];
		char mTextBuffer[MAX_CHARS_PER_FRAME];
		uint32 mNumCharInTextBuffer;
		uint32 mNumCharToDelete;
		bool mbUpperCase = false;
		POINT mMousePos;
		POINT mMouseDelta;
		bool mbMouseDown[3];
	};
}