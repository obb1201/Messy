#pragma once

#include "Mesh.h"
#include "Resource/Data/SkeletonData.h"
#include "System/Container/Vector.h"

namespace fm
{
	class Skeleton;

	class SkinnedMesh final : public Mesh
	{
		friend class ModelManager;
		friend class Renderer;
	public:

	protected:
		Skeleton* mSkeleton;
		ConstantBuffer mCBPerSkinned;

		// --- method
		SkinnedMesh() = delete;
		SkinnedMesh(const void* const vertexData0, const void* const vertexData1, const void* const indexData, const eMeshType meshtype, 
			const uint8 numMeshParts, const uint32 numVertices, const uint32 numIndices, 
			const uint32 vertexSize0, const uint32 vertexSize1, const uint32 indexSize);
		~SkinnedMesh();
	};
}