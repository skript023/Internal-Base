#include "common.hpp"
#include "logger.hpp"
#include "pointers.hpp"
#include "memory/all.hpp"

namespace big
{
	pointers::pointers()
	{
		memory::pattern_batch main_batch;

		for (int i = 0;!swapchain_found && i <= 3; i++)
		{
			swapchain_found = this->get_swapchain();
			std::this_thread::sleep_for(100ms);
		}
		
		main_batch.add("Example", "FF E3", [this](memory::handle ptr)
		{
			m_example = ptr.as<decltype(m_example)>();
		});

		main_batch.run(memory::module(nullptr));

		this->m_hwnd = FindWindowW(L"gfx_test", L"Renderer: [DirectX11], Input: [Raw input], 64 bits");
		if (!this->m_hwnd)
			throw std::runtime_error("Failed to find the game's window.");

		g_pointers = this;
	}

	pointers::~pointers()
	{
		g_pointers = nullptr;
	}

	bool pointers::get_swapchain()
	{
		WNDCLASSEX window_class;
		window_class.cbSize = sizeof(WNDCLASSEX);
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpfnWndProc = DefWindowProc;
		window_class.cbClsExtra = 0;
		window_class.cbWndExtra = 0;
		window_class.hInstance = GetModuleHandle(NULL);
		window_class.hIcon = NULL;
		window_class.hCursor = NULL;
		window_class.hbrBackground = NULL;
		window_class.lpszMenuName = NULL;
		window_class.lpszClassName = L"Kiero";
		window_class.hIconSm = NULL;

		::RegisterClassEx(&window_class);

		this->m_window = ::CreateWindow(window_class.lpszClassName, L"Kiero DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, window_class.hInstance, NULL);

		if (this->m_window == NULL)
		{
			return false;
		}

		HMODULE libD3D11 = ::GetModuleHandle(L"d3d11.dll");
		if (libD3D11 == NULL)
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(window_class.lpszClassName, window_class.hInstance);
			return false;
		}

		void* D3D11CreateDeviceAndSwapChain = ::GetProcAddress(libD3D11, "D3D11CreateDeviceAndSwapChain");
		if (D3D11CreateDeviceAndSwapChain == NULL)
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(window_class.lpszClassName, window_class.hInstance);
			return false;
		}

		D3D_FEATURE_LEVEL feature_level;
		const D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1, };

		DXGI_RATIONAL refresh_rate;
		refresh_rate.Numerator = 60;
		refresh_rate.Denominator = 1;

		DXGI_MODE_DESC buffer_desc;
		buffer_desc.Width = 100;
		buffer_desc.Height = 100;
		buffer_desc.RefreshRate = refresh_rate;
		buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC swapchain_desc;
		swapchain_desc.BufferDesc = buffer_desc;
		swapchain_desc.SampleDesc = sampleDesc;
		swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchain_desc.BufferCount = 1;
		swapchain_desc.OutputWindow = this->m_window;
		swapchain_desc.Windowed = 1;
		swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = (d3d_create_device_and_swapchain_t(D3D11CreateDeviceAndSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, feature_levels, 2, D3D11_SDK_VERSION, &swapchain_desc, &this->m_swapchain, &this->m_d3d_device, &feature_level, &this->m_d3d_context);
		if (FAILED(hr))
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(window_class.lpszClassName, window_class.hInstance);
			return false;
		}

		::memcpy(this->m_swapchain_methods, *(void***)this->m_swapchain, sizeof(m_swapchain_methods));
		::memcpy(this->m_device_methods, *(void***)this->m_d3d_device, sizeof(m_device_methods));
		::memcpy(this->m_context_methods, *(void***)this->m_d3d_context, sizeof(m_context_methods));

		this->m_swapchain->Release();
		this->m_swapchain = nullptr;

		this->m_d3d_device->Release();
		this->m_d3d_device = nullptr;

		this->m_d3d_context->Release();
		this->m_d3d_context = nullptr;

		::DestroyWindow(this->m_window);
		::UnregisterClass(window_class.lpszClassName, window_class.hInstance);

		return true;
	}
}