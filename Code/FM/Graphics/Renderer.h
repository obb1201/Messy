#pragma once

#include "Renderer_platform.h"
#include "System/Assert.h"
#include "System/Container/Vector.h"
#include "System/UniquePtr.h"
#include "RenderElement.h"
#include "ConstantBuffer.h"

namespace fm
{
	class CmpCamera;

	class Renderer final : public RendererPlatform
	{
	public:
		enum class ClearMask : uint8
		{
			Color = 1 << 0,
			Depth = 1 << 1,
			Stencil = 1 << 2,
			All = Color | Depth | Stencil
		};

		static void CreateInstance();
		static void DeleteInstance();

		static Renderer& GetInstance() { Assert(mInstance, "create instance before using it"); return *mInstance; }
		Renderer() = default;
		~Renderer();

		void Init();
		void BeginRender();
		void Render();
		void EndRender();

		void Draw(const RenderElement& element);
		void DrawSkinned(const RenderElement& element);
		// TODO(pope): make a faster version that takes a float *?
		void SetClearColor(const float r, const float g, const float b, const float a);
		void Clear(const ClearMask mask);

		FORCEINLINE void Submit(const RenderElement& element);
		FORCEINLINE void SetViewProjectMatrix(const Matrix& viewProjectionMatrix);

		void DrawRectangle(float xScale, float yScale, float x, float y, const Vector4& color);

	protected:
		void InitFullScreenQuad();

	private:
		static UniquePtr<Renderer> mInstance;

		static const uint32 MAX_RENDERELEMENTS = 1024;

		// We are using an array instead of a vector to avoid copy constructor being called
		// with any remove function.
		Array<RenderElement, MAX_RENDERELEMENTS> mRenderElements;
		uint32 mRenderElementsIndex = 0;

		Matrix mViewProjectionMatrix;
		ConstantBuffer mCBPerCamera;

		void DeInit();
	};

	void Renderer::Submit(const RenderElement& element)
	{
		mRenderElements[mRenderElementsIndex++] = element;
	}

	void Renderer::SetViewProjectMatrix(const Matrix& viewProjectionMatrix)
	{
		mViewProjectionMatrix = viewProjectionMatrix;
	}
}