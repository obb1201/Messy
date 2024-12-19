#pragma once

#include "ControllerManagerSharedData.h"
#include "ControllerPlatform.h"
#include "System/Assert.h"
#include "System/UniquePtr.h"


namespace fm
{
	// HACK
	struct ControllerPacket
	{
		uint32 playerIndex;
		uint32 actionid;
		Binding keyBinding;
	};

	class ControllerManager final : public ControllerPlatform
	{
	public:
		static void CreateInstance();
		static void DeleteInstance();
		static ControllerManager& GetInstance() { Assert(mInstance, "create instance before using it"); return *mInstance; }

		void SetLocalPlayerIndex(const uint32 playerIndex) { mLocalPlayerIndex = playerIndex; }
		void SetInput(const uint32 playerIndex, const KeyboardBinding* const keyboardBinding, const uint32 keyboardBindingSize,
			const ControllerBinding* const controllerBinding, const uint32 controllerBindingSize,
			const MouseBinding* const mouseBinding, const uint32 mouseBindingSize);

		void SetInputRemote(const ControllerPacket& packet);

		// TODO: bad name.
		bool GetAction(const uint32 playerIndex, const uint32 actionIndex) const;
		void GetActionValues(const uint32 playerIndex, const uint32 actionIndex, float& outDX, float& outDY) const;

		void Update();

		COPY_CONSTRUCTOR(ControllerManager) = delete;
		ASSIGN_OPERATOR(ControllerManager) = delete;

	private:
		static constexpr int MAX_INPUT = 256;

		static UniquePtr<ControllerManager> mInstance;

		uint32 mLocalPlayerIndex;

		Binding mKeyBindings[MAX_PLAYERS][MAX_INPUT];
		KeyboardBinding mKeyboardBinding[MAX_INPUT];
		uint32 mKeyboardBindingSize;
		ControllerBinding mControllerBinding[MAX_PLAYERS][MAX_INPUT];
		uint32 mControllerBindingSize[MAX_PLAYERS];
		MouseBinding mMouseBinding[MAX_INPUT];
		uint32 mMouseBindingSize;

		ControllerManager() = default;

		// updates
		void UpdateMouse();
		void UpdateKeyboard();
		void UpdateController();
	};
}