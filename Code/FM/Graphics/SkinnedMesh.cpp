#include "SkinnedMesh.h"

namespace fm
{
	SkinnedMesh::SkinnedMesh(const void* const vertexData0, const void* const vertexData1, const void* const indexData, const eMeshType meshType, 
		const uint8 numMeshParts, const uint32 numVertex, const uint32 numIndex, 
		const uint32 vertexSize0, const uint32 vertexSize1, const uint32 indexSize)
		: Mesh(vertexData0, vertexData1, indexData, meshType, numMeshParts, numVertex, numIndex, vertexSize0, vertexSize1, indexSize)
	{
		mCBPerSkinned.Create(sizeof(PerSkinnedConstantBuffer));
	}

	SkinnedMesh::~SkinnedMesh()
	{
	}
}