#pragma once

#include <string>
#include <iostream>


#include "../ImGui/imgui.h"
#include "../ImGui/imgui_stdlib.h"

/// <summary>
/// Namespace containing helper functions for ImGui integration.
/// </summary>
namespace ImGuiHelper
{
	/// <summary>
	/// Displays text at the given position, using ImGui::Text().
	/// </summary>
	/// <param name="Text :">The text to display.</param>
	/// <param name="Pos :">The position of the text.</param>
	/// <param name="IsTextDisabilited : ">Flag indicating whether the text is disabled.</param>
	void	TextWithPos(const char* Text, ImVec2 Pos, bool IsTextDisabilited);

	/// <summary>
	/// Displays a button with the specified label and size at the specified position , using ImGui::Button().
	/// </summary>
	/// <param name="Label :">The label of the button.</param>
	/// <param name="Size :">The size of the button.</param>
	/// <param name="Pos :">The position of the button.</param>
	/// <returns>True if the button is pressed, otherwise false.</returns>
	bool	ButtonWithPos(const char* Label, ImVec2 Size, ImVec2 Pos);
	
	/// <summary>
	/// Displays an input text field at the specified position and allows the user to input text , using ImGui::InputText().
	/// </summary>
	/// <param name="label :">The label of the input field.</param>
	/// <param name="buf :">Pointer to the string to store the input text.</param>
	/// <param name="Pos :">The position of the input field.</param>
	/// <param name="flag :">Optional flags for the input field.</param>
	/// <returns>True if the input field content has changed, otherwise false(can change as the ImGuiInputTextFlags_ varies).</returns>
	bool	InputTextWithPos(const char* label, char* buf, size_t size, ImVec2 Pos, ImGuiInputTextFlags_ flag = ImGuiInputTextFlags_None);
	
	/// <summary>
	/// Displays an input text field at the specified position and allows the user to input text, using ImGui::InputText().
	/// </summary>
	/// <param name="label :">The label of the input field.</param>
	/// <param name="str :">Pointer to the string to store the input text.</param>
	/// <param name="Pos :">The position of the input field.</param>
	/// <param name="flag :">Optional flags for the input field.</param>
	/// <returns>True if the input field content has changed, otherwise false(can change as the ImGuiInputTextFlags_ varies).</returns>
	bool	InputTextWithPos(const char* label, std::string* str, ImVec2 Pos, ImGuiInputTextFlags_ flag);

	/// <summary>
	/// Displays an image centered within the available space , using ImGui::Image().
	/// </summary>
	/// <param name="user_texture_id :">The texture ID of the image to display.</param>
	/// <param name="imageSize :">The size of the image.</param>
	void	ImageCentered(ImTextureID user_texture_id, ImVec2 imageSize);
	
	/// <summary>
	/// Moves the cursor to the specified position on the same line using ImGui::SameLine() if there is enough space based on the 
	/// <para> - index of the current item</para>
	/// <para> - ImGui style</para>
	/// <para> - maximum item count per line</para>
	/// <para> - visible window boundaries</para>
	/// </summary>
	/// <param name="index :">The index of the current item.</param>
	/// <param name="style :">The ImGui style.</param>
	/// <param name="maxItemCount :">The maximum number of items per line.</param>
	/// <param name="window_visible_x2 :">The right position of the visible window.</param>
	/// <param name="xSize :">The size of the item.</param>
	void	SameLineMax(size_t index, ImGuiStyle& style, size_t maxItemCount, float window_visible_x2, float xSize);

	/// <summary>
	/// Helper to display a little (?) mark which shows a tooltip when hovered
	/// </summary>
	/// <param name="desc"></param>
	void HelpMarker(const char* desc);

}