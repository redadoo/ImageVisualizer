#pragma once

struct ImageFile;

#include "../FileHelper/FileHelper.h"
#include "../ImGuiHelper/ImGuiHelper.h"
#include <vector>
#include<iostream>
#include <mutex>
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
			void StartUpPage();
			
			std::string ReadFilePath() 
			{
				const std::lock_guard<std::mutex> lock(mutexFilePath);
				return FolderToVisualize;
			}

			void SetIsFolderChanged(bool v)
			{
				const std::lock_guard<std::mutex> lock(mutexIsFolderChangedSet);
				isFolderChanged = v;
			}

			bool GetIsFolderChanged()
			{
				const std::lock_guard<std::mutex> lock(mutexIsFolderChangedGet);
				return isFolderChanged;
			}

			std::thread								threadForCheckDir;
			void CheckFolder();

		private:
			bool									realTimeChange;
			bool									debugModeIsOn;
			bool									isFolderChanged;


			std::string								tmpPath;
			std::vector<ImageFile>					Imagefiles;
			bool									StartUpPageDraw;
			std::string								FolderToVisualize;
			bool									dirPathIsCorrect = true;
			bool									openWindows;
			std::mutex								mutexFilePath;
			std::mutex								mutexIsFolderChangedSet;
			std::mutex								mutexIsFolderChangedGet;
			int										indexImageToDisplay;

			void ShowFile();
			void ShowError();
			void ShowImage();
			void SearchFlag();
			void SearchFolder();
			bool IsFolderChanged();
			void AddFilesVector(std::filesystem::path path);

	};
}



#endif