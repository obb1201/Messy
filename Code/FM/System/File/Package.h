#pragma once

#include "Resource/Data/PackageData.h"
#include "System/Assert.h"
#include "System/Hash.h"
#include "System/File/FileType.h"

namespace fm
{
	class Package
	{
	public:
		friend class PackageManager;

		// if mHeaderOnly == true, offset is file offset.
		// if mHeaderOnly == false, offset is pointer. 
		// file offsets start with 0 when they're saved.
		// file offset or pointer have to be adjusted after they're loaded.
		Package(const char* const package, const bool bUseTempMemory, const bool bHeaderOnly = false);
		~Package();

		COPY_CONSTRUCTOR(Package) = delete;
		ASSIGN_OPERATOR(Package) = delete;
		
		// these functions work differently defending on mHeaderOnly
		const void* GetFileByName(const hashid filename) const;
		const void* GetFileByIndex(const uint32 index) const;
		void GetFile(const hashid filename, uint8* buffer);

		bool IsHeaderOnly() const { return mHeaderOnly; }
		const PackageDataHeader& GetHeader() const { return mPackageData->Header; }

	private:
		const static int FILENAME_LEN = 64;

		PackageData* mPackageData = nullptr;
		char mPackageName[FILENAME_LEN];
		bool mHeaderOnly;
		bool mIsLoaded;

		Package() = delete;

		// constructor calls one of them
		void LoadAll(const bool bUseTempMemory);		// when mHeaderOnly = false;
		void LoadHeader();								// when mHeaderOnly = true;

		// returns offset to where file data is stored
		uintptr fixupFileInfo();
		void fixupFileOffsets(const uintptr offset);
	};
}
