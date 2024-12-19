#include "CmpSkinnedMeshRenderable.h"
#include "ICmpCamera.h"
#include "ICmpEntity.h"

#include "System/Memory.h"
#include "Resource/ModelManager.h"

#include "Animation/AnimationGraph.h"

#include "Graphics/Renderer.h"
#include "Graphics/Model.h"
#include "Graphics/Mesh.h"
#include "Graphics/RenderElement.h"

namespace fm
{
	const static eComponentType sType = eComponentType::SkinnedMesh;

	IComponent* CmpSkinnedMeshRenderable::Create(hashid objectName)
	{
		// TODO(pope) mempoo
		return fmNew(eMemPool::Rendering, "StaticMeshRenerable") CmpSkinnedMeshRenderable(objectName);
	}

	void CmpSkinnedMeshRenderable::Clean(IComponent* component)
	{
		fmDelete(component);
	}

	void CmpSkinnedMeshRenderable::RegisterTypeInfo()
	{
		ICmpRenderable::RegisterComponent(sType);
		GameObjectManager& gom = GameObjectManager::GetInstance();

		gom.RegisterComponentTypeInfo(sType, Create, Clean);
	}

	CmpSkinnedMeshRenderable::CmpSkinnedMeshRenderable(hashid objectName)
		: ICmpRenderable(sType, objectName)
	{
	}

	eMessageResult CmpSkinnedMeshRenderable::HandleMessage(eMessageType type, const void * const params)
	{
		return eMessageResult::Ok;
	}

	void CmpSkinnedMeshRenderable::Update(const float elaspedTime)
	{
		mAnimationGraph->Update(elaspedTime);
	}

	void CmpSkinnedMeshRenderable::CullAndSubmit(const ICmpCamera* currentCamera)
	{
		// TODO : cull with model unit by camera.

		const ICmpEntity* entity = GetEntity();
		const Matrix& worldMatrix = entity->GetTransform();

		for (uint32 i = 0; i < mModel->GetMeshCount(); ++i)
		{
			RenderElement element;
			element.WorldMatrix = worldMatrix; // TODO : mesh local matrix * world matrix for cull.
			element.WorldInvertTransposeMatrix = worldMatrix.GetInvert().GetTranspose();
			element.MeshData = mModel->GetMesh(i);
			Renderer::GetInstance().Submit(element);
		}
	}
}