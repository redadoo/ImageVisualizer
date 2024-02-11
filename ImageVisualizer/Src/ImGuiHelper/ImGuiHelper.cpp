#include "ImGuiHelper.h"

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

bool ImGuiHelper::ButtonWithPos(const char* Label, ImVec2 Size, ImVec2 Pos)
{
	ImGui::SetCursorPos(Pos);
	return ImGui::Button(Label, Size);
}

void ImGuiHelper::SameLineMax(size_t index,ImGuiStyle& style,size_t maxItemCount, float window_visible_x2,float xSize)
{
	float last_button_x2 = ImGui::GetItemRectMax().x;
	float next_button_x2 = last_button_x2 + style.ItemSpacing.x + xSize;
	if (index + 1 < maxItemCount && next_button_x2 < window_visible_x2)
	{
		ImGui::SameLine();
	}
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

void ImGuiHelper::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

bool ImGuiHelper::ComboWithPos(const char* const items[], const char* name, ImVec2 pos, float width, int* index, int size)
{
	ImGui::SetCursorPos(pos);
	ImGui::PushItemWidth(width);
	return ImGui::Combo(name, index, items, size);
}
