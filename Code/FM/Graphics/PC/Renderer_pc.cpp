#include "Graphics/Renderer.h"
#include "Graphics/Model.h"
#include "Graphics/FrontEndManager.h"
#include "Graphics/SkinnedMesh.h"

#include "System/Assert.h"
#include "System/Memory.h"

namespace fm
{
	//--------------------
	// Common Renderer
	//--------------------
	void Renderer::Init()
	{
		Assert(mWindowHandle, "call RenderPC::setWindowHandle() before initializing it");

		UINT createDeviceFlags = 0;
#if defined(DEBUG)   
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevel;
		HRESULT hr = D3D11CreateDevice(
			0,                 // default adapter
			mDriverType,
			0,                 // no software device
			createDeviceFlags,
			0, 0,              // default feature level array
			D3D11_SDK_VERSION,
			&mDevice,
			&featureLevel,
			&mContext);

		if (FAILED(hr))
		{
			MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
			return;
		}

		if (featureLevel < D3D_FEATURE_LEVEL_11_0)
		{
			MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
			return;
		}

		// Check 4X MSAA quality support for our back buffer format.
		// All Direct3D 11 capable devices support 4X MSAA for all render 
		// target formats, so we only need to check quality support.
		Verify(S_OK == mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
		Assert(m4xMsaaQuality > 0);

		// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = mClientWidth;
		sd.BufferDesc.Height = mClientHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Use 4X MSAA? 
		if (mEnable4xMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m4xMsaaQuality - 1;
		}
		// No MSAA
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = mWindowHandle;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		// To correctly create the swap chain, we must use the IDXGIFactory that was
		// used to create the device.  If we tried to use a different IDXGIFactory instance
		// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
		// This function is being called with a device from a different IDXGIFactory."
		IDXGIDevice* dxgiDevice = 0;
		Verify(S_OK == mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

		IDXGIAdapter* dxgiAdapter = 0;
		Verify(S_OK == dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

		IDXGIFactory* dxgiFactory = 0;
		Verify(S_OK == dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

		Verify(S_OK == dxgiFactory->CreateSwapChain(mDevice, &sd, &mSwapChain));

		SafeRelease(dxgiDevice);
		SafeRelease(dxgiAdapter);
		SafeRelease(dxgiFactory);

		// The remaining steps that need to be carried out for d3d creation
		// also need to be executed every time the window is resized.  So
		// just call the OnResize method here to avoid code duplication.
		ID3D11Debug *d3dDebug = nullptr;
		if (SUCCEEDED(mDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3dDebug)))
		{
			ID3D11InfoQueue *d3dInfoQueue = nullptr;
			if (SUCCEEDED(d3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)))
			{
#ifdef DEBUG
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
				D3D11_MESSAGE_ID hide[] =
				{
					D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
					// Add more message IDs here as needed
				};

				D3D11_INFO_QUEUE_FILTER filter;
				memset(&filter, 0, sizeof(filter));
				filter.DenyList.NumIDs = _countof(hide);
				filter.DenyList.pIDList = hide;
				d3dInfoQueue->AddStorageFilterEntries(&filter);
				d3dInfoQueue->Release();
			}
			d3dDebug->Release();
		}

		// Resize the swap chain and recreate the render target view.
		Verify(S_OK == mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		ID3D11Texture2D* backBuffer;
		Verify(S_OK == mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
		Verify(S_OK == mDevice->CreateRenderTargetView(backBuffer, 0, &mBackbuffer));
		SafeRelease(backBuffer);

		// Create the depth/stencil buffer and view.
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = mClientWidth;
		depthStencilDesc.Height = mClientHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// Use 4X MSAA? --must match swap chain MSAA values.
		if (mEnable4xMsaa)
		{
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
		}
		// No MSAA
		else
		{
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}

		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		Verify(S_OK == mDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilTex2D));
		Verify(S_OK == mDevice->CreateDepthStencilView(mDepthStencilTex2D, 0, &mDepthStencilView));

		// Bind the render target view and depth/stencil view to the pipeline.
		mContext->OMSetRenderTargets(1, &mBackbuffer, mDepthStencilView);

		// Set the viewport transform.
		mViewPort.TopLeftX = 0;
		mViewPort.TopLeftY = 0;
		mViewPort.Width = static_cast<float>(mClientWidth);
		mViewPort.Height = static_cast<float>(mClientHeight);
		mViewPort.MinDepth = 0.0f;
		mViewPort.MaxDepth = 1.0f;

		mContext->RSSetViewports(1, &mViewPort);

		mCBPerCamera.Create(sizeof(PerCameraConstantBuffer));
	}

	void Renderer::DeInit()
	{
		mRenderElementsIndex = 0;

		SafeRelease(mBackbuffer);
		SafeRelease(mDepthStencilTex2D);
		SafeRelease(mDepthStencilView);
		SafeRelease(mSwapChain);

		if (mContext)
			mContext->ClearState();

		SafeRelease(mContext);
		SafeRelease(mDevice);
	}

	void Renderer::Render()
	{
		float clearColor[] = { 1.f, 0.f, 0.f, 1.f };
		mContext->ClearRenderTargetView(mBackbuffer, clearColor);
		mContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		PerCameraConstantBuffer perCamera;
		perCamera.ViewProjectionTranspose = mViewProjectionMatrix.GetTranspose();
		mContext->UpdateSubresource(mCBPerCamera.mConstantBuffer, 0, NULL, &perCamera, 0, 0);
		mContext->VSSetConstantBuffers(0, 1, &mCBPerCamera.mConstantBuffer);

		for (uint32 i = 0; i < mRenderElementsIndex; ++i)
		{
			RenderElement& element = mRenderElements[i];

			switch (element.MeshData->GetType())
			{
			case Mesh::eMeshType::Static:
				Draw(element);
				break;
			case Mesh::eMeshType::Skinned:
				DrawSkinned(element);
				break;
			default:
				Assert(false, "unknown data type");
				break;
			}

			element.MeshData = nullptr;
		}
		mRenderElementsIndex = 0;
	}

	void Renderer::InitFullScreenQuad()
	{
		D3D11_BUFFER_DESC desc;
		MemSet(&desc, 0, sizeof(desc));
		//desc.Usage = D3D10_USAGE_IMMUTABLE;
	}

	void Renderer::Draw(const RenderElement& element)
	{
		const Mesh* mesh = element.MeshData;

		mContext->IASetVertexBuffers(0, 1, &mesh->mVBStream[0].mVertexBuffer, &mesh->mStride[0], &mesh->mOffset[0]);
		mContext->IASetVertexBuffers(1, 1, &mesh->mVBStream[1].mVertexBuffer, &mesh->mStride[1], &mesh->mOffset[1]);
		mContext->IASetIndexBuffer(mesh->mIndexBuffer.mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		mContext->RSSetState(mesh->mRasterizerState);
		mContext->OMSetBlendState(mesh->mBlendState, blendFactor, 0xFFFFFFFF);
		mContext->OMSetDepthStencilState(mesh->mDepthStencilState, 0);

		PerObjectConstantBuffer perObject;
		perObject.WorldTranspose = element.WorldMatrix.GetTranspose();
		perObject.WorldInvertTranspose = element.WorldInvertTransposeMatrix.GetTranspose();
		mContext->UpdateSubresource(mesh->mCBPerObject.mConstantBuffer, 0, NULL, &perObject, 0, 0);

		// HACK : like this code,  will be deleted
		PerFrameConstantBuffer perFrame;
		Vector3 lightDir(-1.0f, -1.0f, 3.0f);
		lightDir = lightDir.Normalize();
		perFrame.LightDirection = lightDir;
		perFrame.Ambient = 1.0f;
		mContext->UpdateSubresource(mesh->mCBPerFrame.mConstantBuffer, 0, NULL, &perFrame, 0, 0);

		mContext->VSSetShader(mesh->mVertexShader.mVertexShader, NULL, 0);
		mContext->PSSetShader(mesh->mPixelShader.mPixelShader, NULL, 0);
		mContext->VSSetConstantBuffers(1, 1, &mesh->mCBPerObject.mConstantBuffer);
		mContext->PSSetConstantBuffers(0, 1, &mesh->mCBPerFrame.mConstantBuffer);

		mContext->PSSetShaderResources(0, 1, &mesh->mShaderResourceView);
		mContext->PSSetSamplers(0, 1, &mesh->mSamplerState);

		mContext->IASetInputLayout(mesh->mInputLayout.mInputLayout);
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mContext->DrawIndexed(mesh->mNumIndices, 0, 0);
	}

	void Renderer::DrawSkinned(const RenderElement& element)
	{
		const SkinnedMesh* mesh = static_cast<const SkinnedMesh*>(element.MeshData);

		mContext->IASetVertexBuffers(0, 1, &mesh->mVBStream[0].mVertexBuffer, &mesh->mStride[0], &mesh->mOffset[0]);
		mContext->IASetVertexBuffers(1, 1, &mesh->mVBStream[1].mVertexBuffer, &mesh->mStride[1], &mesh->mOffset[1]);
		mContext->IASetIndexBuffer(mesh->mIndexBuffer.mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		mContext->RSSetState(mesh->mRasterizerState);
		mContext->OMSetBlendState(mesh->mBlendState, blendFactor, 0xFFFFFFFF);
		mContext->OMSetDepthStencilState(mesh->mDepthStencilState, 0);

		PerObjectConstantBuffer perObject;
		perObject.WorldTranspose = element.WorldMatrix.GetTranspose();
		perObject.WorldInvertTranspose = element.WorldInvertTransposeMatrix.GetTranspose();
		mContext->UpdateSubresource(mesh->mCBPerObject.mConstantBuffer, 0, NULL, &perObject, 0, 0);

		// TODO : copy and transpose transforms of maximum bones.
		PerSkinnedConstantBuffer perSkinned;
		//perSkinned.BoneTransform = mesh->mSkeleton->mFinalBoneTransforms;
		mContext->UpdateSubresource(mesh->mCBPerSkinned.mConstantBuffer, 0, NULL, &perObject, 0, 0);

		// HACK : like this code,  will be deleted
		PerFrameConstantBuffer perFrame;
		Vector3 lightDir(-1.0f, -1.0f, 3.0f);
		lightDir = lightDir.Normalize();
		perFrame.LightDirection = lightDir;
		perFrame.Ambient = 1.0f;
		mContext->UpdateSubresource(mesh->mCBPerFrame.mConstantBuffer, 0, NULL, &perFrame, 0, 0);

		mContext->VSSetShader(mesh->mVertexShader.mVertexShader, NULL, 0);
		mContext->PSSetShader(mesh->mPixelShader.mPixelShader, NULL, 0);
		mContext->VSSetConstantBuffers(1, 1, &mesh->mCBPerObject.mConstantBuffer);
		mContext->VSSetConstantBuffers(2, 1, &mesh->mCBPerObject.mConstantBuffer);
		mContext->PSSetConstantBuffers(0, 1, &mesh->mCBPerFrame.mConstantBuffer);

		mContext->PSSetShaderResources(0, 1, &mesh->mShaderResourceView);
		mContext->PSSetSamplers(0, 1, &mesh->mSamplerState);

		mContext->IASetInputLayout(mesh->mInputLayout.mInputLayout);
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mContext->DrawIndexed(mesh->mNumIndices, 0, 0);
	}

	void Renderer::DrawRectangle(float xScale, float yScale, float x, float y, const Vector4& color)
	{
		UIVertextConstantBuffer uiTrans;
		Matrix transformation(xScale, 0.0f, 0.0f, 0.0f,
			0.0f, yScale, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f, // don't put 0.0f.
			x, y, 0.0f, 1.0f); // don't put 0.0f.
		uiTrans.Transformation = transformation;
		mContext->UpdateSubresource(FrontEndManager::GetInstance().mVertexConstantBuffer.mConstantBuffer, 0, NULL, &uiTrans, 0, 0);

		UIPixelConstantBuffer uiColor;
		uiColor.Color = color;
		mContext->UpdateSubresource(FrontEndManager::GetInstance().mPixelConstantBuffer.mConstantBuffer, 0, NULL, &uiColor, 0, 0);
		
		mContext->IASetVertexBuffers(0, 1, &FrontEndManager::GetInstance().mVertexBuffer.mVertexBuffer, &FrontEndManager::GetInstance().mStride, &FrontEndManager::GetInstance().mOffset);
		mContext->IASetIndexBuffer(FrontEndManager::GetInstance().mIndexBuffer.mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		mContext->VSSetShader(FrontEndManager::GetInstance().mVertexShader.mVertexShader, NULL, 0);
		mContext->PSSetShader(FrontEndManager::GetInstance().mPixelShader.mPixelShader, NULL, 0);
		mContext->VSSetConstantBuffers(1, 1, &FrontEndManager::GetInstance().mVertexConstantBuffer.mConstantBuffer);
		mContext->PSSetConstantBuffers(1, 1, &FrontEndManager::GetInstance().mPixelConstantBuffer.mConstantBuffer);

		mContext->IASetInputLayout(FrontEndManager::GetInstance().mInputLayout.mInputLayout);
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mContext->DrawIndexed(6, 0, 0); 
	}

	void Renderer::EndRender()
	{
		mSwapChain->Present(0, 0);
	}

	//--------------------
	// PC Renderer
	//--------------------
}