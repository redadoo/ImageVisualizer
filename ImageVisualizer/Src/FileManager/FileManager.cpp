#include "FileManager.h"


bool CompareFileName(const File &a, const File& b) { return a.name < b.name; }
bool CompareFileDataCreation(const File& a, const File& b)  { return a.creationTime < b.creationTime; }
bool CompareFileType(const File& a, const File& b) { return a.type < b.type; }


FileManager::FileManager()
{
	isFolderChange = false;
	exitFolderCheck = false;
	haveFolderHaveChanged = false;

	fileType = FileType::All;
	fileOrder = FileOrder::Alphabetical;
	fileLogo = new FileLogo();
}

FileManager::~FileManager()
{
	delete fileLogo;
}

bool FileManager::SortFiles(FileOrder _fileOrder, bool ascending, std::vector<File> &fileToShow)
{
	if (_fileOrder != fileOrder)
	{
		switch (_fileOrder)
		{
			case Alphabetical:
				std::sort(files.begin(), files.end(), CompareFileName);
				break;
			case DataCreation:
				std::sort(files.begin(), files.end(), CompareFileDataCreation);
				break;
			case Type:
				std::sort(files.begin(), files.end(), CompareFileType);
				break;
		}
		fileOrder = _fileOrder;
		fileToShow = files;
		return true;
	}
	return false;
}

void FileManager::SetFileType(FileType newFileType) { fileType = newFileType; }

void FileManager::GetFiles(std::vector<File> &fileToShow)
{
	if (fileType != FileType::All)
	{
		for (const File &var : files)
		{
			if (var.type == fileType)
			{
				fileToShow.push_back(var);
			}
		}
	}
	else
	{
		fileToShow.clear();
		fileToShow = files;
	}
}

FileLogo *FileManager::GetFileLogo(){ return fileLogo;}

FileType FileManager::GetFileType() { return fileType; }

void FileManager::AddFiles(std::filesystem::directory_entry entry)
{
	files.push_back({ GetFileName(entry.path().string()), entry.path() });
}
