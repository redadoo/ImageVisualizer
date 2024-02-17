#pragma once

# include"../Engine/Engine.h"
# include <vector>
# include <functional>
# include <iostream>
# include <filesystem>
# include <fstream>

# define PATH_LOGO_TXT "/Users/edoar/Desktop/GitHub/ImageVisualizer/ImageVisualizer/Logo/txt_logo.png"

/// <summary>
/// Enum representing different file types.
/// </summary>
enum FileType
{
	All,
	Image,
	Text,
	None
};

enum FileOrder
{
	NoneFileOrder,
	Alphabetical,
	DataCreation,
	Type
};

struct Date
{
	SYSTEMTIME date;

	bool operator>(const Date &other) const;
	bool operator<(const Date &other) const;
};

/// <summary>
/// Loads a texture from a file using the STB Image library and creates a Direct3D 11 shader resource view.
/// </summary>
/// <param name="filename:">The file path of the image to be loaded.</param>
/// <param name="out_srv:">A pointer to a pointer that will store the created ID3D11ShaderResourceView.</param>
/// <param name="out_width:">A pointer to an integer that will store the width of the loaded image.</param>
/// <param name="out_height:">A pointer to an integer that will store the height of the loaded image.</param>
/// <returns>
/// Returns true if the texture is successfully loaded and the shader resource view is created;
/// otherwise, returns false and outputs the failure reason to the standard output.
/// </returns>
bool		LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

/// <summary>
/// Represents an image file.
/// </summary>
struct ImageFile
{
	bool						errorOnLoadImage;
	int							my_image_width;
	int							my_image_height;
	ID3D11ShaderResourceView*	image = NULL;

	ImageFile(){}

	ImageFile(const char *path) {
		errorOnLoadImage = !LoadTextureFromFile(path, &image, &my_image_width, &my_image_height);
	}

	/// <summary>
	/// Releases any resources associated with the image.
	/// </summary>
	void ReleaseResources()
	{
		if (image != NULL)
		{
			image->Release();
			image = NULL;
		}
	}
};

/// <summary> Reads content from a file into a vector. </summary>
/// <param name="fileName"> Path to the file to read. </param>
/// <param name="content"> Vector to store file content. </param>
/// <returns> True if successful, false otherwise. </returns>
/// <remarks> Displays error message if file cannot be opened. </remarks>
bool		ReadContentFile(std::string fileName, std::vector<std::string>* content);  // TODO: find another method for read file

/// <summary>
///  Represents an text file.
/// </summary>
struct TextFile
{
	bool						errorOnReadFile;
	std::vector<std::string>	content;

	TextFile(){}

	TextFile(std::string filePath) {
		errorOnReadFile = !ReadContentFile(filePath, &content);
	}
};

/// <summary>
/// Determines the type of the specified file based on its name.
/// </summary>
/// <param name="fileName :">The name of the file.</param>
/// <returns>The FileType enum value indicating the type of the file.</returns>
FileType	GetFileType(std::string fileName);

/// <summary>
///  Represents an generic file.
/// </summary>
struct File
{
	std::string					name;
	std::filesystem::path		path;
	FileType					type;
	TextFile					textFile;
	ImageFile					imageFile;
	Date						creationTime;
	Date						lastAccessTime;
	ULONGLONG					fileSize;

	File(std::string _name, std::filesystem::path _path) : name(_name), path(_path), type(GetFileType(name)) 
	{

		if (type == FileType::Image)
			imageFile = ImageFile(path.string().c_str());
		else if (type == FileType::Text)
			textFile = TextFile(path.string());

		WIN32_FILE_ATTRIBUTE_DATA	fileInfo;
		if (GetFileAttributesEx(path.string().c_str(), GetFileExInfoStandard, &fileInfo) != 0) {

			FileTimeToSystemTime(&fileInfo.ftLastAccessTime, &lastAccessTime.date);
			FileTimeToSystemTime(&fileInfo.ftCreationTime, &creationTime.date);

			ULARGE_INTEGER ul;
			ul.HighPart = fileInfo.nFileSizeHigh;
			ul.LowPart = fileInfo.nFileSizeLow;
			fileSize = ul.QuadPart;
		}
		else {
			// Failed to get file attributes
			DWORD error = GetLastError();
			std::cerr << "Failed to get file attributes. Error code: " << error << std::endl;
		}

	}

};

/// <summary>
///  Represents different type of file logo.
/// </summary>
struct FileLogo
{
	int							my_logo_width;
	int							my_logo_height;
	bool						errorOnLoadingImage;
	ID3D11ShaderResourceView*	logoTxt = NULL;

	FileLogo()
	{
		errorOnLoadingImage = !LoadTextureFromFile(PATH_LOGO_TXT, &logoTxt, &my_logo_width, &my_logo_height);
	}

	void ShowLogo(ImVec2 pos)
	{
		ImGui::SetCursorPos(pos);
		ImGui::Image(logoTxt, ImVec2(60, 60));
	}

	void ShowLogo()
	{
		ImGui::ImageButton(logoTxt, ImVec2(60, 60));
	}

	~FileLogo()
	{
		if (!errorOnLoadingImage)
			logoTxt->Release();
	}
};

/// <summary>
/// Checks if a file with the specified name exists.
/// </summary>
/// <param name="name :">The name of the file.</param>
/// <returns>True if the file exists, otherwise false.</returns>
bool		exists_file(const std::string& name);

/// <summary>
/// Extracts the file name from the given file path.
/// </summary>
/// <param name="path :">The file path.</param>
/// <returns>The extracted file name.</returns>
std::string GetFileName(std::string const& path);