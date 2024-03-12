#include "FileManager.h"


bool CompareFileName(const GenericFile &a, const GenericFile& b, bool v) { return v && (a.name < b.name); }
bool CompareFileDataCreation(const GenericFile& a, const GenericFile& b, bool v)  { (void)v; (void)b; (void)a; return true; }
bool CompareFileType(const GenericFile& a, const GenericFile& b, bool v) { return v && (a.type < b.type); }

struct CompareFileNameWithParam {
    bool ascending;
    CompareFileNameWithParam(bool val) : ascending(val) {}
    bool operator()(const GenericFile &a, const GenericFile &b) const {
        return CompareFileName(a, b, ascending);
    }
};

struct CompareFileDataCreationWithParam {
    bool ascending;
    CompareFileDataCreationWithParam(bool val) : ascending(val) {}
    bool operator()(const GenericFile &a, const GenericFile &b) const {
        return CompareFileDataCreation(a, b, ascending);
    }
};

struct CompareFileTypeWithParam {
    bool ascending;
    CompareFileTypeWithParam(bool val) : ascending(val) {}
    bool operator()(const GenericFile &a, const GenericFile &b) const {
        return CompareFileType(a, b, ascending);
    }
};


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

bool FileManager::SortFiles(FileOrder _fileOrder, bool ascending, std::vector<GenericFile> &fileToShow)
{
	if (_fileOrder != fileOrder)
	{
		switch (_fileOrder)
		{
			case Alphabetical:
				std::sort(files.begin(), files.end(), CompareFileNameWithParam(ascending));
				break;
			case DataCreation:
				std::sort(files.begin(), files.end(), CompareFileDataCreationWithParam(ascending));
				break;
			case Type:
				std::sort(files.begin(), files.end(), CompareFileTypeWithParam(ascending));
				break;
			case NoneFileOrder:
				break;
		}
		fileOrder = _fileOrder;
		fileToShow = files;
		return true;
	}
	return false;
}

void FileManager::SetFileType(FileType newFileType) { fileType = newFileType; }

void FileManager::GetFiles(std::vector<GenericFile> &fileToShow)
{
	if (fileType != FileType::All)
	{
		for (const GenericFile &var : files)
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

void FileManager::AddFiles(std::filesystem::directory_entry entry)
{
	files.push_back({ GetFileName(entry.path().string()), entry.path() });
}

FileLogo *FileManager::GetFileLogo(){ return fileLogo;}

FileType FileManager::GetFileType() { return fileType; }

