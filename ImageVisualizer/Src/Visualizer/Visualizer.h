#pragma once


#include "../FileHelper/FileHelper.h"
#include "../ImGuiHelper/ImGuiHelper.h"
#include <vector>
#include<iostream>
#include <mutex>
#include <atomic>
#include <thread>

#ifndef VISUALIZER_H
#define VISUALIZER_H


struct ImageFile; // struct to rapresent image file (declared in FileHelper.h)

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

		int										indexImageToDisplay;

		bool									closeMainPage;
		bool									pathUpdated;
		bool									realTimeChange;
		bool									imageWindow;

		bool									dirPathIsCorrect;

		std::atomic<bool>						haveFolderHaveChanged;
		std::atomic<bool>						exitFolderCheck;
		std::atomic<bool>						isFolderChange;

		std::mutex								pathFolderMutex;
		std::condition_variable					cv;
		
		std::string								tmpPath;
		std::string								currentFolder;

		std::vector<ImageFile>					imageFiles;


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

		/// <summary>
		/// open a imgui window for show images in real size
		/// </summary>
		void ShowImage();

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
		void ScanAndAddImageFiles(bool isFolderModified);

		/// <summary>
		/// This method updates the current folder path for the Visualizer object.
		/// <para> It also marks that the path has been updated and notifies all waiting threads</para>
		/// </summary>
		void SetPath(const std::string& newPath);
};

#endif