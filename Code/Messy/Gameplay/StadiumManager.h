#pragma once

#include "System/Hash.h"
#include "System/Assert.h"
#include "System/GlobalInclude.h"
#include "System/UniquePtr.h"
#include "System/Container/Vector.h"

using namespace fm;

namespace messy
{
	class StadiumManager final
	{
		friend class LoadingScreenState;
		FRIEND_UNIQUEPTR(StadiumManager);
	public:
		// singleton-related
		static void CreateInstance();
		static void DeleteInstance();
		static StadiumManager& GetInstance() { Assert(mInstance.get() != nullptr); return *mInstance; }

		COPY_CONSTRUCTOR(StadiumManager) = delete;
		ASSIGN_OPERATOR(StadiumManager) = delete;

		void Initialize();
		void CleanUp();

	private:
		static UniquePtr<StadiumManager> mInstance;

		Handle mPackageHandle = INVALID_HANDLE;
		hashid mStadiumObjectID = INVALID_HASH;

		bool mbLoadHasFinished = false;

		// --- Methods
		StadiumManager() = default;
		~StadiumManager();

		void checkStadiumCreated();
		FORCEINLINE bool isLoadHasFinished() const { return mbLoadHasFinished; }
	};
} 