#include "ControllerManager.h"
#include "System/Memory.h"

namespace fm
{
	UniquePtr<ControllerManager> ControllerManager::mInstance;

	void ControllerManager::SetInput(const uint32 playerIndex, 
		const KeyboardBinding* const keyboardBinding, const uint32 keyboardBindingSize, 
		const ControllerBinding* const controllerBinding, const uint32 controllerBindingSize,
		const MouseBinding* mouseBinding, const uint32 mouseBindingSize)
	{
		Assert(playerIndex < MAX_PLAYERS, "index out of bound");

		mKeyboardBindingSize = keyboardBindingSize;
		mControllerBindingSize[playerIndex] = controllerBindingSize;
		mMouseBindingSize = mouseBindingSize;

		for (auto i = 0U; i < controllerBindingSize; i++)
		{
			const ControllerBinding &controllerBindding = controllerBinding[i];
			
			Assert(controllerBindding.ActionID < MAX_INPUT, "index out of bound");
			Assert(controllerBindding.Deadzone >= 0, "deadzone canot be negative");

			mKeyBindings[playerIndex][controllerBindding.ActionID].Index = i;
			mControllerBinding[playerIndex][i] = controllerBindding;
		}

		for (auto keyboardIndex = 0U; keyboardIndex < keyboardBindingSize; keyboardIndex++)
		{
			mKeyboardBinding[keyboardIndex] = keyboardBinding[keyboardIndex];
			for (auto controllerIndex = 0U; controllerIndex < mControllerBindingSize[playerIndex]; ++controllerIndex)
			{
				ControllerBinding& controllerBinding = mControllerBinding[playerIndex][controllerIndex];
				if (controllerBinding.Key == mKeyboardBinding[keyboardIndex].ControllerKey)
				{
					mKeyBindings[mLocalPlayerIndex][controllerBinding.ActionID].Index = controllerIndex;
					break;
				}
			}
		}

		for (auto i = 0U; i < mouseBindingSize; i++)
		{
			const MouseBinding &mouseBindding = mouseBinding[i];
			
			Assert(mouseBindding.ActionID < MAX_INPUT, "index out of bound");
			
			mKeyBindings[mLocalPlayerIndex][mouseBindding.ActionID].Index = i;
			mMouseBinding[i] = mouseBindding;
		}
	}

	void ControllerManager::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		
		mInstance = UniquePtr<ControllerManager>(fmNew(eMemPool::Audio, "audio instance") ControllerManager());

		MemSet(mInstance->mKeyBindings, 0, sizeof(mInstance->mKeyBindings));
		MemSet(mInstance->mKeyboardBinding, 0, sizeof(mInstance->mKeyboardBinding));
		MemSet(mInstance->mControllerBinding, 0, sizeof(mInstance->mControllerBinding));
		MemSet(mInstance->mMouseBinding, 0, sizeof(mInstance->mMouseBinding));
	}

	void ControllerManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated control manager");
		mInstance = nullptr;
	}

	void ControllerManager::SetInputRemote(const ControllerPacket& packet)
	{
		mKeyBindings[packet.playerIndex][packet.actionid] = packet.keyBinding;
	}

	bool ControllerManager::GetAction(const uint32 playerIndex, const uint32 actionIndex) const
	{
		Assert(playerIndex < MAX_PLAYERS, "index out of bound");

		return mKeyBindings[playerIndex][actionIndex].Value;
	}

	void ControllerManager::GetActionValues(const uint32 playerIndex, const uint32 actionIndex, float& outDX, float& outDY) const
	{
		Assert(playerIndex < MAX_PLAYERS, "index out of bound");

		const Binding &binding = mKeyBindings[playerIndex][actionIndex];
		outDX = binding.Dx;
		outDY = binding.Dy;
	}

	void ControllerManager::Update()
	{
		UpdateMouse();
		UpdateKeyboard();
		UpdateController();
	}

	void ControllerManager::UpdateMouse()
	{
		for (auto i = 0U; i < mMouseBindingSize; i++)
		{
			MouseBinding &mouseBinding = mMouseBinding[i];
			Binding &actiondata = mKeyBindings[mLocalPlayerIndex][mouseBinding.ActionID];

			const int index = int(mouseBinding.Key);
			if (mouseBinding.Key < MouseKey::ButtonLast)
			{
				if (!mouseBinding.Repeat)
				{
					actiondata.Value = (!mouseBinding.Value) && mbMouseDown[index];
				}
				else
				{
					actiondata.Value = mbMouseDown[index];
				}
				mouseBinding.Value = mbMouseDown[index];
			}
			else
			{
				if (mouseBinding.Key == MouseKey::Pos)
				{
					actiondata.Dx = (float)mMousePos.x;
					actiondata.Dy = (float)mMousePos.y;
				}
				else if (mouseBinding.Key == MouseKey::DeltaPos)
				{
					actiondata.Dx = (float)mMouseDelta.x;
					actiondata.Dy = (float)mMouseDelta.y;
				}
			}
		}
	}

	//TODO bind thumbdirection
	void ControllerManager::UpdateKeyboard()
	{
		for (auto i = 0U; i < mKeyboardBindingSize; i++)
		{
			KeyboardBinding &keyboardBinding = mKeyboardBinding[i];
			int index = 0;
			bool found = false;
			while (mVirtualKeyLookUp[index].Input != Keyboard::KeyNone)
			{
				if (mVirtualKeyLookUp[index].Input == keyboardBinding.Key)
				{
					found = true;
					keyboardBinding.Value = mKeyboardPress[index];
					break;
				}
				index++;
			}
			Assert(found, "input not in look up table");
			
			if (keyboardBinding.Value)
			{
				auto controllerIndex = 0U;
				while(controllerIndex < mControllerBindingSize[mLocalPlayerIndex])
				{
					if (keyboardBinding.ControllerKey == mControllerBinding[mLocalPlayerIndex][controllerIndex].Key)
					{
						break;
					}
					++controllerIndex;
				}

				Assert(controllerIndex < mControllerBindingSize[mLocalPlayerIndex], "index out of bound");

				ControllerBinding& controllerBinding = mControllerBinding[mLocalPlayerIndex][controllerIndex];
				Binding &actiondata = mKeyBindings[mLocalPlayerIndex][controllerBinding.ActionID];

				int index = 0;
				while (mControllerKeyLookUp[index].Input != GamePad::None)
				{
					if (mControllerKeyLookUp[index].Input == controllerBinding.Key)
					{
						switch (mControllerKeyLookUp[index].Input)
						{
						case GamePad::LeftTrigger:
							mControllerStates[controllerBinding.ControllerID].Gamepad.bLeftTrigger = static_cast<BYTE>(controllerBinding.Deadzone * TriggerMaxValue);
							break;
						case GamePad::RightTrigger:
							mControllerStates[controllerBinding.ControllerID].Gamepad.bRightTrigger = static_cast<BYTE>(controllerBinding.Deadzone * TriggerMaxValue);
							break;
						default:
							mControllerStates[controllerBinding.ControllerID].Gamepad.wButtons |= mControllerKeyLookUp[index].Controllerkey;
							break;
						}

						break;
					}
					++index;
				}
			}
		}
	}

	void ControllerManager::UpdateController()
	{
		for (auto u = 0U; u < MAX_PLAYERS; ++u)
		{
			for (auto i = 0U; i < mControllerBindingSize[u]; i++)
			{
				ControllerBinding &controllerBinding = mControllerBinding[u][i];
				Binding &actiondata = mKeyBindings[u][controllerBinding.ActionID];
				int index = 0;
				bool found = false;
				while (mControllerKeyLookUp[index].Input != GamePad::None)
				{
					if (mControllerKeyLookUp[index].Input == controllerBinding.Key)
					{
						found = true;
						bool bPress = false;
						double distanceSquared = 0;

						switch (mControllerKeyLookUp[index].Input)
						{
						case GamePad::LeftTrigger:
							bPress = mControllerStates[controllerBinding.ControllerID].Gamepad.bLeftTrigger >= (controllerBinding.Deadzone * TriggerMaxValue);
							break;
						case GamePad::RightTrigger:
							bPress = mControllerStates[controllerBinding.ControllerID].Gamepad.bRightTrigger >= (controllerBinding.Deadzone * TriggerMaxValue);
							break;
						case GamePad::LeftThumbDirection:
							distanceSquared = controllerBinding.Deadzone * ThumbStickMaxValue;
							distanceSquared *= distanceSquared;
							actiondata.Dx = mControllerStates[controllerBinding.ControllerID].Gamepad.sThumbLX;
							actiondata.Dy = mControllerStates[controllerBinding.ControllerID].Gamepad.sThumbLY;
							bPress = ProcessDeadZone(distanceSquared, (actiondata.Dx * actiondata.Dx) + (actiondata.Dy * actiondata.Dy));
							break;
						case GamePad::RightThumbDirection:
							distanceSquared = controllerBinding.Deadzone * ThumbStickMaxValue;
							distanceSquared *= distanceSquared;
							actiondata.Dx = mControllerStates[controllerBinding.ControllerID].Gamepad.sThumbRX;
							actiondata.Dy = mControllerStates[controllerBinding.ControllerID].Gamepad.sThumbRY;
							bPress = ProcessDeadZone(distanceSquared, (actiondata.Dx * actiondata.Dx) + (actiondata.Dy * actiondata.Dy));
							break;
						default:
							bPress = (mControllerStates[controllerBinding.ControllerID].Gamepad.wButtons & mControllerKeyLookUp[index].Controllerkey) != 0;
							break;
						}

						if (!controllerBinding.Repeat)
						{
							actiondata.Value = (!controllerBinding.Value) && bPress;
						}
						else
						{
							actiondata.Value = bPress;
						}
						controllerBinding.Value = bPress;
						break;
					}
					index++;
				}
				Assert(found, "input not in look up table");
			}
		}
	}
}