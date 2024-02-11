#pragma once

#include<iostream>
#include <filesystem>
#include <fstream>
#include"../Engine/Engine.h"

/// <summary>
/// Enum representing different file types.
/// </summary>
enum FileType
{
	Image,
	Text,
	None
};



/// <summary>
/// Determines the type of the specified file based on its name.
/// </summary>
/// <param name="fileName :">The name of the file.</param>
/// <returns>The FileType enum value indicating the type of the file.</returns>
FileType GetFileType(std::string fileName);

/// <summary>
/// Checks if a file with the specified name exists.
/// </summary>
/// <param name="name :">The name of the file.</param>
/// <returns>True if the file exists, otherwise false.</returns>
bool exists_file(const std::string& name);

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
bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

/// <summary>
/// Extracts the file name from the given file path.
/// </summary>
/// <param name="path :">The file path.</param>
/// <returns>The extracted file name.</returns>
std::string GetFileName(std::string const& path);

/// <summary> Reads content from a file into a vector. </summary>
/// <param name="fileName"> Path to the file to read. </param>
/// <param name="content"> Vector to store file content. </param>
/// <returns> True if successful, false otherwise. </returns>
/// <remarks> Displays error message if file cannot be opened. </remarks>
bool ReadContentFile(std::string fileName, std::vector<std::string> *content);  // TODO: find another method for read file


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
///  Represents an generic file.
/// </summary>
struct File
{
	int							fileSize;
	std::string					name;
	std::filesystem::path		path;
	FileType					type;
	ImageFile					imageFile;
	TextFile					textFile;

	File(std::string _name, std::filesystem::path _path) : name(_name), path(_path), type(GetFileType(name)) 
	{
		if (type == FileType::Image)
			imageFile = ImageFile(path.string().c_str());
		else if (type == FileType::Text)
			textFile = TextFile(path.string());
	}

};