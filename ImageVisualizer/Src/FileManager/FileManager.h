#ifndef FILEMANAGER_H
# define FILEMANAGER_H

# include "../FileHelper/FileHelper.h"
# include <vector>
# include <iostream>
# include <algorithm>
# include <filesystem>
# include <thread>
# include <mutex>
# include <atomic>
# include <condition_variable>


enum FileOrder
{
	NoneFileOrder,
	Alphabetical,
	DataCreation,
	Type
};

namespace FileHelper {}
using namespace FileHelper;

class FileManager
{
	public:
		std::condition_variable	cv;
		std::mutex				pathFolderMutex;

		FileManager();
		~FileManager();
		bool					SortFiles(FileOrder _fileOrder, bool ascending, std::vector<GenericFile>& fileToShow);
		void					SetFileType(FileType newFileType);
		void					GetFiles(std::vector<GenericFile> &fileToShow);
		FileLogo				*GetFileLogo();
		FileType				GetFileType();
		void					AddFiles(std::filesystem::directory_entry entry);
		void					SetPath(const std::string& newPath);
	private:

		std::vector<GenericFile>	files;
		FileType					fileType;
		FileLogo*					fileLogo;
		FileOrder					fileOrder;

		std::atomic<bool>			isFolderChange;
		std::atomic<bool>			exitFolderCheck;
		std::atomic<bool>			haveFolderHaveChanged;

		std::thread					threadfolderCheck;


};

#endif