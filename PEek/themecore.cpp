#include "themecore.h"
#include "imgui.h"

void ApplyTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 0.0f;
    style.FrameRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 3.0f;
    style.PopupRounding = 3.0f;
    style.ChildRounding = 0.0f;

    style.FramePadding = ImVec2(8, 4);
    style.ItemSpacing = ImVec2(8, 5);
    style.WindowPadding = ImVec2(0, 0);
    style.ScrollbarSize = 10.0f;
    style.GrabMinSize = 6.0f;
    style.IndentSpacing = 0.0f;

    ImVec4* c = style.Colors;

    c[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    c[ImGuiCol_ChildBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    c[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);

    c[ImGuiCol_Border] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

    c[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    c[ImGuiCol_FrameBgHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    c[ImGuiCol_FrameBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);

    c[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    c[ImGuiCol_TitleBgActive] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);

    c[ImGuiCol_Button] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    c[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    c[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);

    c[ImGuiCol_Header] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    c[ImGuiCol_HeaderHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    c[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

    c[ImGuiCol_Tab] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    c[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    c[ImGuiCol_TabSelected] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

    c[ImGuiCol_TableHeaderBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    c[ImGuiCol_TableBorderLight] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    c[ImGuiCol_TableBorderStrong] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    c[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    c[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.02f);

    c[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
    c[ImGuiCol_ScrollbarGrab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    c[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    c[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);

    c[ImGuiCol_Separator] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    c[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    c[ImGuiCol_TextDisabled] = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);

    c[ImGuiCol_ResizeGrip] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    c[ImGuiCol_ResizeGripHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    c[ImGuiCol_ResizeGripActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
}