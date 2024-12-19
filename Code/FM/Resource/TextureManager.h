#pragma once

#include "System/Container/Vector.h"
#include "System/GlobalInclude.h"
#include "System/UniquePtr.h"

namespace fm
{
	class Texture;
	struct TextureData;

	class TextureManager final
	{
		FRIEND_UNIQUEPTR(TextureManager);
	public:
		Handle Create(const TextureData& data);
		void Delete(const Handle handle);

		FORCEINLINE const Texture* Get(const Handle handle) const;

		// -- singleton related
		static void CreateInstance();
		static void DeleteInstance();
		static TextureManager& GetInstance() { Assert(mInstance, "create instance before using it"); return *mInstance; }
		
		COPY_CONSTRUCTOR(TextureManager) = delete;
		ASSIGN_OPERATOR(TextureManager) = delete;

	private:
		static const uint32 MAX_TEXTURES = 1024;
		Vector<Texture*, MAX_TEXTURES> mTextures;

		static UniquePtr<TextureManager> mInstance;

		// --- methods
		TextureManager();
		~TextureManager();
	};

	const Texture* TextureManager::Get(const Handle handle) const
	{
		Assert(handle != INVALID_HANDLE);

		const uint32 index = handle;
		return mTextures[index];
	}
}