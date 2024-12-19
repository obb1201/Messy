#include "OptionParser.h"

unique_ptr<OptionParser> OptionParser::mInstance;

OptionParser::OptionParser()
{
	mOptionValues.clear();
	mOptionValues.insert(make_pair(L"w", OPT_WIDTH));
	mOptionValues.insert(make_pair(L"h", OPT_HEIGHT));
	mOptionValues.insert(make_pair(L"f", OPT_FORMAT));
	mOptionValues.insert(make_pair(L"cm", OPT_COLOR_MAP));
	mOptionValues.insert(make_pair(L"cmhi", OPT_COLOR_MAP_HIGH));
	mOptionValues.insert(make_pair(L"nm", OPT_NORMAL_MAP));
	mOptionValues.insert(make_pair(L"nmhi", OPT_NORMAL_MAP_HIGH));
	mOptionValues.insert(make_pair(L"rm", OPT_ROUGHNESS_MAP));
	mOptionValues.insert(make_pair(L"sp", OPT_SPECULAR_MAP));
	mOptionValues.insert(make_pair(L"hdr", OPT_HDR));
	mOptionValues.insert(make_pair(L"lo", OPT_LOW));
}

void OptionParser::CreateInstance()
{
	//Don't create this twice.
	assert(mInstance == nullptr);

	mInstance = make_unique<OptionParser>();
}

void OptionParser::DeleteInstance()
{
	//don't delete uncreated OptionParser"
	assert(mInstance != nullptr);
	mInstance = nullptr;
}

OPTIONS OptionParser::GetOptionByName(const wstring& optionName)
{
	auto option = mOptionValues.find(optionName);
	if (option == mOptionValues.end())
	{
		return OPT_NONE;
	}

	return option->second;
}