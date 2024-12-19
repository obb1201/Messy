#include "ModelManager.h"

#include "Animation/Skeleton.h"

#include "Data/IndexData.h"
#include "Data/SkeletonData.h"
#include "Data/CompressedVertexData.h"

#include "System/Memory.h"
#include "Graphics/Model.h"
#include "Graphics/Mesh.h"
#include "Graphics/SkinnedMesh.h"

namespace fm
{
	UniquePtr<ModelManager> ModelManager::mInstance;

	void ModelManager::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = UniquePtr<ModelManager>(fmNew(eMemPool::Resource, "model Manager instance") ModelManager());
	}

	void ModelManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated ModelManager");
		mInstance.reset();
	}

	ModelManager::~ModelManager()
	{
		for (uint32 i = 0; i < mModels.GetSize(); ++i)
		{
			// HACK : there could be holes being null between elements having a model;
			if (mModels[i])
			{
				SafeDelete(mModels[i]);
			}
		}
		mModels.RemoveAll();
	}

	Handle ModelManager::Create(const ModelData& data)
	{
		const uint8* cur = reinterpret_cast<const uint8*>(&data);
		cur += sizeof(ModelData::Header);

		const auto& header = data.Header;
		const auto& infos = reinterpret_cast<const ModelData::MeshInfos*>(cur);

		uintptr currentPosition = reinterpret_cast<uintptr>(cur);
		fixupMeshOffsets(currentPosition, header.NumMeshes);

		// adjust Mesh infos address
		cur += sizeof(ModelData::MeshInfos);

		Model* model = fmNew(eMemPool::Resource, "model instance") Model();
		mModels.Add(model);

		Mesh::eMeshType keepMeshType;
		for (uint32 meshIndex = 0; meshIndex < header.NumMeshes; ++meshIndex)
		{
			// adjust Mesh address
			const auto& mesh = reinterpret_cast<const ModelData::Mesh*>(cur);
			cur += sizeof(ModelData::Mesh);

			// adjust Mesh parts address
			const auto& meshpart = reinterpret_cast<const ModelData::MeshPart*>(cur);
			cur += sizeof(ModelData::MeshPart) * mesh->NumMeshParts;

			void* curVertexData0;
			void* curVertexData1;
			uint32 vbSize0 = 0;
			uint32 vbSize1 = 0;

			// TODO : fix below for 64bit alignment requirement.
			vbSize0 = sizeof(CompressedVertexData::Stream0) * mesh->NumVertices;
			vbSize1 = infos[meshIndex].VBSize - vbSize0;

			curVertexData0 = reinterpret_cast<void*>(infos[meshIndex].VBOffset);
			curVertexData1 = reinterpret_cast<void*>(infos[meshIndex].VBOffset + vbSize0);

			const void* curIndexData = reinterpret_cast<const void*>(infos[meshIndex].IBOffset);
			const uint32 ibSize = infos[meshIndex].IBSize;

			Mesh* newMesh = nullptr;
			keepMeshType = (Mesh::eMeshType)mesh->MeshType;

			switch ((Mesh::eMeshType)mesh->MeshType)
			{
			case Mesh::eMeshType::Static:
			{
				newMesh = fmNew(eMemPool::Resource, "mesh instance") Mesh(curVertexData0, curVertexData1, curIndexData, 
					(Mesh::eMeshType)mesh->MeshType, mesh->NumMeshParts, mesh->NumVertices, mesh->NumIndices, 
					vbSize0, vbSize1, ibSize);
				break;
			}
			case Mesh::eMeshType::Skinned:
			{
				newMesh = fmNew(eMemPool::Resource, "mesh instance") SkinnedMesh(curVertexData0, curVertexData1, curIndexData, 
					(Mesh::eMeshType)mesh->MeshType, mesh->NumMeshParts, mesh->NumVertices, mesh->NumIndices, 
					vbSize0, vbSize1, ibSize);
				break;
			}
			case Mesh::eMeshType::Instanced:
				break;
			default:
				Assert(false, "unknown data type");
				break;
			}
			model->Add(newMesh);

			for (uint32 partIndex = 0; partIndex < mesh->NumMeshParts; ++partIndex)
			{
				Mesh::MeshPartInfo meshPartInfo;
				meshPartInfo.VertexOffset = meshpart[partIndex].VBOffset;
				meshPartInfo.NumVertices = meshpart[partIndex].VBSize;
				meshPartInfo.StartIndex = meshpart[partIndex].IBOffset;
				meshPartInfo.IndexCount = meshpart[partIndex].IBSize;
				meshPartInfo.MaterialID = meshpart[partIndex].MaterialName;
				newMesh->mMeshPartInfos.Add(meshPartInfo);
			}
		}

		if (keepMeshType == Mesh::eMeshType::Skinned)
		{
			const auto& skeletonData = reinterpret_cast<const SkeletonData*>(cur);

			model->mSkeleton = fmNew(eMemPool::Resource, "skeleton instance") Skeleton();

			for (uint32 boneIndex = 0; boneIndex < skeletonData->Header.NumBones; ++boneIndex)
			{
				model->mSkeleton->mBones.Add({ skeletonData->BoneTable.Bones[boneIndex].ID, skeletonData->BoneTable.Bones[boneIndex].ParentID,
					Quaternion(skeletonData->BoneTable.BoneTransforms[boneIndex].Rotation),	skeletonData->BoneTable.BoneTransforms[boneIndex].Translation });
			}

			for (uint32 i = 0; i < model->mMeshes.GetSize(); ++i)
			{
				SkinnedMesh* skinnedMesh = (SkinnedMesh*)model->mMeshes[i];
				skinnedMesh->mSkeleton = model->mSkeleton;
			}
		}

		return mModels.GetSize() - 1;
	}

	void ModelManager::Delete(const Handle handle)
	{
		Assert(handle != INVALID_HANDLE);
		Assert(mModels[handle] != nullptr);

		SafeDelete(mModels[handle]);
	}

	void ModelManager::fixupMeshOffsets(const uintptr currentPosition, const uint32 numMeshes)
	{
		auto* infos = reinterpret_cast<ModelData::MeshInfos*>(currentPosition);

		const uint8* cur = reinterpret_cast<const uint8*>(currentPosition);
		cur += sizeof(ModelData::MeshInfos);

		const uintptr offset = reinterpret_cast<uintptr>(cur);

		for (uint32 meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			infos[meshIndex].VBOffset += offset;
			infos[meshIndex].IBOffset += offset;

			// adjust Mesh address
			const auto& mesh = reinterpret_cast<const ModelData::Mesh*>(cur);
			cur += sizeof(ModelData::Mesh);
			cur += sizeof(ModelData::MeshPart) * mesh->NumMeshParts;

			uintptr currentPosition = reinterpret_cast<uintptr>(cur);

			switch ((Mesh::eMeshType)mesh->MeshType)
			{
			case Mesh::eMeshType::Static:
				break;
			case Mesh::eMeshType::Skinned:
			{
				uintptr nextPosition = fixupSkeletonOffset(currentPosition);
				break;
			}
			case Mesh::eMeshType::Instanced:
				break;
			default:
				Assert(false, "unknown data type");
				break;
			}
		}
	}

	uintptr ModelManager::fixupSkeletonOffset(const uintptr currentPosition)
	{
		auto* skeletonData = reinterpret_cast<SkeletonData*>(currentPosition);

		uintptr ptr = reinterpret_cast<uintptr>(&skeletonData->BoneTable.Names);
		ptrdiff offset = reinterpret_cast<ptrdiff>(skeletonData->BoneTable.Names);
		skeletonData->BoneTable.Names = reinterpret_cast<hashid*>(ptr + offset);

		ptr = reinterpret_cast<uintptr>(&skeletonData->BoneTable.Bones);
		offset = reinterpret_cast<ptrdiff>(skeletonData->BoneTable.Bones);
		skeletonData->BoneTable.Bones = reinterpret_cast<SkeletonData::Bone*>(ptr + offset);

		ptr = reinterpret_cast<uintptr>(&skeletonData->BoneTable.BoneTransforms);
		offset = reinterpret_cast<ptrdiff>(skeletonData->BoneTable.BoneTransforms);
		skeletonData->BoneTable.BoneTransforms = reinterpret_cast<SkeletonData::BoneTransform*>(ptr + offset);

		uintptr nextPosition = reinterpret_cast<uintptr>(skeletonData->BoneTable.BoneTransforms);
		nextPosition += sizeof(SkeletonData::BoneTransform) * skeletonData->Header.NumBones;

		return nextPosition;
	}
}