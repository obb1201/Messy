#pragma once

#include "System/Hash.h"
#include "System/GlobalInclude.h"

#pragma pack(push)
#pragma pack(1)

namespace fm
{
	struct ModelData
	{
		enum class eMeshType : uint8;

		struct Header
		{
			uint8 Version;
			uint16 NumMeshes;
			uint8 Reserved[5];
		};
		static_assert(sizeof(Header) == 8, "misaligned Model data header");

		struct MeshInfos
		{
			hashid Names;
			uintptr VBOffset;		// memory offset
			uint32 VBSize;			// memory size
			uintptr IBOffset;
			uint32 IBSize;
			uint8 NumVBStreams;
			uint8 Reserved[7];
		};
		static_assert(sizeof(MeshInfos) == 40, "misaligned Model info");

		struct Mesh
		{
			eMeshType MeshType;
			uint8 NumMeshParts;
			uint16 NumVertices;
			uint16 NumIndices;
			uint8 Reserved[2];
		};
		static_assert(sizeof(Mesh) == 8, "misaligned mesh");

		struct MeshPart
		{
			uint16 VBOffset;		// offset of vertex's start location
			uint16 VBSize;			// number of vertices
			uint16 IBOffset;		// offset of index's start location
			uint16 IBSize;			// number of indices
			hashid MaterialName;
		};
		static_assert(sizeof(MeshPart) == 16, "misaligned mesh part");

		Header Header;
		MeshInfos* Infos;	// Infos[NumMeshes]

		// --- example of Model Data File Structure ---
		// Header
		// MeshInfos[num]
		// Mesh1
		// MeshParts[num]
		// Data
		// Mesh2
		// MeshParts[num]
		// Data
	};
}
#pragma pack(pop)
