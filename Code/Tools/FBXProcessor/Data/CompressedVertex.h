#pragma once
#include "Resource\Data\CompressedVertexData.h"

#pragma once
namespace fm 
{
#pragma pack(push)
#pragma pack(1)
	struct CompressedVertex
	{
		CompressedVertexData::Stream0* Stream0;
		CompressedVertexData::Stream1* Stream1;
	};
#pragma pack(pop)
}