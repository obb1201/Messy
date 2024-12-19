#include <windows.h>
#include <Windowsx.h>

#include "ControllerManager_PC.h"
#include "Controller/ControllerManager.h"
#include "System/Assert.h"

//this is suppose to be defined by something in windows for NEXTRAWINPUTBLOCK, but I couldn't find it
//nobody else seems to have this problem cept one guy and there is like no solution offer in that site.
typedef unsigned __int64 QWORD;

namespace fm
{
	void ControllerManagerPC::SetWindowHandle(const HWND hwnd)
	{
		mWindowHandle = hwnd;

		RAWINPUTDEVICE device;
		{
			device.usUsagePage = 0x01;
			device.usUsage = 0x06;
			device.dwFlags = RIDEV_NOLEGACY;        // do not generate legacy messages such as WM_KEYDOWN
			device.hwndTarget = hwnd;
		}

		if (RegisterRawInputDevices(&device, 1, sizeof(device)) == FALSE)
		{
			Assert(false, "rawinput reg failed");
		}

		for (auto i = 0U; i < MAX_PLAYERS; ++i)
		{
			mControllerConnection[i] = 0;
		}
	}

	/*
	@Param buffer a char array that needs to have capacity of MaxCharPerFrame
	@param numCharsToDelete number of char should be deleted from previous buffer
	*/
	void ControllerManagerPC::GetTextBuffer(char* const outBuffer, const uint32 buffsize, uint32& outNumCharsToDelete) const
	{
		Assert(buffsize >= (mNumCharInTextBuffer + 1), "buffer size too small");
		outNumCharsToDelete = mNumCharToDelete;

		//size +1 for the null
		memcpy(outBuffer, mTextBuffer, (mNumCharInTextBuffer + 1));
	}

	void ControllerManagerPC::HandleInput()
	{
		//CONTROLLER
		for (auto i = 0U; i < MAX_PLAYERS; i++)
		{
			if (mControllerConnection[i] <= 0)
			{
				DWORD dwResult;
				ZeroMemory(&mControllerStates[i], sizeof(XINPUT_STATE));
				dwResult = XInputGetState(i, &mControllerStates[i]);
				if (dwResult != ERROR_SUCCESS)
				{
					// TODO: what is the unit?
					constexpr int TIMEOUT = 180;
					mControllerConnection[i] = TIMEOUT;
				}
			}
			else
			{
				mControllerConnection[i] -= 1;//TODO replace with time whenever
			}
		}

		//KEYBOARD
		uint64 rawInputBuffer[128];
		uint32 bytes = sizeof(rawInputBuffer);

		mNumCharInTextBuffer = 0;
		mNumCharToDelete = 0;

		while (true)
		{
			uint32 count = GetRawInputBuffer((PRAWINPUT)rawInputBuffer, &bytes, sizeof(RAWINPUTHEADER));
			if (count <= 0)
			{
				mTextBuffer[mNumCharInTextBuffer] = 0;
				return;
			}

			const RAWINPUT* raw = (const RAWINPUT*)rawInputBuffer;
			for (auto i = 0U; i < count; ++i)
			{
				Assert((raw->header.dwType == RIM_TYPEKEYBOARD), "Not keyboard");
				const RAWKEYBOARD& rawKB = raw->data.keyboard;
				UINT flags = rawKB.Flags;
				//It has to be check against RI_KEY_BREAK. RI_KEY_MAKE doesn't work for some reason
				const bool bKeyDown = !(flags & RI_KEY_BREAK);
				UINT scanCode = rawKB.MakeCode;
				int virtualkey = rawKB.VKey;
				const bool isE0 = ((flags & RI_KEY_E0) != 0);
				const bool isE1 = ((flags & RI_KEY_E1) != 0); //this is use for something else that i'm not adding yet

				//TEXT input code
				char c = MapVirtualKey(rawKB.VKey, MAPVK_VK_TO_CHAR);
				if (virtualkey == VK_SHIFT)
				{
					mbUpperCase = bKeyDown;
					virtualkey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
				}
				else if (virtualkey == VK_CONTROL)
				{
					if (isE0)
					{
						virtualkey = VK_RCONTROL;
					}
					else
					{
						virtualkey = VK_LCONTROL;
					}
				}
				else if (virtualkey == VK_MENU)
				{
					if (isE0)
					{
						virtualkey = VK_RMENU;
					}
					else
					{
						virtualkey = VK_LMENU;
					}
				}
				else if (bKeyDown)
				{
					if (virtualkey == VK_BACK)
					{
						if (mNumCharInTextBuffer <= 0)
						{
							++mNumCharToDelete;
						}
						else
						{
							--mNumCharInTextBuffer;
						}
					}
					else if (c != 0)
					{
						mTextBuffer[mNumCharInTextBuffer] = c;
						if (!mbUpperCase)
						{
							mTextBuffer[mNumCharInTextBuffer] = tolower(c);
						}
						++mNumCharInTextBuffer;
					}
				}
				//End of Text Input stuff

				int index = 0;
				int lookupvirtualkey = mVirtualKeyLookUp[index].VirtualKey;
				while (lookupvirtualkey != END)
				{
					if (lookupvirtualkey == virtualkey)
					{
						mKeyboardPress[index] = bKeyDown;
						return;
					}
					index++;
					lookupvirtualkey = mVirtualKeyLookUp[index].VirtualKey;
				}
				raw = NEXTRAWINPUTBLOCK(raw);
			}
		}
	}

	void ControllerManagerPC::HandleMouse(const WPARAM wParam, const LPARAM lParam)
	{
		const int x = GET_X_LPARAM(lParam);
		const int y = GET_Y_LPARAM(lParam);

		mMouseDelta.x = x - mMousePos.x;
		mMouseDelta.y = y - mMousePos.y;
		mMousePos.x = x;
		mMousePos.y = y;
	}

	void ControllerManagerPC::HandleMouseDown(const MouseKey key, const bool bDown)
	{
		mbMouseDown[int(key)] = bDown;
	}

	bool ControllerManagerPC::ProcessDeadZone(const double deadzone, const double value)
	{
		return value >= deadzone;
	}
}