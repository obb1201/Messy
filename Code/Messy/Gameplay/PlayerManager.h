#pragma once

#include "System/Hash.h"
#include "System/Assert.h"
#include "System/GlobalInclude.h"
#include "System/UniquePtr.h"
#include "System/Container/Vector.h"

using namespace fm;

namespace messy
{
	class Player;

	class PlayerManager final
	{
		friend class LoadingScreenState;
		FRIEND_UNIQUEPTR(PlayerManager);
	public:
		// singleton-related
		static void CreateInstance();
		static void DeleteInstance();
		static PlayerManager& GetInstance() { Assert(mInstance.get() != nullptr); return *mInstance; }

		COPY_CONSTRUCTOR(PlayerManager) = delete;
		ASSIGN_OPERATOR(PlayerManager) = delete;

		void Initialize();
		void CleanUp();
		void Update();

	private:
		static UniquePtr<PlayerManager> mInstance;
		static const uint32 MAXIMUM_PLAYERS = 22;

		struct PlayerLoadInfo
		{
			Handle PackageHandle;
			hashid ObjectID;
			bool bLoadHasFinished;
		};
		Vector<PlayerLoadInfo, MAXIMUM_PLAYERS> mPlayerLoadInfos;
		Vector<Player*, MAXIMUM_PLAYERS> mPlayers;

		bool mbAllLoadHasFinished = false;

		// --- Methods
		PlayerManager() = default;
		~PlayerManager();

		void checkPlayersCreated();
		FORCEINLINE bool isLoadHasFinished() const { return mbAllLoadHasFinished; }
	};
} 