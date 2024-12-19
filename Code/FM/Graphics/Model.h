#pragma once

#include "System/GlobalInclude.h"
#include "System/Container/Vector.h"
#include "System/Hash.h"

#include "Mesh.h"

// Model - a resource to render
namespace fm
{
	class Mesh;
	class Skeleton;

	class Model final
	{
		friend class ModelManager;
	public:
		const uint32 GetMeshCount() const { return mMeshes.GetSize(); }

		FORCEINLINE void Add(Mesh* mesh);
		FORCEINLINE const Mesh* GetMesh(const uint32 index) const;

	private:
		static const uint8 MAXIMUM_MESHES = 16;

		Vector<Mesh*, MAXIMUM_MESHES> mMeshes;
		Skeleton* mSkeleton;

		// --- methods
		Model() = default;
		~Model();
	};

	void Model::Add(Mesh* mesh)
	{
		Assert(mesh != nullptr, "invalid mesh pointer");
		mMeshes.Add(mesh);
	}

	const Mesh* Model::GetMesh(const uint32 index) const
	{
		return mMeshes[index];
	}
}
