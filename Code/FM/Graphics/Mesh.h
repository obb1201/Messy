#pragma once

#include "System\Container\Vector.h"

#include "Mesh_platform.h"

namespace fm
{
	class Mesh : public MeshPlatform
	{
		friend class Model;
		friend class Renderer;
		friend class ModelManager;
	public:
		enum class eMeshType : uint8
		{
			Static,
			Skinned,
			Instanced,

			Count,
		};

		const eMeshType GetType() const { return mMeshType; }

	protected:
		static const uint32 MAXIMUM_MESHPARTS = 16;

		struct MeshPartInfo
		{
			uint16 VertexOffset;
			uint16 NumVertices;
			uint16 StartIndex;
			uint16 IndexCount;
			hashid MaterialID;
		};
		Vector<MeshPartInfo, MAXIMUM_MESHPARTS> mMeshPartInfos;

		eMeshType mMeshType;
		uint8 mNumMeshparts = 0;
		uint32 mNumVertices = 0;
		uint32 mNumIndices = 0;
		bool mbDynamic = false;

		// --- method
		Mesh() = delete;
		Mesh(const void* const vertexData0, const void* const vertexData1, const void* const indexData, const eMeshType meshtype, 
			const uint8 numMeshParts, const uint32 numVertices, const uint32 numIndices, 
			const uint32 vertexSize0, const uint32 vertexSize1, const uint32 indexSize);
		~Mesh();

		// HACK : three method will be modified after Rendering Architecture is completed.
		void buildRenderState();
	};
}