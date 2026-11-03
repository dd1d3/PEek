#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <Windows.h>
#include <commdlg.h>
#include <string>
#include "f1mainmodule.h"
#include "dotnetdetect.h"
#include "uicore.h"
#include "logo_globals.h"

PEResult g_result;
char g_filePath[MAX_PATH] = "";
static int g_selectedTab = 0;

static void OpenFileDialog()
{
    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = g_filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Executable\0*.exe\0All Files\0*.*\0";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileNameA(&ofn))
        AnalyzePE(g_filePath, g_result);
}

static void RenderTitleBar(float totalW, float titleH)
{
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.04f, 0.04f, 0.04f, 0.0f));
    ImGui::BeginChild("##titlebar", ImVec2(totalW, titleH), false,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::SetCursorPos(ImVec2(14, 10));
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "PEek");

    ImGui::SameLine(totalW - 290);
    ImGui::SetNextItemWidth(210);
    if (ImGui::InputText("##path", g_filePath, MAX_PATH, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        DWORD attrib = GetFileAttributesA(g_filePath);
        if (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY))
            AnalyzePE(g_filePath, g_result);
        else
            g_result.error = "File not found";
    }

    ImGui::SameLine();
    if (ImGui::Button("Open", ImVec2(60, 0)))
        OpenFileDialog();

    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 wp = ImGui::GetWindowPos();
    draw->AddLine(
        ImVec2(wp.x, wp.y + titleH),
        ImVec2(wp.x + totalW, wp.y + titleH),
        IM_COL32(25, 25, 25, 255), 1.0f);
}

static void RenderSidebar(float totalW, float totalH, float titleH, float sideW)
{
    ImGui::SetCursorPos(ImVec2(0, titleH + 1));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.05f, 0.05f, 0.05f, 0.0f));
    ImGui::BeginChild("##sidebar", ImVec2(sideW, totalH - titleH - 1), false,
        ImGuiWindowFlags_NoScrollbar);

    ImGui::Spacing();
    ImGui::Spacing();

    const char* tabs[] = { "Info", "Hashes", "Sections", "Imports", "Strings"};

    for (int i = 0; i < 5; i++)
    {
        bool selected = (g_selectedTab == i);

        ImGui::PushStyleColor(ImGuiCol_Button,
            selected ? ImVec4(0.10f, 0.10f, 0.10f, 1.0f)
            : ImVec4(0.00f, 0.00f, 0.00f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.10f, 0.10f, 0.10f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text,
            selected ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
            : ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

        ImGui::SetCursorPosX(0);
        if (ImGui::Button(tabs[i], ImVec2(sideW, 30)))
            g_selectedTab = i;

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();

    }

    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 sp = ImGui::GetWindowPos();
    ImVec2 ss = ImGui::GetWindowSize();
    draw->AddLine(
        ImVec2(sp.x + ss.x, sp.y),
        ImVec2(sp.x + ss.x, sp.y + ss.y),
        IM_COL32(20, 20, 20, 255), 1.0f);

    ImGui::EndChild();
    ImGui::PopStyleColor();
}

static void RenderContent(float totalW, float totalH, float titleH, float sideW)
{
    bool loaded = g_result.validPE;

    ImGui::SetCursorPos(ImVec2(sideW + 1, titleH + 1));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.05f, 0.05f, 0.05f, 0.0f));
    ImGui::BeginChild("##content", ImVec2(totalW - sideW - 1, totalH - titleH - 1), false);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14, 10));
    ImGui::SetCursorPos(ImVec2(14, 10));

    if (!g_result.error.empty())
        ImGui::TextColored(ImVec4(0.8f, 0.3f, 0.3f, 1.0f), "Error: %s", g_result.error.c_str());

    // Info
    if (g_selectedTab == 0)
    {
        auto row = [](const char* label, const char* value) {
            ImGui::SetCursorPosX(14);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", label);
            ImGui::SameLine(160);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", value);
            };

        row("File Type", loaded ? (g_result.is64bit ? "PE64" : "PE32") : "-");
        row("Architecture", loaded ? (g_result.is64bit ? "x64" : "x86") : "-");
        row("Machine", loaded ? g_result.machine.c_str() : "-");
        row("Sections", loaded ? std::to_string(g_result.sections.size()).c_str() : "-");
        row("Imports", loaded ? std::to_string(g_result.imports.size()).c_str() : "-");
        row("TimeDateStamp", loaded ? g_result.timeDateStamp.c_str() : "-");

        // Compiler
        ImGui::Spacing();
        ImGui::SetCursorPosX(14);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Compiler");

        if (loaded && g_result.dotNetInfo)
        {
            ImGui::SameLine(160);
            ImGui::TextColored(ImVec4(0.3f, 0.6f, 1.0f, 1.0f), ".NET (Managed)");
		}
		else if (loaded && !g_result.compilerHits.empty())
        {
            float maxScore = 0;
            for (auto& h : g_result.compilerHits)
                if (h.score > maxScore) maxScore = h.score;

            for (auto& h : g_result.compilerHits)
            {
                float pct = (maxScore > 0) ? (h.score / maxScore) * 100.0f : 0.0f;

                ImVec4 color;
                if (pct >= 80)      color = ImVec4(0.4f, 0.8f, 0.4f, 1.0f);
                else if (pct >= 40) color = ImVec4(1.0f, 0.7f, 0.2f, 1.0f);
                else                color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

                ImGui::SetCursorPosX(14);
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "  %-10s", h.name.c_str());
                ImGui::SameLine(160);
                ImGui::TextColored(color, "%.0f%%", pct);
            }
        }
        else if (loaded)
        {
            ImGui::SameLine(160);
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Undefined");
        }
        else
        {
            ImGui::SameLine(160);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "-");
        }

        // Packer
        ImGui::Spacing();
        ImGui::SetCursorPosX(14);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Packer");

        if (loaded && !g_result.packerHits.empty())
        {
            float maxScore = 0;
            for (auto& h : g_result.packerHits)
                if (h.score > maxScore) maxScore = h.score;

            for (auto& h : g_result.packerHits)
            {
                float pct = (maxScore > 0) ? (h.score / maxScore) * 100.0f : 0.0f;

                ImVec4 color;
                if (pct >= 80)      color = ImVec4(0.4f, 0.8f, 0.4f, 1.0f);
                else if (pct >= 40) color = ImVec4(1.0f, 0.7f, 0.2f, 1.0f);
                else                color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

                ImGui::SetCursorPosX(14);
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "  %-10s", h.name.c_str());
                ImGui::SameLine(160);
                ImGui::TextColored(color, "%.0f%%", pct);
            }
        }
        else if (loaded)
        {
            ImGui::SameLine(160);
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Unknown");
        }
        else
        {
            ImGui::SameLine(160);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "-");
        }

        // Entropy
        if (loaded && !g_result.sectionEntropy.empty())
        {
            ImGui::Spacing();
            static bool entropyOpen = true;
            ImGui::SetCursorPosX(14);
            ImGui::BeginChild("##entropyblock", ImVec2(300, 0), false);
            if (ImGui::CollapsingHeader("Entropy##info", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Spacing();
                for (int i = 0; i < (int)g_result.sections.size(); i++)
                {
                    double ent = g_result.sectionEntropy[i];
                    ImVec4 color;
                    if (ent >= 7.0)      color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
                    else if (ent >= 6.0) color = ImVec4(1.0f, 0.7f, 0.2f, 1.0f);
                    else                 color = ImVec4(0.4f, 0.8f, 0.4f, 1.0f);

                    ImGui::SetCursorPosX(14);
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%-12s", g_result.sections[i].c_str());
                    ImGui::SameLine(160);
                    ImGui::TextColored(color, "%.4f", ent);
                }
                ImGui::Spacing();
            }
            ImGui::EndChild();
        }
    }

    // Hashes
    if (g_selectedTab == 1)
    {
        static char empty[1] = "";

        ImGui::SetCursorPosX(14);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "MD5");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##md5",
            loaded ? (char*)g_result.md5.c_str() : empty,
            loaded ? g_result.md5.size() + 1 : 1,
            ImGuiInputTextFlags_ReadOnly);

        ImGui::Spacing();

        ImGui::SetCursorPosX(14);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "SHA256");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##sha256",
            loaded ? (char*)g_result.sha256.c_str() : empty,
            loaded ? g_result.sha256.size() + 1 : 1,
            ImGuiInputTextFlags_ReadOnly);


		ImGui::Spacing();

		ImGui::SetCursorPosX(14);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "SHA1");
		ImGui::SetNextItemWidth(-1);
		ImGui::InputText("##sha1",
			loaded ? (char*)g_result.sha1.c_str() : empty,
			loaded ? g_result.sha1.size() + 1 : 1,
			ImGuiInputTextFlags_ReadOnly);
    }

    // Sections
    if (g_selectedTab == 2)
    {
        if (ImGui::BeginTable("##sections", 2,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
            ImVec2(-1, -1)))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 40);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            if (loaded)
                for (int i = 0; i < (int)g_result.sections.size(); i++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%d", i);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", g_result.sections[i].c_str());
                }

            ImGui::EndTable();
        }
    }

    // Imports
    if (g_selectedTab == 3)
    {
        if (ImGui::BeginTable("##imports", 2,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
            ImVec2(-1, -1)))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed, 40);
            ImGui::TableSetupColumn("DLL", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            if (loaded)
                for (int i = 0; i < (int)g_result.imports.size(); i++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%d", i);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", g_result.imports[i].c_str());
                }

            ImGui::EndTable();
        }
    }


    // Strings
    if (g_selectedTab == 4)
    {
        ImGui::SetCursorPosX(14);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
            "Found: %d strings", (int)g_result.foundStrings.size());
        ImGui::Spacing();

        if (ImGui::BeginTable("##strings", 2,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX,
            ImVec2(-1, -1)))
        {
            ImGui::TableSetupScrollFreeze(1, 1);
            ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed,50);
            ImGui::TableSetupColumn("String", ImGuiTableColumnFlags_WidthFixed, 5000);
            ImGui::TableHeadersRow();

            if (loaded)
                for (int i = 0; i < (int)g_result.foundStrings.size(); i++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%d", i);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushID(i);
                    if (ImGui::Selectable(g_result.foundStrings[i].c_str()))
                    {
                        ImGui::SetClipboardText(g_result.foundStrings[i].c_str());
                        ImGui::OpenPopup("copied_popup");
                    }
                    if (ImGui::BeginPopup("copied_popup"))
                    {
                        ImGui::Text("Copied to clipboard");
                        ImGui::EndPopup();
                    }
                    ImGui::PopID();
                }

            ImGui::EndTable();
        }
    }

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void RenderUI()
{
    ImGuiIO& io = ImGui::GetIO();
    float totalW = io.DisplaySize.x;
    float totalH = io.DisplaySize.y;
    float titleH = 34.0f;
    float sideW = 120.0f;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(totalW, totalH));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("##root", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PopStyleVar();

    if (g_logoSRV)
    {
        ImGui::GetWindowDrawList()->AddImage(
            (ImTextureID)g_logoSRV,
            ImVec2(0, 0),
            ImVec2(totalW, totalH)
        );
    }

    RenderTitleBar(totalW, titleH);
    RenderSidebar(totalW, totalH, titleH, sideW);
    RenderContent(totalW, totalH, titleH, sideW);

    ImGui::End();
}