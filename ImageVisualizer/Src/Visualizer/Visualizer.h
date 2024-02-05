#pragma once

struct ImageFile;

#include "../FileHelper/FileHelper.h"
#include "../ImGuiHelper/ImGuiHelper.h"
#include <vector>
#include<iostream>
#include <mutex>
#include <atomic>
#include <thread>

#ifndef VISUALIZER_H
#define VISUALIZER_H

/// <summary>
/// 
/// </summary>

namespace App
{

	class Visualizer
	{

		public:
			std::thread								threadfolderCheck;

			void StartUpPage();

		private:
			std::atomic<bool>						haveFolderHaveChanged{ false };
			std::atomic<bool>						exitFolderCheck{ false };
			std::mutex								pathFolderMutex;
			std::condition_variable					cv;
			bool									pathUpdated = false;


			int										indexImageToDisplay;

			bool									realTimeChange;
			bool									debugModeIsOn;
			bool									isFolderChanged;
			bool									StartUpPageDraw;
			bool									dirPathIsCorrect = true;
			bool									openWindows;
			std::atomic<bool>						isFolderChange = false;

			std::string								tmpPath;
			std::string								currentFolder;

			std::vector<ImageFile>					imageFiles;


			void ShowFile();
			void ShowError();
			void ShowImage();
			void SearchFlag();
			void ClearImage();
			void CheckFolder();
			void SearchFolder();
			bool IsFolderChanged();
			void ScanAndAddImageFiles();
			void AddFilesVector(std::filesystem::path path);
			void SetPath(const std::string& newPath);
	};
}



#endif