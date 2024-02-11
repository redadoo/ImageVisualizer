#pragma once


#include "../FileHelper/FileHelper.h"
#include "../ImGuiHelper/ImGuiHelper.h"
#include "../ImGui/Imgui_themes.h"
#include "../ImGui/imgui_internal.h"
#include <vector>
#include<iostream>
#include <mutex>
#include <atomic>
#include <thread>
#include <algorithm>

# define PATH_LOGO_TXT "/Users/edoar/Desktop/GitHub/ImageVisualizer/ImageVisualizer/Logo/txt_logo.png"

/* struct declared in FileHelper.h*/
struct File;
struct ImageFile; 
struct TextFile; 

bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

/// <summary>
///  Represents an different type of file logo.
/// </summary>
struct FileLogo
{
	int							my_logo_width;
	int							my_logo_height;
	ID3D11ShaderResourceView*	logo = NULL;

	FileLogo()
	{
		LoadTextureFromFile(PATH_LOGO_TXT, &logo, &my_logo_width, &my_logo_height);
	}

	void ShowLogo(ImVec2 pos)
	{
		ImGui::SetCursorPos(pos);
		ImGui::Image(logo, ImVec2(60, 60));
	}

	void ShowLogo()
	{
		ImGui::ImageButton(logo, ImVec2(60, 60));
	}
};


#ifndef VISUALIZER_H
#define VISUALIZER_H


/// <summary>
/// The Visualizer class provides functionality for visualizing image files in a specified folder.
/// </summary>

class Visualizer
{

	public:

		Visualizer();

		~Visualizer();

		/// <summary>
		/// Displays the main page of the visualizer.
		/// </summary>
		void ShowMainPage();

		/// <summary>
		/// Gets the status indicating whether the main page is closed.
		/// </summary>
		bool isMainPageClosed() { return closeMainPage; }

	private:
		std::thread								threadfolderCheck;
		size_t									indexFileToDisplay;
		bool									showThemeMenu;
		bool									closeMainPage;
		bool									pathUpdated;
		bool									realTimeChange;
		bool									mediaWindow;

		bool									dirPathIsCorrect;

		std::atomic<bool>						haveFolderHaveChanged;
		std::atomic<bool>						exitFolderCheck;
		std::atomic<bool>						isFolderChange;

		std::mutex								pathFolderMutex;
		std::condition_variable					cv;
		
		std::string								tmpPath;
		std::string								currentFolder;

		int										indexOrder;
		int										indexFilter;
		const char								*filter[3] = { "all file", "image", "text" };
		const char								*order[3] = { "name", "type", "creation date"};

		FileLogo								fileLogo;
		std::vector<File>						files;

		/// <summary>
		/// show on the main page file inside folder in the form of button (at the moment only image file is supported)
		/// </summary>
		void ShowFile();

		/// <summary>
		/// show on the main page this flags/buttons:
		/// <para> - Real time visualization : enable this flag to see live changes to the folder </para>
		/// <para> - x button : close program </para>
		/// </summary>
		void DisplayButtonAndFlag();


		void ShowImage();

		void ShowText();


		/// <summary>
		/// check if path is correct and if so search the folder and init file
		/// </summary>
		void SearchFolder();
	
		/// <summary>
		/// Monitors changes in the specified folder and updates the Visualizer accordingly.
		/// </summary>
		void CheckFolder();

		/// <summary>
		/// Manages the real-time thread for checking and updating the folder contents.
		/// </summary>
		void ManageRealTimeThread();

		/// <summary>
		/// Scans the specified path for image files and adds them to the list of image files.
		/// </summary>
		/// <param name="isFolderModified">A boolean indicating if the function is called when something inside the folder is changed.</param>
		void ScanAndAddFiles(bool isFolderModified);

		/// <summary>
		/// This method updates the current folder path for the Visualizer object.
		/// <para> It also marks that the path has been updated and notifies all waiting threads</para>
		/// </summary>
		void SetPath(const std::string& newPath);

		void MenuBar();
};

#endif