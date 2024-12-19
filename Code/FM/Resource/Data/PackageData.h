#pragma once

#include "System/Hash.h"
#include "System/GlobalInclude.h"

namespace fm
{
#pragma pack(push)
#pragma pack(1)
	enum class eFileType : uint8;

	struct PackageDataHeader
	{
		uint8 Version;
		uint16 NumFiles;
		uint8 Reserved[5];
	};

	struct PackageData
	{
		struct FileInfo
		{
			uintptr Offset;
			uint32 Size;
			eFileType Type;
			uint8 Reserved[3];
		};

		struct FileTable
		{
			hashid* Names;     // hashid[NumFiles]
			FileInfo* Infos;     // FileInfo[NumFiles]
		};

		PackageDataHeader Header;
		FileTable FileTable;

		static_assert(sizeof(FileInfo) == 16, "FileInfo align error");
		static_assert(sizeof(PackageDataHeader) == 8, "Header align error");
		static_assert(sizeof(FileTable) == 16, "Header align error");
	};
#pragma pack(pop)
}
