#pragma once

#include "System\Math\Matrix.h"

namespace fm
{
	class Mesh;

	struct RenderElement
	{
		Matrix WorldMatrix;
		Matrix WorldInvertTransposeMatrix;
		const Mesh* MeshData;
	};
}
