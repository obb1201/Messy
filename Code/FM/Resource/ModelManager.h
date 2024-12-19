#pragma once

#include "System/Container/Vector.h"
#include "System/GlobalInclude.h"
#include "System/UniquePtr.h"

#include "Data/ModelData.h"

namespace fm
{
	class Model;
	struct ModelData;

	class ModelManager final
	{
		FRIEND_UNIQUEPTR(ModelManager);
	public:
		Handle Create(const ModelData& data);
		void Delete(const Handle handle);

		FORCEINLINE Model* Get(const Handle handle);

		// -- singleton related
		static void CreateInstance();
		static void DeleteInstance();

		static ModelManager& GetInstance() { Assert(mInstance, "create instance before using it"); return *mInstance; }

		COPY_CONSTRUCTOR(ModelManager) = delete;
		ASSIGN_OPERATOR(ModelManager) = delete;

	private:
		static UniquePtr<ModelManager> mInstance;

		static const uint32 MAX_MODELS = 1024;
		Vector<Model*, MAX_MODELS> mModels;

		// --- methods
		ModelManager() = default;
		~ModelManager();

		void fixupMeshOffsets(const uintptr currentPosition, const uint32 numMeshes);
		uintptr fixupSkeletonOffset(const uintptr currentPosition);
	};

	Model* ModelManager::Get(const Handle handle)
	{
		Assert(handle != INVALID_HANDLE);

		const uint32 index = handle;
		return mModels[index];
	}
}