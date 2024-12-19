#include "Graphics/Renderer2D.h"
#include "Graphics/Renderer.h"

namespace fm
{
	UniquePtr<Renderer2D> Renderer2D::mInstance;

	void Renderer2D::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = UniquePtr<Renderer2D>(fmNew(eMemPool::Rendering, "render instance") Renderer2D());
	}

	void Renderer2D::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated sprite renderer");
		mInstance = nullptr;
	}

	Renderer2D::~Renderer2D()
	{
		
	}

	void Renderer2D::BeginRender()
	{

	}

	void Renderer2D::Render()
	{
		Vector4 color;

		color.Set(0.f, 1.f, 0.f, 1.f); 
		DrawRectangle(0.1f, 0.1f, 0.0f, 0.0f, color);

		color.Set(0.f, 0.f, 1.f, 1.f);
		DrawRectangle(0.5f, 0.1f, -1.0f + 0.5f, 1.0f - 0.1f, color);

		color.Set(1.f, 1.f, 1.f, 1.f);
		DrawRectangle(0.5f, 0.1f, -1.0f + 0.5f, -1.0f + 0.1f, color);
	}

	void Renderer2D::EndRender()
	{

	}

	void Renderer2D::DrawRectangle(float xScale, float yScale, float x, float y, const Vector4& color)
	{
		Renderer::GetInstance().DrawRectangle(xScale, yScale, x, y, color);
	}
}