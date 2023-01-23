#include "common.hpp"
#include "fonts/font_list.hpp"
#include "logger.hpp"
#include "gui.hpp"
#include "pointers.hpp"
#include "renderer.hpp"

//ImGui Include
#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>
#include <imgui_internal.h>

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace big
{
	renderer::renderer()
	{
		g_renderer = this;
	}

	renderer::~renderer()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();

		g_renderer = nullptr;
	}

	void renderer::on_present()
	{
		if (g_gui.m_opened)
		{
			ImGui::GetIO().MouseDrawCursor = true;
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		}
		else
		{
			ImGui::GetIO().MouseDrawCursor = false;
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (g_gui.m_opened)
		{
			g_gui.dx_on_tick();
		}

		ImGui::Render();
		m_d3d_context->OMSetRenderTargets(1, &m_d3d_render_target, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	bool renderer::init(IDXGISwapChain* swapchain)
	{
		if (SUCCEEDED(swapchain->GetDevice(__uuidof(ID3D11Device), (void**)&m_d3d_device)))
		{
			m_d3d_device->GetImmediateContext(&m_d3d_context);

			DXGI_SWAP_CHAIN_DESC sd;
			swapchain->GetDesc(&sd);
			m_window = sd.OutputWindow;

			ID3D11Texture2D* m_back_buffer;
			swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_back_buffer);
			if (m_back_buffer && m_d3d_device)
			{
				m_d3d_device->CreateRenderTargetView(m_back_buffer, NULL, &m_d3d_render_target);
			}
			else
			{
				return false;
			}

			m_back_buffer->Release();
		}
		else
		{
			return false;
		}

		this->imgui_init();

		this->m_init = true;
		LOG(HACKER) << "Swapchain initialized.";

		return true;
	}

	void renderer::imgui_init()
	{
		auto file_path = std::filesystem::path(std::getenv("appdata"));
		file_path /= "Ellohim";
		if (!std::filesystem::exists(file_path))
		{
			std::filesystem::create_directory(file_path);
		}
		else if (!std::filesystem::is_directory(file_path))
		{
			std::filesystem::remove(file_path);
			std::filesystem::create_directory(file_path);
		}
		file_path /= "imgui.ini";

		ImGuiContext* ctx = ImGui::CreateContext();

		static std::string path = file_path.make_preferred().string();
		ctx->IO.IniFilename = path.c_str();

		ImGui_ImplDX11_Init(m_d3d_device, m_d3d_context);
		ImGui_ImplWin32_Init(m_window);

		ImFontConfig font_cfg{};
		font_cfg.FontDataOwnedByAtlas = false;
		std::strcpy(font_cfg.Name, "Rubik");

		m_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_rubik), sizeof(font_rubik), 20.f, &font_cfg);
		m_monospace_font = ImGui::GetIO().Fonts->AddFontDefault();

		g_gui.dx_init();
	}

	void renderer::pre_reset()
	{
		if (m_d3d_render_target)
		{
			m_d3d_context->OMSetRenderTargets(0, 0, 0);
			m_d3d_render_target->Release();
		}

		ImGui_ImplDX11_InvalidateDeviceObjects();
	}

	void renderer::post_reset(IDXGISwapChain* this_)
	{
		ImGui_ImplDX11_CreateDeviceObjects();

		ID3D11Texture2D* m_back_buffer;
		this_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_back_buffer);

		if (m_back_buffer)
		{
			m_d3d_device->CreateRenderTargetView(m_back_buffer, NULL, &m_d3d_render_target);
		}

		m_back_buffer->Release();
		m_d3d_context->OMSetRenderTargets(1, &m_d3d_render_target, NULL);
	}

	void renderer::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (msg == WM_KEYUP && wparam == VK_INSERT)
		{
			//Persist and restore the cursor position between menu instances.
			static POINT cursor_coords{};
			if (g_gui.m_opened)
			{
				GetCursorPos(&cursor_coords);
			}
			else if (cursor_coords.x + cursor_coords.y != 0)
			{
				SetCursorPos(cursor_coords.x, cursor_coords.y);
			}

			g_gui.m_opened ^= true;
		}
			

		if (g_gui.m_opened)
		{
			ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
		}
	}
}
