#include "imgui.h"
#include "imgui.hh"
#include "main.hh"
#include "network.hh"

namespace UiLevelBrowser {
    static bool do_open = false;
    static level_list_state state;

    static ImVec2 dim;

    void open() {
        do_open = true;

        dim = UI(185.f, 162.f);

        state.status = request_status::Loading;
        state.levels.clear();

        P.add_action(ACTION_GET_LEVEL_LIST, &state);
    }

    void layout_loading() {
        ImGui::TextUnformatted("Loading levels...");
    }

    void layout_failed() {
        ImGui::TextUnformatted("Failed to load levels.");
    }

    static void level_card(const level_info &lvl) {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

        ImGui::BeginChild(
            (std::string("##level") + std::to_string(lvl.id)).c_str(),
            dim,
            ImGuiChildFlags_Borders);

        ImDrawList *dl = ImGui::GetWindowDrawList();

        ImVec2 p = ImGui::GetCursorScreenPos();

        // Thumbnail placeholder

        dl->AddRectFilled(
            p,
            ImVec2(p.x + dim.x, p.y + UI(135.f / 1.3f)),
            IM_COL32(75, 75, 80, 255),
            8.f,
            ImDrawFlags_RoundCornersTop);

        dl->AddText(
            ImVec2(p.x + UI(78.f / 1.3f), p.y + UI(60.f / 1.3f)),
            IM_COL32(220,220,220,255),
            "Thumbnail");

        ImGui::Dummy(ImVec2(dim.x, UI(100.f)));

        // Title

        ImGui::PopStyleVar(1);

        ImGui::SetCursorPosX(UI(5.f));

        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + UI(175.f));
        ImGui::TextUnformatted(lvl.title.c_str());
        ImGui::PopTextWrapPos();

        // Author

        float author_y = dim.y
            - (ImGui::GetStyle().WindowPadding.y / 2)
            - ImGui::GetTextLineHeight();

        ImGui::SetCursorPos(ImVec2(UI(5.f), author_y));
        ImGui::TextDisabled("%s", lvl.author.name.c_str());

        ImGui::EndChild();

        ImGui::PopStyleVar(2);
    }

    void layout_levels() {
        ImGui::BeginChild("LevelGrid");

        const float spacing = ImGui::GetStyle().ItemSpacing.x;

        float avail = ImGui::GetContentRegionAvail().x;

        int columns = std::max(1, (int)((avail + spacing) / (dim.x + spacing)));

        for (size_t i = 0; i < state.levels.size(); ++i) {
            if (i == 40)
                break;

            level_card(state.levels[i]);

            if ((i + 1) % columns != 0)
                ImGui::SameLine();
        }

        ImGui::EndChild();
    }

    void layout() {
        handle_do_open(&do_open, "Level Browser");

        ImGui_CenterNextWindow();

        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowSize(
            ImVec2(
                std::min(viewport->WorkSize.x * 0.99f, UI(800)),
                viewport->WorkSize.y * 0.99f),
            ImGuiCond_Always);
        if (ImGui::BeginPopupModal("Level Browser", REF_TRUE, MODAL_FLAGS)) {
            if (state.status == request_status::Loading)
                layout_loading();
            else if (state.status == request_status::Failed)
                layout_failed();
            else
                layout_levels();

            ImGui::EndPopup();
        }
    }
}
