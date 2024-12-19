#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <filesystem>

#include "Package.h"
#include "System/Assert.h"
#include "System/Memory.h"

namespace fs = std::tr2::sys;

namespace fm
{
	Package::Package(const char* const packageName, const bool bUseTempMemory, const bool bHeaderOnly)
		: mHeaderOnly(bHeaderOnly)
	{
		const size_t size = strlen(packageName);
		Assert(FILENAME_LEN > size, "package name is too long");

		strncpy_s(mPackageName, packageName, size);

		if (mHeaderOnly)
		{
			// TODO: reevalute if headeronly package loading makes sense
			LoadHeader();
		}
		else
		{
			LoadAll(bUseTempMemory);
		}
	}

	Package::~Package()
	{
		fmDeleteArray(reinterpret_cast<uint8*>(mPackageData));
		mPackageData = nullptr;
	}

	void Package::LoadAll(const bool bUseTempMemory)
	{
		Assert(mPackageData == nullptr);

		uintmax_t filesize = fs::file_size(mPackageName);

		FILE* pFile;
		fopen_s(&pFile, mPackageName, "r");

		Assert(pFile != nullptr);

		const eMemPool mempool = bUseTempMemory ? eMemPool::Temp : eMemPool::Resource;
		mPackageData = reinterpret_cast<PackageData*>(fmNew(mempool, "load package") uint8[filesize]);
		fread(mPackageData, filesize, 1, pFile);
		fclose(pFile);

		const uintptr cur = fixupFileInfo();
		fixupFileOffsets(cur);
	}

	void Package::LoadHeader()
	{
		Assert(mPackageData == nullptr);

		FILE* pFile;
		fopen_s(&pFile, mPackageName, "r");

		Assert(pFile != nullptr);

		// load temporary header
		PackageDataHeader tmpHeader;
		fread(&tmpHeader, sizeof(PackageDataHeader), 1, pFile);

		const uint32 tableSize = (sizeof(hashid) + sizeof(PackageData::FileInfo)) * tmpHeader.NumFiles;
		const uint32 numBytes = tableSize + sizeof(PackageDataHeader);

		mPackageData = reinterpret_cast<PackageData*>(fmNew(eMemPool::Resource, "file table") uint8[numBytes]);

		// rewind and reload header + file tables together
		fseek(pFile, 0, SEEK_SET);

		fread(mPackageData, numBytes, 1, pFile);
		fclose(pFile);

		fixupFileInfo();
		fixupFileOffsets(tableSize + sizeof(PackageDataHeader));
	}

	uintptr Package::fixupFileInfo()
	{
		auto& fileTable = mPackageData->FileTable;

		uintptr ptr = reinterpret_cast<uintptr>(&fileTable.Names);
		ptrdiff offset = reinterpret_cast<ptrdiff>(fileTable.Names);
		fileTable.Names = reinterpret_cast<hashid*>(ptr + offset);

		ptr = reinterpret_cast<uintptr>(&fileTable.Infos);
		offset = reinterpret_cast<ptrdiff>(fileTable.Infos);
		fileTable.Infos = reinterpret_cast<PackageData::FileInfo*>(ptr + offset);

		uintptr dataStartPosition = reinterpret_cast<uintptr>(fileTable.Infos);
		dataStartPosition += sizeof(PackageData::FileInfo) * mPackageData->Header.NumFiles;
		
		return dataStartPosition;
	}

	void Package::fixupFileOffsets(const uintptr offset)
	{
		const auto& fileTable = mPackageData->FileTable;
		for (int i = 0; i < mPackageData->Header.NumFiles; ++i)
		{
			fileTable.Infos[i].Offset += offset;
		}
	}

	const void* Package::GetFileByName(const hashid filename) const
	{
		Assert(!mHeaderOnly, "cannot use this funciton when only header is loaded");

		const PackageDataHeader& header = mPackageData->Header;
		const auto& fileTable = mPackageData->FileTable;

		for (int i = 0; i < header.NumFiles; ++i)
		{
			if (fileTable.Names[i] == filename)
			{
				return reinterpret_cast<void*>(fileTable.Infos[i].Offset);
			}
		}

		Assert(false, "file not found");

		return nullptr;
	}

	const void* Package::GetFileByIndex(const uint32 index) const
	{
		Assert(!mHeaderOnly, "cannot use this funciton when only header is loaded");
		Assert(index < mPackageData->Header.NumFiles, "out of index");

		return reinterpret_cast<void*>(mPackageData->FileTable.Infos[index].Offset);
	}

	// TODO: take size and limit to that size. or make memory stream class
	void Package::GetFile(const hashid filename, uint8* buffer)
	{
		Assert(!mHeaderOnly, "cannot use this funciton when only header is loaded");

		const PackageDataHeader& header = mPackageData->Header;
		const auto& fileTable = mPackageData->FileTable;

		for (int i = 0; i < header.NumFiles; ++i)
		{
			if (fileTable.Names[i] == filename)
			{
				FILE * pFile;
				fopen_s(&pFile, mPackageName, "r");
				Assert(pFile, "failed to open a file");

				fseek(pFile, (long)fileTable.Infos[i].Offset, SEEK_SET);
				fread(buffer, fileTable.Infos[i].Size, 1, pFile);
				fclose(pFile);
				break;
			}
		}

		Assert(false, "file not found");
	}
}