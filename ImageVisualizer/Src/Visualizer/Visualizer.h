#pragma once

# include "../FileManager/FileManager.h"
# include "../ImGuiHelper/ImGuiHelper.h"
# include "../ImGui/Imgui_themes.h"
# include "../ImGui/imgui_internal.h"

#ifndef VISUALIZER_H
# define VISUALIZER_H


/* struct declared in FileHelper.h*/
struct File;
struct ImageFile; 
struct TextFile; 
struct FileLogo;

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
		/// Gets the status indicating whether the main page need to be closed.
		/// </summary>
		bool isMainPageClosed() { return closeMainPage; }

	private:
		FileManager								fileManager;
		ImVec2									searchBarPos;
		float									searchBarSize;
		
		size_t									indexFileToDisplay;

		bool									mediaWindow;
		bool									pathUpdated;
		bool									showThemeMenu;
		bool									closeMainPage;
		bool									realTimeChange;
		bool									dirPathIsCorrect;

		bool									nameOrder;
		bool									dataCreationOrder;
		bool									typeOrder;


		bool									ascending;
		bool									descending;

		bool									allFileFilter;
		bool									imageFilter;
		bool									textFilter;

		std::string								tmpPath;
		std::string								currentFolder;

		std::vector<File>						fileToShow;

		/// <summary>
		/// set position on main window for search bar
		/// </summary>
		void	setPosWidget();
		
		/// <summary>
		/// show on the main page file inside folder in the form of button (at the moment only image and text file is supported)
		/// </summary>
		void	ShowFile();

		void	ShowImage();

		void	ShowText();

		/// <summary>
		/// check if path is correct and if so search the folder and init file
		/// </summary>
		void	SearchFolder();
	
		/// <summary>
		/// Monitors changes in the specified folder and updates the Visualizer accordingly.
		/// </summary>
		void	CheckFolder();

		/// <summary>
		/// Manages the real-time thread for checking and updating the folder contents.
		/// </summary>
		void	ManageRealTimeThread();

		/// <summary>
		/// Scans the specified path for image files and adds them to the list of image files.
		/// </summary>
		/// <param name="isFolderModified">A boolean indicating if the function is called when something inside the folder is changed.</param>
		void	ScanAndAddFiles(bool isFolderModified);

		/// <summary>
		/// This method updates the current folder path for the Visualizer.
		/// <para> It also marks that the path has been updated and notifies all waiting threads</para>
		/// </summary>
		void	SetPath(const std::string& newPath);

		void	MenuBar();

};

#endif