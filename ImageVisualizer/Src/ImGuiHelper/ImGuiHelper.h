#pragma once

#include <string>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_stdlib.h"


namespace ImGuiHelper
{
	void	ColorBox(const char* label, ImVec2 Size, ImVec2 Pos);
	void	TextWithPos(const char* Text, ImVec2 Pos, bool IsTextDisabilited);
	bool	CheckBoxWithPos(const char* label, bool* v, ImVec2 Pos);
	bool	ButtonWithPos(const char* Label, ImVec2 Size, ImVec2 Pos);
	bool	InputTextWithPos(const char* label, char* buf, size_t size, ImVec2 Pos, ImGuiInputTextFlags_ flag = ImGuiInputTextFlags_None);
	bool	InputTextWithPos(const char* label, std::string* str, ImVec2 Pos, ImGuiInputTextFlags_ flag);
	bool	BeginChildWithPos(const char* Label, ImVec2 Size, bool Border, ImVec2 Pos);
	void	TextWithBox(const char* Text, ImVec2 Pos, bool IsTextDisabilited);
	void	SameLineMax(int index, ImGuiStyle& style, size_t maxItemCount, float window_visible_x2, float xSize);

}