
# include "../FileHelper/FileHelper.h"
# include <vector>
# include <iostream>
# include <algorithm>
# include <filesystem>
# include <thread>
# include <mutex>
# include <atomic>

#ifndef FILEMANAGER_H
# define FILEMANAGER_H

struct File;
struct ImageFile;
struct TextFile;
struct FileLogo;
enum   FileType;
enum   FileOrder;

class FileManager
{
	public:
		std::condition_variable	cv;
		std::mutex				pathFolderMutex;

		FileManager();
		~FileManager();
		bool				SortFiles(FileOrder _fileOrder, bool ascending, std::vector<File>& fileToShow);
		void				SetFileType(FileType newFileType);
		void				GetFiles(std::vector<File> &fileToShow);
		FileLogo			*GetFileLogo();
		FileType			GetFileType();
		void				AddFiles(std::filesystem::directory_entry entry);
		void				SetPath(const std::string& newPath);
	private:

		std::vector<File>		files;
		FileType				fileType;
		FileLogo*				fileLogo;
		FileOrder				fileOrder;

		std::atomic<bool>		isFolderChange;
		std::atomic<bool>		exitFolderCheck;
		std::atomic<bool>		haveFolderHaveChanged;

		std::thread				threadfolderCheck;


};

#endif