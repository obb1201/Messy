#include "TextureManager.h"

#include "Data/TextureData.h"
#include "Graphics/Texture.h"

#include "System/Memory.h"

namespace fm
{
	UniquePtr<TextureManager> TextureManager::mInstance;

	void TextureManager::CreateInstance()
	{
		Assert(mInstance == nullptr, "don't create this twice");
		mInstance = UniquePtr<TextureManager>(fmNew(eMemPool::Resource, "texture Manager instance") TextureManager());
	}

	void TextureManager::DeleteInstance()
	{
		Assert(mInstance != nullptr, "don't delete uncreated TextureManager");
		mInstance.reset();
	}

	TextureManager::TextureManager()
	{
		//MemSet(mTextures.Elements, 0, StaticArraySize(mTextures.Elements));
	}

	TextureManager::~TextureManager()
	{
		for (uint32 i = 0; i < mTextures.GetSize(); ++i)
		{
			SafeDelete(mTextures[i]);
		}
		mTextures.RemoveAll();
	}

	Handle TextureManager::Create(const TextureData& data)
	{
		const uint8* cur = reinterpret_cast<const uint8*>(&data);
		cur += sizeof(TextureData::Header);

		const auto& header = data.Header;
		Assert(header.ArraySize != 0, "should be more or equal than 1");
		Assert(header.NumMips != 0, "should be more or equal than 1");

		const uint8* surfacedata = reinterpret_cast<const uint8*>(data.Data);

		Texture* texture = fmNew(eMemPool::Resource, "texture instance") Texture(surfacedata, (Texture::eFormat)header.Format,
			header.Width, header.Height, header.Depth, header.ArraySize, header.NumMips);
		mTextures.Add(texture);

		return mTextures.GetSize() - 1;
	}

	void TextureManager::Delete(const Handle handle)
	{
		Assert(handle != INVALID_HANDLE);
		Assert(mTextures[handle] != nullptr);

		SafeDelete(mTextures[handle]);
	}
}