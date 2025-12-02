#include "DetailedGPUInfo.h"
#include <windows.h>
#include <dxgi.h>
#include <vector>
#include <string>
#include <comdef.h>
#pragma comment(lib,"dxgi.lib")

DetailedGPUInfo::DetailedGPUInfo() {}
DetailedGPUInfo::~DetailedGPUInfo() {}

vector<GPUData>DetailedGPUInfo::get_all_gpus()
{
	vector<GPUData> gpus;
	
	IDXGIFactory* pFactory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) {
		return gpus;
	}
	UINT i = 0;
	IDXGIAdapter* pAdapter = nullptr;

	while(pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
			DXGI_ADAPTER_DESC desc;
			pAdapter->GetDesc(&desc);

			GPUData gpu;
			gpu.index = i;

			//convert wide string to string
			wstring ws(desc.Description);
			gpu.name = string(ws.begin(), ws.end());

			gpu.vram_gb = static_cast<float>(desc.DedicatedVideoMemory) / (1024 * 1024 * 1024);
			gpu.frequency_ghz = 0.0f; //placeholder

			gpus.push_back(gpu);
			pAdapter->Release();
			i++;
	}
	if (pFactory)pFactory->Release();
	return gpus;
}

GPUData DetailedGPUInfo::primary_gpu_info()
{
	auto gpus = get_all_gpus();
	if (!gpus.empty())return gpus[0];
	return GPUData{ -1,"No gpu Found", 0.0f, 0.0f };
}