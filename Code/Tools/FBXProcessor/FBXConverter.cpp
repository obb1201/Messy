#include "FBXConverter.h"

#include <iostream>

#include "System/Assert.h"
#include "System/Memory.h"
#include "System/Math/MathUtility.h"

namespace fm
{
	FBXConverter::FBXConverter()
	{
		mFBXManager = FbxManager::Create();
		mFBXManager->SetIOSettings(FbxIOSettings::Create(mFBXManager, IOSROOT));
	}

	FBXConverter::~FBXConverter()
	{
		if (mFBXScene)
		{
			mFBXScene->Destroy();
		}

		if (mFBXManager)
		{
			mFBXManager->Destroy();
		}
	}

	bool FBXConverter::LoadModelFile(const char* const path)
	{
		if (importFBX(path))
		{
			FbxNode * node = mFBXScene->GetRootNode();
			mMeshCount = getMeshCount(*node);
			mNodeCount = node->GetChildCount();

			mIndexBuffers.reserve(mMeshCount);

			// For now, assuming that the node has one attribute and depth of node tree is 1.
			// we do not decide yet how to handle parent-child node structure.

			for (uint32 nodeIndex = 0; nodeIndex < mNodeCount; nodeIndex++)
			{
				const FbxNode* cNode = node->GetChild(nodeIndex);
				const FbxNodeAttribute* nodeAttribute = cNode->GetNodeAttribute();

				if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
				{
					const FbxMesh& const mesh = *((const FbxMesh*)nodeAttribute);

					// TODO : if mesh has some materials, the way to save data will be different from current way 
					// should store vertex data into each materials
					IndexList indices = readVertexBuffer(mesh);

					if (indices.size() == 0)
					{
						std::cerr << "Fail : Vertex Buffer read" << std::endl;
						Assert(false, "Fail: Vertex Buffer read");
					}
					mIndexBuffers.push_back(indices);
				}
				else if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eSkeleton)
				{

				}
			}

			return true;
		}

		return false;
	}

	void FBXConverter::Convert()
	{
		for (const Vertex& vertex : mVertexList)
		{
			CompressedVertexData::Stream0 vertexStream0 = { 0, };
			CompressedVertexData::Stream1 vertexStream1 = { 0, };

			vertexStream0.Position.X = FBXConverter::compressToFloat16(vertex.Position.X);
			vertexStream0.Position.Y = FBXConverter::compressToFloat16(vertex.Position.Y);
			vertexStream0.Position.Z = FBXConverter::compressToFloat16(vertex.Position.Z);

			vertexStream0.TexCoord.U = FBXConverter::compressToFloat16(vertex.TexCoord.U);
			vertexStream0.TexCoord.V = FBXConverter::compressToFloat16(vertex.TexCoord.V);

			vertexStream1.Normal.X = FBXConverter::compressToInt16(vertex.Normal.X, true);
			vertexStream1.Normal.Y = FBXConverter::compressToInt16(vertex.Normal.Y, false);
			vertexStream1.Normal.X = FBXConverter::storeSignbitForZ(vertex.Normal.Z, vertexStream1.Normal.X);
			
			// TODO : calculate dot product
			// result always more than 0.8

			mVertexStream0.push_back(vertexStream0);
			mVertexStream1.push_back(vertexStream1);
		}
	}

	uint32 FBXConverter::getMeshCount(const FbxNode& node)
	{
		uint32 meshCount = 0;
		uint32 nodeCount = node.GetChildCount();

		for (uint32 nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++)
		{
			const FbxNode* cNode = node.GetChild(nodeIndex);
			const FbxNodeAttribute* nodeAttribute = cNode->GetNodeAttribute();

			if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
			{
				meshCount++;
			}
		}

		return meshCount;
	}

	const int32 FBXConverter::findVertexIndex(const Vertex & vertex) const
	{
		uint32 overlapIndex = INVALID_INDEX;
		for (uint32 index = 0; index < mVertexList.size(); ++index)
		{
			if (mVertexList[index] == vertex)
			{
				overlapIndex = index;
				break;
			}
		}
		return overlapIndex;
	}

	void FBXConverter::CreateCompressModelFile(const char * const absolutePath)
	{
		Convert();

		ModelData modelData = { 0, };
		modelData.Header.Version = 0;
		modelData.Header.NumMeshes = mMeshCount;

		modelData.Infos = new ModelData::MeshInfos[mMeshCount];
		MemSet(modelData.Infos, 0, sizeof(ModelData::MeshInfos) * mMeshCount);

		mNumVBStreams = 2;

		// For now, assuming that each mesh has one mesh part with one material.
		// and it is not considered about separating one compressed vertex buffer into steam vb0, vb1
		// TODO : numMeshParts should be changed if mesh has multiple materials. later you can changed numMeshParts at anytime.
		uint32 offset = 0;
		const uint32 meshAndMeshPartsSize = sizeof(ModelData::Mesh) + sizeof(ModelData::MeshPart) * mMarterialCount;

		for (uint32 meshIndex = 0; meshIndex < mMeshCount; ++meshIndex)
		{
			const uint32 vbSize = sizeof(CompressedVertexData::Stream0) * (uint32)mVertexStream0.size() + sizeof(CompressedVertexData::Stream1) * (uint32)mVertexStream1.size();
			const uint32 ibSize = sizeof(IndexData) * (uint32)mIndexBuffers[meshIndex].size();

			offset += meshAndMeshPartsSize;

			modelData.Infos[meshIndex].NumVBStreams = mNumVBStreams;
			modelData.Infos[meshIndex].VBOffset = offset;
			modelData.Infos[meshIndex].VBSize = vbSize;
			modelData.Infos[meshIndex].IBOffset = offset + vbSize;
			modelData.Infos[meshIndex].IBSize = ibSize;
			// TODO : add mesh name.
			//modelData.Infos[meshIndex].Names = HashString(meshname);

			offset += (vbSize + ibSize);
		}

		FBXConverter::createModelFile(absolutePath, modelData, mMeshCount, mMarterialCount, mVertexStream0, mVertexStream1, mIndexBuffers);
	}

	IndexList FBXConverter::readVertexBuffer(const FbxMesh& const mesh)
	{
		IndexList indices;
		int polygonCount = mesh.GetPolygonCount();
		int vertexCounter = 0;

		int vertexCount = mesh.GetPolygonVertexCount();
		vertexCount = vertexCount / polygonCount;

		mMarterialCount = mesh.GetElementMaterialCount();

		if (vertexCount != 3)
		{
			std::cerr << "vertex count of single polygon is Not 3" << std::endl;
			Assert(false, "vertex count of single polygon is not 3");
		}
		else
		{
			int indexCount = 0;
			uint8 index = 0;
			const FbxVector4* const controlPoints = mesh.GetControlPoints();

			for (int pIndex = 0; pIndex < polygonCount; ++pIndex)
			{
				for (int vIndex = 0; vIndex < 3; ++vIndex)
				{
					Vertex vertex = { 0, };
					const int controlIndex = mesh.GetPolygonVertex(pIndex, vIndex);

					vertex.Position.X = controlPoints[controlIndex][0];
					vertex.Position.Y = controlPoints[controlIndex][1];
					vertex.Position.Z = controlPoints[controlIndex][2];

					if (!readNormal(mesh, &vertex, controlIndex, vertexCounter))
					{
						std::cerr << "Can't read normal" << std::endl;
						Assert(false, "Can't read normal");
					}

					if (!readUV(mesh, &vertex, vertexCounter, controlIndex))
					{
						std::cerr << "Can't read UV" << std::endl;
						Assert(false, "Can't read UV");
					}

					if (!readBinormal(mesh, &vertex, vertexCounter, controlIndex))
					{
						std::cerr << "Can't read Binormal" << std::endl;
						Assert(false, "Can't read Binormal");
					}

					if (!readTangent(mesh, &vertex, vertexCounter, controlIndex))
					{
						std::cerr << "Can't read Tangent" << std::endl;
						Assert(false, "Can't read Tangent");
					}

					int32 duplicateIndex = findVertexIndex(vertex);
					if (duplicateIndex == INVALID_INDEX)
					{
						mVertexList.push_back(vertex);
						indices.push_back(index);
						++index;
					}
					else
					{
						indices.push_back(duplicateIndex);
					}

					vertexCounter++;
				}
			}
		}
		return indices;
	}

	bool FBXConverter::readNormal(const FbxMesh& const mesh, Vertex* const vertex, const int controlIndex, const int vertexIndex)
	{
		const FbxLayerElementNormal* const normal = mesh.GetElementNormal();
		const FbxLayerElement::EMappingMode mappingMode = normal->GetMappingMode();

		if (normal)
		{
			if (mappingMode == FbxLayerElement::eByPolygonVertex)
			{
				if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					vertex->Normal.X = normal->GetDirectArray().GetAt(vertexIndex)[0];
					vertex->Normal.Y = normal->GetDirectArray().GetAt(vertexIndex)[1];
					vertex->Normal.Z = normal->GetDirectArray().GetAt(vertexIndex)[2];
				}
				else if (normal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				{
					int index = normal->GetIndexArray().GetAt(vertexIndex);
					vertex->Normal.X = normal->GetDirectArray().GetAt(index)[0];
					vertex->Normal.Y = normal->GetDirectArray().GetAt(index)[1];
					vertex->Normal.Z = normal->GetDirectArray().GetAt(index)[2];
				}
			}
			else
			{
				std::cerr << "this mapping mode is not support(normal element)" << std::endl;
				Assert(false, "this mapping mode is not support(normal element)");
			}
		}
		else
		{
			std::cerr << "missing Normal" << std::endl;
			Assert(false, "missing Normal");
			return false;
		}

		return true;
	}

	bool FBXConverter::readUV(const FbxMesh& const mesh, Vertex* const vertex, const int vertexIndex, const int controlIndex)
	{
		const FbxGeometryElementUV* const uvElement = mesh.GetElementUV();
		const FbxLayerElement::EMappingMode mappingMode = uvElement->GetMappingMode();

		if (uvElement)
		{
			if (mappingMode == FbxGeometryElement::eByPolygonVertex)
			{
				if (uvElement->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					vertex->TexCoord.U = uvElement->GetDirectArray().GetAt(vertexIndex)[0];
					vertex->TexCoord.V = uvElement->GetDirectArray().GetAt(vertexIndex)[1];
				}
				else if (uvElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				{
					int index = uvElement->GetIndexArray().GetAt(vertexIndex);
					vertex->TexCoord.U = uvElement->GetDirectArray().GetAt(index)[0];
					vertex->TexCoord.V = uvElement->GetDirectArray().GetAt(index)[1];
				}
			}
			else
			{
				std::cerr << "this mapping mode is not support(UV element)" << std::endl;
				Assert(false, "this mapping mode is not support(UV eleemnt)");
			}
		}
		else
		{
			std::cerr << "missing UV" << std::endl;
			Assert(false, "missing UV");
			return false;
		}

		return	true;
	}

	bool FBXConverter::readBinormal(const FbxMesh& const mesh, Vertex* const vertex, const int vertexIndex, const int controlIndex)
	{
		const FbxGeometryElementBinormal* const binormal = mesh.GetElementBinormal();

		if (binormal)
		{
			const FbxLayerElement::EMappingMode mappingMode = binormal->GetMappingMode();
			const FbxLayerElement::EReferenceMode referenceMode = binormal->GetReferenceMode();

			if (mappingMode == FbxGeometryElement::eByPolygonVertex)
			{
				if (referenceMode == FbxLayerElement::EReferenceMode::eDirect)
				{
					vertex->Binormal.X = binormal->GetDirectArray().GetAt(vertexIndex)[0];
					vertex->Binormal.Y = binormal->GetDirectArray().GetAt(vertexIndex)[1];
					vertex->Binormal.Z = binormal->GetDirectArray().GetAt(vertexIndex)[2];
				}
				else if (binormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				{
					int index = binormal->GetIndexArray().GetAt(vertexIndex);
					vertex->Binormal.X = binormal->GetDirectArray().GetAt(index)[0];
					vertex->Binormal.Y = binormal->GetDirectArray().GetAt(index)[1];
					vertex->Binormal.Z = binormal->GetDirectArray().GetAt(index)[2];
				}
			}
			else
			{
				std::cerr << "this mapping mode is not support(binormal element)" << std::endl;
				Assert(false, "this mapping mode is not support(binormal element)");
			}
		}
		else
		{
			std::cerr << "missing Binoraml" << std::endl;
			Assert(false, "missing Binormal");
			return false;
		}
		return true;
	}

	const int32 FBXConverter::getBinormalSign(const Vertex vertex)
	{
		Vector normalVector;
		normalVector.X = vertex.Normal.X;
		normalVector.Y = vertex.Normal.Y;
		normalVector.Z = vertex.Normal.Z;

		Vector tangentVector;
		tangentVector.X = vertex.Tangent.X;
		tangentVector.Y = vertex.Tangent.Y;
		tangentVector.Z = vertex.Tangent.Z;

		int32 dotProduct = normalVector.DotProduct(tangentVector);
		int32 sign = 0;
		if (dotProduct > 0)
		{
			sign = 1;
		}
		else if (dotProduct <= 0)
		{
			sign = -1;
		}
		
		return sign;
	}

	bool FBXConverter::readTangent(const FbxMesh& const mesh, Vertex* const vertex, const int vertexIndex, const int controlIndex)
	{
		const FbxGeometryElementTangent* const tangent = mesh.GetElementTangent();

		if (tangent)
		{
			const FbxLayerElement::EMappingMode mappingMode = tangent->GetMappingMode();
			const FbxLayerElement::EReferenceMode referenceMode = tangent->GetReferenceMode();

			if (mappingMode == FbxLayerElement::EMappingMode::eByPolygonVertex)
			{
				if (referenceMode == FbxLayerElement::EReferenceMode::eDirect)
				{
					vertex->Tangent.X = tangent->GetDirectArray().GetAt(vertexIndex)[0];
					vertex->Tangent.Y = tangent->GetDirectArray().GetAt(vertexIndex)[1];
					vertex->Tangent.Z = tangent->GetDirectArray().GetAt(vertexIndex)[2];
				}
				else if (referenceMode == FbxLayerElement::EReferenceMode::eIndexToDirect)
				{
					int index = tangent->GetIndexArray().GetAt(vertexIndex);
					vertex->Tangent.X = tangent->GetDirectArray().GetAt(index)[0];
					vertex->Tangent.Y = tangent->GetDirectArray().GetAt(index)[1];
					vertex->Tangent.Z = tangent->GetDirectArray().GetAt(index)[2];
				}
			}
			else
			{
				std::cerr << "this mapping mode is not support(Tangent element)" << std::endl;
				Assert(false, "this mapping mode is not support(Tangent element)");
			}
		}
		else
		{
			std::cerr << "missing Tangent" << std::endl;
			Assert(false, "missing Tangent");
			return false;
		}

		return true;
	}

	// http://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion
	union Bits
	{
		float f;
		int32_t si;
		uint32_t ui;
	};

	CompressedVertexData::float16 FBXConverter::compressToFloat16(const double value)
	{
		const int shift = 13;
		const int shiftSign = 16;

		const int32_t infN = 0x7F800000; // flt32 infinity
		const int32_t maxN = 0x477FE000; // max flt16 normal as a flt32
		const int32_t minN = 0x38800000; // min flt16 normal as a flt32
		const int32_t signN = 0x80000000; // flt32 sign bit

		const int32_t infC = infN >> shift;
		const int32_t nanN = (infC + 1) << shift; // minimum flt16 nan as a flt32
		const int32_t maxC = maxN >> shift;
		const int32_t minC = minN >> shift;
		const int32_t signC = signN >> shiftSign; // flt16 sign bit

		const int32_t mulN = 0x52000000; // (1 << 23) / minN
		const int32_t mulC = 0x33800000; // minN / (1 << (23 - shift))

		const int32_t subC = 0x003FF; // max flt32 subnormal down shifted
		const int32_t norC = 0x00400; // min flt32 normal down shifted

		const int32_t maxD = infC - maxC - 1;
		const int32_t minD = minC - subC - 1;

		Bits v, s;
		v.f = (float)value;
		uint32_t sign = v.si & signN;
		v.si ^= sign;
		sign >>= shiftSign; // logical shift
		s.si = mulN;
		s.si = (int32_t)(s.f * v.f); // correct subnormals
		v.si ^= (s.si ^ v.si) & -(minN > v.si);
		v.si ^= (infN ^ v.si) & -((infN > v.si) & (v.si > maxN));
		v.si ^= (nanN ^ v.si) & -((nanN > v.si) & (v.si > infN));
		v.ui >>= shift; // logical shift
		v.si ^= ((v.si - maxD) ^ v.si) & -(v.si > maxC);
		v.si ^= ((v.si - minD) ^ v.si) & -(v.si > subC);
		return v.ui | sign;
	}

	void FBXConverter::decompressToFloat(float* __restrict out, const uint16_t in)
	{
		uint32_t t1;
		uint32_t t2;
		uint32_t t3;

		t1 = in & 0x7fff;                       // Non-sign bits 
		t2 = in & 0x8000;                       // Sign bit 
		t3 = in & 0x7c00;                       // Exponent 

		t1 <<= 13;                              // Align mantissa on MSB 
		t2 <<= 16;                              // Shift sign bit into position 

		t1 += 0x38000000;                       // Adjust bias 

		t1 = (t3 == 0 ? 0 : t1);                // Denormals-as-zero 

		t1 |= t2;                               // Re-insert sign bit 

		*((uint32_t*)out) = t1;
	}

	int16 FBXConverter::compressToInt16(const double value, const bool bZeroLastBit)
	{
		// It refers to MonoGame/MonoGame.FrameWork/.../NormalizedShort2.cs
		const float maxPos = 0x7FFF;
		const float minNeg = -maxPos;

		int16 result = (int16)Clamp((float)round(value * maxPos), minNeg, maxPos) & 0xFFFF;
		if (bZeroLastBit)
		{
			result = result & 0xFFFE;
		}

		return result;
	}

	int16 FBXConverter::storeSignbitForZ(const double z, const int16 value)
	{
		return z >= 0.0f ? value | 0x01 : value;
	}

	void FBXConverter::createModelFile(const char* const absolutePath, ModelData& modelData, const uint32 meshCount, const uint32 marterialCount,
		const std::vector<CompressedVertexData::Stream0> stream0, const std::vector<CompressedVertexData::Stream1> stream1, const std::vector<IndexList>IndexBuffers)
	{
		FILE * pFile;

		if (fopen_s(&pFile, absolutePath, "wb") == 0)
		{
			// write model header
			fwrite(&modelData.Header, sizeof(ModelData::Header), 1, pFile);
			fwrite(modelData.Infos, sizeof(ModelData::MeshInfos), meshCount, pFile);

			for (uint32 meshIndex = 0; meshIndex < meshCount; ++meshIndex)
			{
				ModelData::Mesh* meshData = new ModelData::Mesh;
				MemSet(meshData, 0, sizeof(ModelData::Mesh));

				meshData->NumVertices = (uint32)stream0.size();
				meshData->NumIndices = (uint32)IndexBuffers[meshIndex].size();
				meshData->NumMeshParts = marterialCount;

				ModelData::MeshPart* meshParts = new ModelData::MeshPart[meshData->NumMeshParts];
				MemSet(meshParts, 0, sizeof(ModelData::MeshPart) * meshData->NumMeshParts);

				uint32 vbOffset = 0, ibOffset = 0;
				for (uint32 partIndex = 0; partIndex < meshData->NumMeshParts; ++partIndex)
				{
					const uint32 numSubVertices = (uint32)stream0.size();					// TODO : split number of vertices into each mesh part.
					const uint32 numSubIndices = (uint32)IndexBuffers[meshIndex].size();	// TODO : split number of indices into each mesh part.

					meshParts[partIndex].VBOffset = vbOffset;
					meshParts[partIndex].VBSize = numSubVertices;
					meshParts[partIndex].IBOffset = ibOffset;
					meshParts[partIndex].IBSize = numSubIndices;
					// TODO : add material name
					// modelData.Meshpart[partIndex].MaterialName = HashString(materialname);

					vbOffset += numSubVertices;
					ibOffset += numSubIndices;
				}

				fwrite(meshData, sizeof(ModelData::Mesh), 1, pFile);
				fwrite(meshParts, sizeof(ModelData::MeshPart), meshData->NumMeshParts, pFile);

				// write compressed vertex buffers
				fwrite(&stream0[0], sizeof(CompressedVertexData::Stream0), stream0.size(), pFile);
				fwrite(&stream1[0], sizeof(CompressedVertexData::Stream1), stream1.size(), pFile);

				fwrite(&IndexBuffers[meshIndex][0], sizeof(IndexData), IndexBuffers[meshIndex].size(), pFile);

				delete meshData;
				delete[] meshParts;
			}

			fclose(pFile);
		}
		else
		{
			std::cerr << "can't open file" << std::endl;
			Assert(false, "can't open file");
		}
	}

	const std::vector<IndexList>& FBXConverter::GetIndexBuffers() const
	{
		return mIndexBuffers;
	}

	const std::vector<Vertex>& FBXConverter::GetVertexBuffers() const
	{
		return mVertexList;
	}

	const std::vector<CompressedVertexData::Stream0>& FBXConverter::GetCompressedVertexBuffers0() const
	{
		return mVertexStream0;
	}

	const std::vector<CompressedVertexData::Stream1>& FBXConverter::GetCompressedVertexBuffers1() const
	{
		return mVertexStream1;
	}

	uint32 FBXConverter::GetNodeCount() const
	{
		return mNodeCount;
	}
	bool FBXConverter::importFBX(const char* const path)
	{
		FbxImporter* import = FbxImporter::Create(mFBXManager, "");
		bool bSucceeded = import->Initialize(path, -1, mFBXManager->GetIOSettings());

		if (!bSucceeded)
		{
			Assert(false);
			std::cerr << "FbxImporter::Initialize() Failed with Error: "
				<< import->GetStatus().GetErrorString() << std::endl;

			return bSucceeded;
		}

		mFBXScene = FbxScene::Create(mFBXManager, "");
		bSucceeded = import->Import(mFBXScene);
		import->Destroy();

		return bSucceeded;
	}
};