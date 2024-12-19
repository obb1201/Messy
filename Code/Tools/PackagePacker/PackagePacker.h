#pragma once

#include <vector>
#include <filesystem>
#include <fstream>

#include "Resource/Data/PackageData.h"
#include "System/GlobalInclude.h"
#include "System/Hash.h"
#include "System/File/FileType.h"
#include "System/Assert.h"

class PackagePacker
{
public:
	PackagePacker();
	~PackagePacker();

	const std::vector<const char*> GetTargetFilePaths(const char* const rootPath);
	void CreatePackage(const char* outputPath);
private:
	std::vector<const char*> mPaths;

	static const std::string extractFileName(const char* const absoultePath);
	static fm::eFileType getFileType(const std::string fileName);
	static fm::uint32 getFileSize(const char* const absoultePath);

	static bool compareTo(const char *const str1, const char *const str2);
};