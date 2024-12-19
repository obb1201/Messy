#pragma once
#include <iostream>
#include <wincodec.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <wrl\client.h>

#include "lib\DirectXTex.h"

#include "FormatParser.h"
#include "OptionParser.h"

using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct Conversion
{
	wchar_t Src[MAX_PATH];
	wchar_t Dest[MAX_PATH];
};

inline static bool ispow2(size_t x)
{
	return ((x != 0) && !(x & (x - 1)));
}

bool GetDXGIFactory(_Outptr_ IDXGIFactory1** pFactory)
{
	if (!pFactory)
		return false;

	*pFactory = nullptr;

	typedef HRESULT(WINAPI* pfn_CreateDXGIFactory1)(REFIID riid, _Out_ void **ppFactory);

	static pfn_CreateDXGIFactory1 s_CreateDXGIFactory1 = nullptr;

	if (!s_CreateDXGIFactory1)
	{
		HMODULE hModDXGI = LoadLibrary(L"dxgi.dll");
		if (!hModDXGI)
			return false;

		s_CreateDXGIFactory1 = reinterpret_cast<pfn_CreateDXGIFactory1>(reinterpret_cast<void*>(GetProcAddress(hModDXGI, "CreateDXGIFactory1")));
		if (!s_CreateDXGIFactory1)
			return false;
	}

	return SUCCEEDED(s_CreateDXGIFactory1(IID_PPV_ARGS(pFactory)));
}

bool CreateDevice(int adapter, _Outptr_ ID3D11Device** pDevice)
{
	if (!pDevice)
		return false;

	*pDevice = nullptr;

	static PFN_D3D11_CREATE_DEVICE s_DynamicD3D11CreateDevice = nullptr;

	if (!s_DynamicD3D11CreateDevice)
	{
		HMODULE hModD3D11 = LoadLibrary(L"d3d11.dll");
		if (!hModD3D11)
			return false;

		s_DynamicD3D11CreateDevice = reinterpret_cast<PFN_D3D11_CREATE_DEVICE>(reinterpret_cast<void*>(GetProcAddress(hModD3D11, "D3D11CreateDevice")));
		if (!s_DynamicD3D11CreateDevice)
			return false;
	}

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ComPtr<IDXGIAdapter> pAdapter;
	if (adapter >= 0)
	{
		ComPtr<IDXGIFactory1> dxgiFactory;
		if (GetDXGIFactory(dxgiFactory.GetAddressOf()))
		{
			if (FAILED(dxgiFactory->EnumAdapters(adapter, pAdapter.GetAddressOf())))
			{
				wprintf(L"\nERROR: Invalid GPU adapter index (%d)!\n", adapter);
				return false;
			}
		}
	}

	D3D_FEATURE_LEVEL fl;
	HRESULT hr = s_DynamicD3D11CreateDevice(pAdapter.Get(),
		(pAdapter) ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
		nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
		D3D11_SDK_VERSION, pDevice, &fl, nullptr);
	if (SUCCEEDED(hr))
	{
		if (fl < D3D_FEATURE_LEVEL_11_0)
		{
			D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS hwopts;
			hr = (*pDevice)->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &hwopts, sizeof(hwopts));
			if (FAILED(hr))
				memset(&hwopts, 0, sizeof(hwopts));

			if (!hwopts.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x)
			{
				if (*pDevice)
				{
					(*pDevice)->Release();
					*pDevice = nullptr;
				}
				hr = HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
			}
		}
	}

	if (SUCCEEDED(hr))
	{
		ComPtr<IDXGIDevice> dxgiDevice;
		hr = (*pDevice)->QueryInterface(IID_PPV_ARGS(dxgiDevice.GetAddressOf()));
		if (SUCCEEDED(hr))
		{
			hr = dxgiDevice->GetAdapter(pAdapter.ReleaseAndGetAddressOf());
			if (SUCCEEDED(hr))
			{
				DXGI_ADAPTER_DESC desc;
				hr = pAdapter->GetDesc(&desc);
				if (SUCCEEDED(hr))
				{
					wprintf(L"\n[Using DirectCompute on \"%ls\"]\n", desc.Description);
				}
			}
		}

		return true;
	}
	else
		return false;
}

void SplitNamingOption(const wstring& origin, wchar_t delim, vector<wstring>& elements)
{
	wstringstream ss;
	ss.str(origin);

	elements.clear();
	
	wstring item;
	while (getline(ss, item, delim))
	{
		elements.push_back(item);
	}
}

int wmain(int argc, wchar_t* argv[])
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		wprintf(L"Failed to initialize COM (%08X)\n", hr);
		return 1;
	}

	FormatParser::CreateInstance();
	OptionParser::CreateInstance();

	// Parameters and defaults
	DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
	
	DWORD64 dwOptions = 0;
	DWORD dwNormalMap = 0;
	DWORD dwCompress = TEX_COMPRESS_DEFAULT;
	DWORD dwSRGB = 0;
	DWORD dwFilter = TEX_FILTER_DEFAULT;
	DWORD dwFilterOpts = 0;
	
	int adapter = -1;
	float alphaWeight = 1.f;
	float nmapAmplitude = 1.f;
	bool bc6hbc7 = false;
	bool optLow = false;
	
	vector<Conversion> conversionList;
	for (int indexArgv = 1; indexArgv < argc; indexArgv++)
	{
		PWSTR pArg = argv[indexArgv];
		
		if (L'-' != pArg[0])
		{
			Conversion conversionDatum;
			wcscpy_s(conversionDatum.Src, MAX_PATH, pArg);

			conversionDatum.Dest[0] = 0;
			conversionList.push_back(conversionDatum);
		}
	}

	for (auto conversion = conversionList.begin(); conversion != conversionList.end(); ++conversion)
	{
		if (conversion != conversionList.begin())
		{
			cout << "\n";
		}

		// Load source image
		printf("reading %ls", conversion->Src);
		fflush(stdout);

		wchar_t ext[_MAX_EXT];
		wchar_t fname[_MAX_FNAME];
		_wsplitpath_s(conversion->Src, nullptr, 0, nullptr, 0, fname, _MAX_FNAME, ext, _MAX_EXT);

		TexMetadata info;
		std::unique_ptr<ScratchImage> image(new (std::nothrow) ScratchImage);

		if (wcscmp(ext, L".dds") == 0)
		{
			DWORD ddsFlags = DDS_FLAGS_NONE;
			hr = LoadFromDDSFile(conversion->Src, ddsFlags, &info, *image);
			if (FAILED(hr))
			{
				wprintf(L" FAILED (%x)\n", hr);
				continue;
			}
		}
		else if (wcscmp(ext, L".tga") == 0)
		{
			hr = LoadFromTGAFile(conversion->Src, &info, *image);
			if (FAILED(hr))
			{
				wprintf(L" FAILED (%x)\n", hr);
				continue;
			}
		}
		else
		{
			DWORD dwFilter = TEX_FILTER_DEFAULT;
			DWORD wicFlags = dwFilter;

			hr = LoadFromWICFile(conversion->Src, wicFlags, &info, *image);
			if (FAILED(hr))
			{
				wprintf(L" FAILED (%x)\n", hr);
				continue;
			}
		}

		vector<wstring> namingOptions;
		SplitNamingOption(fname, L'_', namingOptions);

		OptionParser& optionParser = OptionParser::GetInstance();
		for (auto iter = namingOptions.begin(); iter != namingOptions.end(); ++iter)
		{
			OPTIONS option = optionParser.GetOptionByName(iter->c_str());
			dwOptions |= (DWORD64(1) << option);

			// Handle options with additional value parameter
			switch (option)
			{
			case OPT_COLOR_MAP:
				format = DXGI_FORMAT_BC3_UNORM_SRGB;
				break;

			case OPT_COLOR_MAP_HIGH:
				format = DXGI_FORMAT_BC7_UNORM_SRGB;
				break;

			//Normalmap used special function.
			case OPT_NORMAL_MAP:
				format = DXGI_FORMAT_BC4_SNORM;
				dwNormalMap |= CNMAP_CHANNEL_LUMINANCE;
				break;

			case OPT_NORMAL_MAP_HIGH:
				format = DXGI_FORMAT_BC6H_SF16;
				dwNormalMap |= CNMAP_CHANNEL_LUMINANCE;
				break;

			case OPT_ROUGHNESS_MAP:
				//TODO : What format..?
				break;

			case OPT_HDR:
				format = DXGI_FORMAT_BC6H_SF16;
				break;

			case OPT_SPECULAR_MAP:
				format = DXGI_FORMAT_BC4_SNORM;
				break;

			case OPT_LOW:
				optLow = true;
				format = DXGI_FORMAT_BC1_UNORM_SRGB;
				break;
			}
		}

		if (optLow)
		{
			format = DXGI_FORMAT_BC1_UNORM_SRGB;
		}

		if (image->IsAllChannelEqaul())
		{
			format = DXGI_FORMAT_BC4_SNORM;
		}

		DXGI_FORMAT textureformat = (format == DXGI_FORMAT_UNKNOWN) ? info.format : format;

		//for Normal Map
		if (dwOptions & (DWORD64(1) << OPT_NORMAL_MAP))
		{
			unique_ptr<ScratchImage> timage(new (std::nothrow) ScratchImage);
			if (!timage)
			{
				wprintf(L" ERROR: Memory allocation failed\n");
				return 1;
			}

			DXGI_FORMAT nmfmt = textureformat;
			if (IsCompressed(textureformat))
			{
				nmfmt = (dwNormalMap & CNMAP_COMPUTE_OCCLUSION) ? DXGI_FORMAT_R32G32B32A32_FLOAT : DXGI_FORMAT_R32G32B32_FLOAT;
			}

			hr = ComputeNormalMap(image->GetImages(), image->GetImageCount(), image->GetMetadata(), dwNormalMap, nmapAmplitude, nmfmt, *timage);
			if (FAILED(hr))
			{
				wprintf(L" FAILED [normalmap] (%x)\n", hr);
				return 1;
			}

			auto& tinfo = timage->GetMetadata();

			assert(tinfo.format == nmfmt);
			info.format = tinfo.format;

			assert(info.width == tinfo.width);
			assert(info.height == tinfo.height);
			assert(info.depth == tinfo.depth);
			assert(info.arraySize == tinfo.arraySize);
			assert(info.mipLevels == tinfo.mipLevels);
			assert(info.miscFlags == tinfo.miscFlags);
			assert(info.miscFlags2 == tinfo.miscFlags2);
			assert(info.dimension == tinfo.dimension);

			image.swap(timage);
		}


		size_t tMips = 0;

		// --- Generate mips -----------------------------------------------------------
		if (!ispow2(info.width) || !ispow2(info.height) || !ispow2(info.depth))
		{
			if (info.dimension == TEX_DIMENSION_TEXTURE3D)
			{
				if (!tMips)
				{
					tMips = 1;
				}
				else
				{
					wprintf(L"\nERROR: Cannot generate mips for non-power-of-2 volume textures\n");
					return 1;
				}
			}
			else if (!tMips || info.mipLevels != 1)
			{
				wprintf(L"\nWARNING: Not all feature levels support non-power-of-2 textures with mipmaps\n");
			}
		}

		if ((info.mipLevels != tMips) && (info.mipLevels != 1))
		{
			// Mips generation only works on a single base image, so strip off existing mip levels
			std::unique_ptr<ScratchImage> timage(new (std::nothrow) ScratchImage);
			if (!timage)
			{
				wprintf(L"\nERROR: Memory allocation failed\n");
				return 1;
			}

			TexMetadata mdata = info;
			mdata.mipLevels = 1;
			hr = timage->Initialize(mdata);
			if (FAILED(hr))
			{
				wprintf(L" FAILED [copy to single level] (%x)\n", hr);
				return 1;
			}

			if (info.dimension == TEX_DIMENSION_TEXTURE3D)
			{
				for (size_t d = 0; d < info.depth; ++d)
				{
					hr = CopyRectangle(*image->GetImage(0, 0, d), Rect(0, 0, info.width, info.height),
						*timage->GetImage(0, 0, d), TEX_FILTER_DEFAULT, 0, 0);
					if (FAILED(hr))
					{
						wprintf(L" FAILED [copy to single level] (%x)\n", hr);
						return 1;
					}
				}
			}
			else
			{
				for (size_t i = 0; i < info.arraySize; ++i)
				{
					hr = CopyRectangle(*image->GetImage(0, i, 0), Rect(0, 0, info.width, info.height),
						*timage->GetImage(0, i, 0), TEX_FILTER_DEFAULT, 0, 0);
					if (FAILED(hr))
					{
						wprintf(L" FAILED [copy to single level] (%x)\n", hr);
						return 1;
					}
				}
			}

			image.swap(timage);
			info.mipLevels = image->GetMetadata().mipLevels;
		}

		if ((!tMips || info.mipLevels != tMips) && (info.width > 1 || info.height > 1 || info.depth > 1))
		{
			std::unique_ptr<ScratchImage> timage(new (std::nothrow) ScratchImage);
			if (!timage)
			{
				wprintf(L"\nERROR: Memory allocation failed\n");
				return 1;
			}

			if (info.dimension == TEX_DIMENSION_TEXTURE3D)
			{
				hr = GenerateMipMaps3D(image->GetImages(), image->GetImageCount(), image->GetMetadata(), dwFilter | dwFilterOpts, tMips, *timage);
			}
			else
			{
				hr = GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), dwFilter | dwFilterOpts, tMips, *timage);
			}
			if (FAILED(hr))
			{
				wprintf(L" FAILED [mipmaps] (%x)\n", hr);
				return 1;
			}

			auto& tinfo = timage->GetMetadata();
			info.mipLevels = tinfo.mipLevels;

			assert(info.width == tinfo.width);
			assert(info.height == tinfo.height);
			assert(info.depth == tinfo.depth);
			assert(info.arraySize == tinfo.arraySize);
			assert(info.miscFlags == tinfo.miscFlags);
			assert(info.dimension == tinfo.dimension);

			image.swap(timage);
		}

		//Compression
		if (IsCompressed(textureformat))
		{
			const Image* img = image->GetImage(0, 0, 0);
			const size_t imgCount = image->GetImageCount();
			DWORD cflags = dwCompress;

			std::unique_ptr<ScratchImage> timage(new (std::nothrow) ScratchImage);

			ComPtr<ID3D11Device> pDevice;
			switch (textureformat)
			{
			case DXGI_FORMAT_BC6H_TYPELESS:
			case DXGI_FORMAT_BC6H_UF16:
			case DXGI_FORMAT_BC6H_SF16:
			case DXGI_FORMAT_BC7_TYPELESS:
			case DXGI_FORMAT_BC7_UNORM:
			case DXGI_FORMAT_BC7_UNORM_SRGB:
				bc6hbc7 = true;
				{
					if (!CreateDevice(adapter, pDevice.GetAddressOf()))
					{
						printf("Can't create Device \n");
					}
				}
			}

			//Compression
			if (bc6hbc7 && pDevice)
			{
				hr = Compress(pDevice.Get(), img, imgCount, info, textureformat, dwCompress | dwSRGB, alphaWeight, *timage);
			}
			else
			{
				hr = Compress(img, imgCount, info, textureformat, cflags | dwSRGB, 0.5f, *timage);
			}

			if (FAILED(hr))
			{
				wprintf(L" FAILED [compress] (%x)\n", hr);
			}

			auto& tinfo = timage->GetMetadata();
			info.format = tinfo.format;

			assert(info.width == tinfo.width);
			assert(info.height == tinfo.height);
			assert(info.depth == tinfo.depth);
			assert(info.arraySize == tinfo.arraySize);
			assert(info.mipLevels == tinfo.mipLevels);
			assert(info.miscFlags == tinfo.miscFlags);
			assert(info.dimension == tinfo.dimension);

			image.swap(timage);
		}

		// Save Result
		{
			const Image* img = image->GetImage(0, 0, 0);
			const size_t imgCount = image->GetImageCount();
			DWORD cflags = dwCompress;

			Blob blob;
			info = image->GetMetadata();
			hr = SaveToDDSMemory(img, imgCount, info, DDS_FLAGS_NONE, blob);
		
			ofstream outFile;
			wcscat_s(conversion->Dest, fname);
			wcscat_s(conversion->Dest, L".dds");
		
			outFile.open(conversion->Dest, ios::out | ios::binary);
			outFile.write( (char*)blob.GetBufferPointer(), blob.GetBufferSize());
			outFile.close();
		}
	}

	OptionParser::DeleteInstance();
	FormatParser::DeleteInstance();

	return 0;
}
