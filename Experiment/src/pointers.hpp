#pragma once
#include "common.hpp"

namespace big
{
	using d3d_create_device_and_swapchain_t = HRESULT(WINAPI*)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
	class pointers
	{
	public:
		explicit pointers();
		~pointers();
	public:
		HWND m_hwnd{};

		void* m_example;
	public:
		void* m_swapchain_methods[19];
		void* m_device_methods[44];
		void* m_context_methods[145];
	private:
		bool get_swapchain();
		bool swapchain_found = false;
		HWND m_window;
		IDXGISwapChain* m_swapchain;
		ID3D11Device* m_d3d_device;
		ID3D11DeviceContext* m_d3d_context;
	};

	inline pointers *g_pointers{};
}
