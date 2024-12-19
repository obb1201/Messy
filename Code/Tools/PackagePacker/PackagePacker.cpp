#include <iostream>
#include "PackagePacker.h"

// Todo : Changing the namespace is changed to C ++ 17
namespace fs = std::tr2::sys;
using namespace std;
using namespace fm;

const int ARGUMENT_LENGTH = 3;
const int SUCCESS = 0;
const int FAIL = 1;

int main(int argc, const char* const args[])
{
	if (argc < ARGUMENT_LENGTH)
	{
		std::cout << "Less the value of the argument" << std::endl;
		Assert(false, "Less the value of the argument");
		return FAIL;
	}
	else if (argc > ARGUMENT_LENGTH)
	{
		std::cerr << "There are many argument" << std::endl;
		Assert(false, "There are many argument");
		return FAIL;
	}

	const char* const inputDirectory = args[1];
	const char* const outputDirectory = args[2];

	PackagePacker packer;
	const vector<const char*> targetFiles = packer.GetTargetFilePaths(inputDirectory);

	packer.CreatePackage(outputDirectory);

	return 0;
}

void PackagePacker::CreatePackage(const char* outputPath)
{
	FILE* pFile;
	fopen_s(&pFile, outputPath, "w");

	if (pFile)
	{
		PackageDataHeader header;
		const uint16 numFiles = static_cast<uint16>(mPaths.size());

		header.NumFiles = numFiles;
		header.Version = 0;

		fwrite(&header, sizeof(PackageData::Header), 1, pFile);

		uint64 nameTableOffset = 0;
		uint64 fileInfoOffset = 0;

		nameTableOffset = sizeof(nameTableOffset) + sizeof(fileInfoOffset);
		fwrite(&nameTableOffset, sizeof(uint64), 1, pFile);

		fileInfoOffset = sizeof(fileInfoOffset) + (numFiles * sizeof(hashid));
		fwrite(&fileInfoOffset, sizeof(uint64), 1, pFile);

		hashid* hashNames = new hashid[numFiles];
		for (uint32 index = 0; index < numFiles; ++index)
		{
			const std::string fileName = extractFileName(mPaths[index]);
			hashNames[index] = HashString(fileName.c_str());
		}
		fwrite(hashNames, sizeof(hashid), numFiles, pFile);
		delete[] hashNames;

		// offset from data beginning position
		uint32 fileOffset = 0;
		PackageData::FileInfo* fileInfos = new PackageData::FileInfo[numFiles];

		for (uint32 index = 0; index < numFiles; ++index)
		{
			const std::string fileName = extractFileName(mPaths[index]);
			fileInfos[index].Size = getFileSize(mPaths[index]);

			fileInfos[index].Type = getFileType(fileName);
			fileInfos[index].Offset = fileOffset;
			memset(fileInfos[index].Reserved, 0, 3);

			fileOffset += fileInfos[index].Size;
		}
		fwrite(fileInfos, sizeof(PackageData::FileInfo), numFiles, pFile);

		for (uint32 index = 0; index < numFiles; ++index)
		{
			FILE* pResourceFile;
			fopen_s(&pResourceFile, mPaths[index], "rb");

			Assert(pResourceFile, "Can't read resource file");

			// Todo : add std::err code

			uint8* buffer = new uint8[fileInfos[index].Size];
			fread(buffer, sizeof(uint8), fileInfos[index].Size, pResourceFile);
			fwrite(buffer, sizeof(uint8), fileInfos[index].Size, pFile);

			fclose(pResourceFile);

			delete[] buffer;
		}
		delete[] fileInfos;
	}
	else
	{
		Assert(pFile, "can't create file");
	}

	fclose(pFile);
}

const vector<const char*> PackagePacker::GetTargetFilePaths(const char* const rootPath)
{
	vector<const char*> result;

	fs::recursive_directory_iterator endit;
	fs::recursive_directory_iterator it(rootPath);

	for (; it != endit; ++it)
	{
		fs::path path = it->path();

		if (!is_directory(path))
		{
			char* filePath = nullptr;

			string fileNameString = path.string();

			filePath = new char[fileNameString.size() + 1];

			strcpy_s(filePath, fileNameString.size() + 1, fileNameString.c_str());

			filePath[fileNameString.size()] = '\0';

			string fileName = extractFileName(filePath);

			if (!strchr(filePath, '_') && getFileType(fileName) != eFileType::Count)
			{
				result.push_back(filePath);
			}
		}
	}

	sort(result.begin(), result.end(), PackagePacker::compareTo);
	mPaths = result;

	return mPaths;
}

const std::string PackagePacker::extractFileName(const char* const absoultePath)
{
	fs::path filePath = fs::path(absoultePath);
	return filePath.filename().string();
}

fm::eFileType PackagePacker::getFileType(const std::string fileName)
{
	fs::path filePath = fs::path(fileName);
	
	std::string extension = filePath.extension().string();
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

	if (extension == ".model")
	{
		return eFileType::Model;
	}
	if (extension == ".tex")
	{
		return eFileType::Texture;
	}

	return eFileType::Count;
}

fm::uint32 PackagePacker::getFileSize(const char* const absoultePath)
{
	fs::path path(absoultePath);
	return static_cast<uint32>(file_size(path));
}

bool PackagePacker::compareTo(const char *const str1, const char *const str2)
{
	return strcmp(str1, str2) < 0;
}

PackagePacker::PackagePacker()
{
}

PackagePacker::~PackagePacker()
{
}