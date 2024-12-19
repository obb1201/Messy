#include "Renderer.h"
#include "system/Assert.h"
#include "system/memory.h"

namespace fm
{
	 UniquePtr<Renderer> Renderer::mInstance;

	 void Renderer::CreateInstance()
	 {
		 Assert(mInstance == nullptr, "don't create this twice");
		 mInstance = UniquePtr<Renderer>(fmNew(eMemPool::Rendering, "render instance") Renderer);
	 }

	 void Renderer::DeleteInstance()
	 {
		 Assert(mInstance != nullptr, "don't delete uncreated renderer");
		 mInstance = nullptr;
	 }

	 Renderer::~Renderer()
	 {
		 DeInit();
	 }
}