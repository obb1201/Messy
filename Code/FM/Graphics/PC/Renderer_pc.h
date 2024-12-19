#pragma once

#include <d3d11.h>

#include "System\Assert.h"

namespace fm
{
	class RendererPC
	{
	public:
		void SetWindowHandle(const HWND hwnd) { mWindowHandle = hwnd; }

		ID3D11Device& GetDevice() const { Assert(mDevice != nullptr); return *mDevice; }

	protected:
		HWND mWindowHandle = 0;

		IDXGISwapChain* mSwapChain = nullptr;
		ID3D11Device* mDevice = nullptr;
		ID3D11DeviceContext* mContext = nullptr;

		ID3D11RenderTargetView *mBackbuffer = nullptr;
		ID3D11Texture2D* mDepthStencilTex2D = nullptr;
		ID3D11DepthStencilView* mDepthStencilView = nullptr;

		D3D11_VIEWPORT mViewPort;
		D3D_DRIVER_TYPE mDriverType = D3D_DRIVER_TYPE_HARDWARE;
		uint32 m4xMsaaQuality = 0;
		uint32 mClientWidth = 800;
		uint32 mClientHeight = 600;
		bool mEnable4xMsaa = false;

		ID3D11Buffer* mFullscreenQuadVB = nullptr;
		ID3D11Buffer* mFullScreenQuadIB = nullptr;

		RendererPC() = default;
	};
}