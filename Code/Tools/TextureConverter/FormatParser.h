#pragma once
#include <unordered_map>
#include <memory>
#include <assert.h>
#include <dxgiformat.h>

using namespace std;

class FormatParser
{
public:
public:
	FormatParser();
	~FormatParser() = default;

	FormatParser(const FormatParser&) = delete;
	void operator=(const FormatParser&) = delete;

	static void CreateInstance();
	static void DeleteInstance();

	static FormatParser& GetInstance() { assert(mInstance.get() != nullptr); return *mInstance; }

	DXGI_FORMAT GetFormatByName(const wstring& formatName);
private:
	unordered_map<std::wstring, DXGI_FORMAT> mOptionValues;
	static unique_ptr<FormatParser> mInstance;
};

