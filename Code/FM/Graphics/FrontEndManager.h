#pragma once

#include "System\Container\Vector.h"
#include "System\UniquePtr.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "FrontEndManager_platform.h"

namespace fm
{
	class FrontEndManager final : public FrontEndManagerPlatform
	{
		// Render Class can access all public, private and protected memner variables/functions.
		friend class Renderer;
		//friend class Mesh;

	public:

		// Singletone
		static void CreateInstance();
		static void DeleteInstance();

		void Init();

		static FrontEndManager& GetInstance() { Assert(mInstance, "create instance before using it"); return *mInstance; }
		FrontEndManager() = default;
		~FrontEndManager();

	protected:

		// HACK : three method will be modified after Rendering Architecture is completed.
		void buildShader();
		void buildRenderState();

	private:

		static UniquePtr<FrontEndManager> mInstance;
	};
}