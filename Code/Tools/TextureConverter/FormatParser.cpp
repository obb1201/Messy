#include "FormatParser.h"
#include <dxgiformat.h>

unique_ptr<FormatParser> FormatParser::mInstance;

#define DEFFMT(fmt) { L#fmt, DXGI_FORMAT_ ## fmt }

FormatParser::FormatParser()
{
	mOptionValues.clear();
	// List does not include _TYPELESS or depth/stencil formats
	mOptionValues.insert(DEFFMT(R32G32B32A32_FLOAT));
	mOptionValues.insert(DEFFMT(R32G32B32A32_UINT));
	mOptionValues.insert(DEFFMT(R32G32B32A32_SINT));
	mOptionValues.insert(DEFFMT(R32G32B32_FLOAT));
	mOptionValues.insert(DEFFMT(R32G32B32_UINT));
	mOptionValues.insert(DEFFMT(R32G32B32_SINT));
	mOptionValues.insert(DEFFMT(R16G16B16A16_FLOAT));
	mOptionValues.insert(DEFFMT(R16G16B16A16_UNORM));
	mOptionValues.insert(DEFFMT(R16G16B16A16_UINT));
	mOptionValues.insert(DEFFMT(R16G16B16A16_SNORM));
	mOptionValues.insert(DEFFMT(R16G16B16A16_SINT));
	mOptionValues.insert(DEFFMT(R32G32_FLOAT));
	mOptionValues.insert(DEFFMT(R32G32_UINT));
	mOptionValues.insert(DEFFMT(R32G32_SINT));
	mOptionValues.insert(DEFFMT(R10G10B10A2_UNORM));
	mOptionValues.insert(DEFFMT(R10G10B10A2_UINT));
	mOptionValues.insert(DEFFMT(R11G11B10_FLOAT));
	mOptionValues.insert(DEFFMT(R8G8B8A8_UNORM));
	mOptionValues.insert(DEFFMT(R8G8B8A8_UNORM_SRGB));
	mOptionValues.insert(DEFFMT(R8G8B8A8_UINT));
	mOptionValues.insert(DEFFMT(R8G8B8A8_SNORM));
	mOptionValues.insert(DEFFMT(R8G8B8A8_SINT));
	mOptionValues.insert(DEFFMT(R16G16_FLOAT));
	mOptionValues.insert(DEFFMT(R16G16_UNORM));
	mOptionValues.insert(DEFFMT(R16G16_UINT));
	mOptionValues.insert(DEFFMT(R16G16_SNORM));
	mOptionValues.insert(DEFFMT(R16G16_SINT));
	mOptionValues.insert(DEFFMT(R32_FLOAT));
	mOptionValues.insert(DEFFMT(R32_UINT));
	mOptionValues.insert(DEFFMT(R32_SINT));
	mOptionValues.insert(DEFFMT(R8G8_UNORM));
	mOptionValues.insert(DEFFMT(R8G8_UINT));
	mOptionValues.insert(DEFFMT(R8G8_SNORM));
	mOptionValues.insert(DEFFMT(R8G8_SINT));
	mOptionValues.insert(DEFFMT(R16_FLOAT));
	mOptionValues.insert(DEFFMT(R16_UNORM));
	mOptionValues.insert(DEFFMT(R16_UINT));
	mOptionValues.insert(DEFFMT(R16_SNORM));
	mOptionValues.insert(DEFFMT(R16_SINT));
	mOptionValues.insert(DEFFMT(R8_UNORM));
	mOptionValues.insert(DEFFMT(R8_UINT));
	mOptionValues.insert(DEFFMT(R8_SNORM));
	mOptionValues.insert(DEFFMT(R8_SINT));
	mOptionValues.insert(DEFFMT(A8_UNORM));
	mOptionValues.insert(DEFFMT(R9G9B9E5_SHAREDEXP));
	mOptionValues.insert(DEFFMT(R8G8_B8G8_UNORM));
	mOptionValues.insert(DEFFMT(G8R8_G8B8_UNORM));
	mOptionValues.insert(DEFFMT(BC1_UNORM));
	mOptionValues.insert(DEFFMT(BC1_UNORM_SRGB));
	mOptionValues.insert(DEFFMT(BC2_UNORM));
	mOptionValues.insert(DEFFMT(BC2_UNORM_SRGB));
	mOptionValues.insert(DEFFMT(BC3_UNORM));
	mOptionValues.insert(DEFFMT(BC3_UNORM_SRGB));
	mOptionValues.insert(DEFFMT(BC4_UNORM));
	mOptionValues.insert(DEFFMT(BC4_SNORM));
	mOptionValues.insert(DEFFMT(BC5_UNORM));
	mOptionValues.insert(DEFFMT(BC5_SNORM));
	mOptionValues.insert(DEFFMT(B5G6R5_UNORM));
	mOptionValues.insert(DEFFMT(B5G5R5A1_UNORM));

	// DXGI 1.1 formats
	mOptionValues.insert(DEFFMT(B8G8R8A8_UNORM));
	mOptionValues.insert(DEFFMT(B8G8R8X8_UNORM));
	mOptionValues.insert(DEFFMT(R10G10B10_XR_BIAS_A2_UNORM));
	mOptionValues.insert(DEFFMT(B8G8R8A8_UNORM_SRGB));
	mOptionValues.insert(DEFFMT(B8G8R8X8_UNORM_SRGB));
	mOptionValues.insert(DEFFMT(BC6H_UF16));
	mOptionValues.insert(DEFFMT(BC6H_SF16));
	mOptionValues.insert(DEFFMT(BC7_UNORM));
	mOptionValues.insert(DEFFMT(BC7_UNORM_SRGB));
		
	// DXGI 1.2 formats
	mOptionValues.insert(DEFFMT(AYUV));
	mOptionValues.insert(DEFFMT(Y410));
	mOptionValues.insert(DEFFMT(Y416));
	mOptionValues.insert(DEFFMT(YUY2));
	mOptionValues.insert(DEFFMT(Y210));
	mOptionValues.insert(DEFFMT(Y216));
	// No support for legacy paletted video formats (AI44, IA44, P8, A8P8)
		
	mOptionValues.insert(DEFFMT(B4G4R4A4_UNORM));
}

void FormatParser::CreateInstance()
{
	//Don't create this twice.
	assert(mInstance == nullptr);

	mInstance = make_unique<FormatParser>();
}

void FormatParser::DeleteInstance()
{
	//don't delete uncreated OptionParser"
	assert(mInstance != nullptr);
	mInstance = nullptr;
}

DXGI_FORMAT FormatParser::GetFormatByName(const wstring& formatName)
{
	auto format = mOptionValues.find(formatName);
	if (format == mOptionValues.end())
	{
		return DXGI_FORMAT_UNKNOWN;
	}

	return format->second;
}