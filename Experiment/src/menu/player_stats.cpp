#include "player_stats.h"
#include "imgui.h"
#include "script.hpp"
#include "settings.hpp"

namespace big
{
	void player_stat::render_menu()
	{
        if (ImGui::BeginTabItem("Player Stats"))
        {
            
            ImGui::EndTabItem();
        }
	}
}