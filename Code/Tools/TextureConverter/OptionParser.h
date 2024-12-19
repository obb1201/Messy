#pragma once
#include <unordered_map>
#include <memory>
#include <assert.h>

using namespace std;

enum OPTIONS    // Note: dwOptions below assumes 64 or less options.
{
	OPT_NONE = 0,
	OPT_WIDTH = 1,
	OPT_HEIGHT,
	OPT_FORMAT,
	OPT_COLOR_MAP,
	OPT_COLOR_MAP_HIGH,
	OPT_NORMAL_MAP,
	OPT_NORMAL_MAP_HIGH,
	OPT_ROUGHNESS_MAP,
	OPT_HDR,
	OPT_SPECULAR_MAP,
	OPT_LOW,
	OPT_MAX
};

class OptionParser
{
public:
	OptionParser();
	~OptionParser() = default;

	OptionParser(const OptionParser&) = delete;
	void operator=(const OptionParser&) = delete;

	static void CreateInstance();
	static void DeleteInstance();
	
	static OptionParser& GetInstance() { assert(mInstance.get() != nullptr); return *mInstance; }

	OPTIONS GetOptionByName(const wstring& optionName);
private:
	unordered_map<wstring, OPTIONS> mOptionValues;
	static unique_ptr<OptionParser> mInstance;
};