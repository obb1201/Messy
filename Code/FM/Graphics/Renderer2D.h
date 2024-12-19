#pragma once

#include "System\Container\Vector.h"
#include "System\Math\Vector4.h"
#include "System\UniquePtr.h"

namespace fm
{
	class Renderer2D final
	{
	public:
		static void CreateInstance();
		static void DeleteInstance();

		static Renderer2D& GetInstance() { Assert(mInstance, "create instance before using it"); return *mInstance; }
		Renderer2D() = default;
		~Renderer2D();

		void BeginRender();
		void Render();
		void EndRender();

		void DrawRectangle(float xScale, float yScale, float x, float y, const Vector4& color);

	private:
		static UniquePtr<Renderer2D> mInstance;
	};
}