#include "ImGuiHelper.h"

void ImGuiHelper::ColorBox(const char* label, ImVec2 Size, ImVec2 Pos)
{
	ImGui::SetCursorPos(Pos);
	ImGui::Selectable(label, true, ImGuiSelectableFlags_Disabled, Size);
}

void ImGuiHelper::TextWithPos(const char* Text, ImVec2 Pos, bool IsTextDisabilited)
{

	ImGui::SetCursorPos(Pos);
	if (IsTextDisabilited)
		ImGui::TextDisabled(Text);
	else
		ImGui::Text(Text);
}

bool ImGuiHelper::InputTextWithPos(const char* label, char* buf, size_t size, ImVec2 Pos, ImGuiInputTextFlags_ flag)
{
	ImGui::SetCursorPos(Pos);
	return ImGui::InputText(label, buf, size, flag);
}

bool ImGuiHelper::InputTextWithPos(const char* label, std::string* str, ImVec2 Pos, ImGuiInputTextFlags_ flag)
{
	ImGui::SetCursorPos(Pos);
	return ImGui::InputText(label, str, flag);
}

bool ImGuiHelper::CheckBoxWithPos(const char* label, bool* v, ImVec2 Pos)
{
	ImGui::SetCursorPos(Pos);
	return ImGui::Checkbox(label, v);
}

bool ImGuiHelper::ButtonWithPos(const char* Label, ImVec2 Size, ImVec2 Pos)
{
	ImGui::SetCursorPos(Pos);
	return ImGui::Button(Label, Size);
}

bool ImGuiHelper::BeginChildWithPos(const char* Label, ImVec2 Size, bool Border, ImVec2 Pos)
{
	ImGui::SetCursorPos(Pos);
	return ImGui::BeginChild(Label, Size, Border);
}

void ImGuiHelper::TextWithBox(const char* Text, ImVec2 Pos, bool IsTextDisabilited)
{
	ImGui::Selectable("##test", true, ImGuiSelectableFlags_Disabled);
	ImGuiHelper::TextWithPos(Text, Pos, IsTextDisabilited);
}

void ImGuiHelper::SameLineMax(int index,ImGuiStyle& style,size_t maxItemCount, float window_visible_x2,float xSize)
{
	float last_button_x2 = ImGui::GetItemRectMax().x;
	float next_button_x2 = last_button_x2 + style.ItemSpacing.x + xSize;
	if (index + 1 < maxItemCount && next_button_x2 < window_visible_x2)
		ImGui::SameLine();
}

void ImGuiHelper::ImageCentered(ImTextureID user_texture_id, ImVec2 imageSize)
{
	float alignment = 0.5f;

	ImGuiStyle& style = ImGui::GetStyle();

	float sizeX = imageSize.x + style.FramePadding.x * 2.0f;
	float sizeY = imageSize.y + style.FramePadding.y * 2.0f;
	float availX = ImGui::GetContentRegionAvail().x;
	float availY = ImGui::GetContentRegionAvail().y;

	float offX = (availX - sizeX) * alignment;
	float offY = (availY - sizeY) * alignment;
	if (offX > 0.0f && offY > 0.0f)
	{
		//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
		ImGui::SetCursorPos({ ImGui::GetCursorPosX() + offX ,ImGui::GetCursorPosY() + offY });
	}

	ImGui::Image(user_texture_id, imageSize);
}