#pragma once

#include "System\Hash.h"
#include "Graphics\Model.h"

using namespace fm;

namespace messy
{
	class Player final
	{
		friend class PlayerManager;
	public:
		Player() = default;

		void Init();

		void Update();

	private:
		hashid mObjectID;

		// --- method
		Player(const hashid objectid, Model* model);
		~Player();
	};
} 