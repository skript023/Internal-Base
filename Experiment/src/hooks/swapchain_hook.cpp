#include "hooking.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script_mgr.hpp"

namespace big
{
	HRESULT APIENTRY hooks::swapchain_present(IDXGISwapChain* this_, UINT sync_interval, UINT flags)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				if ((g_renderer->m_init || g_renderer->init(this_)))
					g_renderer->on_present();
			}

			return g_hooking->m_swapchain_present_hook.get_original<decltype(&swapchain_present)>()(this_, sync_interval, flags);
		} EXCEPT_CLAUSE

		return NULL;
	}

	HRESULT APIENTRY hooks::swapchain_resizebuffers(IDXGISwapChain* this_, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_renderer->pre_reset();

				auto result = g_hooking->m_swapchain_resizebuffers_hook.get_original<decltype(&swapchain_resizebuffers)>()
					(this_, buffer_count, width, height, new_format, swapchain_flags);

				if (SUCCEEDED(result))
				{
					g_renderer->post_reset(this_);
				}

				return result;
			}

			return g_hooking->m_swapchain_resizebuffers_hook.get_original<decltype(&swapchain_resizebuffers)>()
				(this_, buffer_count, width, height, new_format, swapchain_flags);
		} EXCEPT_CLAUSE

		return NULL;
	}

	void APIENTRY hooks::swapchain_draw_indexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
	{
		//return g_hooking->m_swapchain_draw_indexed_hook.get_original<decltype(&swapchain_draw_indexed)>()(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
	}
}