#pragma once

#include <fbxsdk.h>
#include <fbxsdk/fileio/fbxiosettings.h>
#include <fbxsdk/scene/geometry/fbxgeometry.h>
#include <vector>

#include "Resource/Data/IndexData.h"
#include "System/GlobalInclude.h"
#include "Data/Vector.h"
#include "Data/Vertex.h"
#include "Data/CompressedVertex.h"
#include "Resource/Data/ModelData.h"
#include "Resource/Data/PackageData.h"
#include "System/Hash.h"

namespace fm
{
	typedef std::vector<IndexData> IndexList;

	class FBXConverter final
	{
	public:
		FBXConverter();
		~FBXConverter();

		bool LoadModelFile(const char* const path);
		const std::vector<IndexList>& GetIndexBuffers() const;
		const std::vector<Vertex>& GetVertexBuffers() const;
		const std::vector<CompressedVertexData::Stream0>& GetCompressedVertexBuffers0() const;
		const std::vector<CompressedVertexData::Stream1>& GetCompressedVertexBuffers1() const;

		uint32 GetNodeCount() const;
		void CreateCompressModelFile(const char * const absolutePath);
		void Convert();

		static void decompressToFloat(float* __restrict out, const uint16_t in);
	private:
		FbxManager* mFBXManager = nullptr;
		FbxScene* mFBXScene = nullptr;
		uint32 mNodeCount = 0;
		uint32 mMeshCount = 0;
		uint32 mMarterialCount = 0;
		uint32 mNumVBStreams = 0;
		std::vector<IndexList> mIndexBuffers;
		std::vector<Vertex> mVertexList;
		std::vector<CompressedVertexData::Stream0> mVertexStream0;
		std::vector<CompressedVertexData::Stream1> mVertexStream1;

		bool importFBX(const char* const path);

		IndexList readVertexBuffer(const FbxMesh& const mesh);
		const int32 findVertexIndex(const Vertex& vertex) const;

		static uint32 getMeshCount(const FbxNode& node);
		static bool readUV(const FbxMesh& const mesh, Vertex* const vertex, const int vertexIndex, const int controlIndex);
		static bool readNormal(const FbxMesh& const mesh, Vertex* const vertex, const int controlIndex, const int vertexIndex);
		static bool readTangent(const FbxMesh& const mesh, Vertex* const vertex, const int vertexIndex, const int controlIndex);
		static bool readBinormal(const FbxMesh& const mesh, Vertex* const vertex, const int vertexIndex, const int controlIndex);
		const static int32 getBinormalSign(const Vertex vertex);

		static CompressedVertexData::float16 compressToFloat16(const double value);

		static int16 compressToInt16(const double value, const bool bZeroLastBit);
		static int16 storeSignbitForZ(const double z, const int16 value);

		static void createModelFile(const char* const absolutePath, ModelData& modelData, const uint32 meshCount, const uint32 marterialCount, std::vector<CompressedVertexData::Stream0> vertexBuffer0,
			std::vector<CompressedVertexData::Stream1> vertexBuffer1, const std::vector<IndexList>IndexBuffers);

	};
}