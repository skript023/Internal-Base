#include "imgui.h"
#include "script.hpp"
#include "player_menu.h"
#include "settings.hpp"

namespace big
{
	void player_menu::render_menu()
	{
        if (ImGui::BeginTabItem("Player Option"))
        {
            

            ImGui::EndTabItem();
        }
	}
}