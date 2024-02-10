#pragma once

#include<iostream>
#include <filesystem>

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
/// Loads a texture from the specified file and creates a shader resource view.
/// </summary>
/// <param name="filename :">The filename of the texture.</param>
/// <param name="out_srv  :">Pointer to a pointer to store the created shader resource view.</param>
/// <param name="out_width :">Pointer to store the width of the loaded texture.</param>
/// <param name="out_height :">Pointer to store the height of the loaded texture.</param>
/// <returns>True if the texture is loaded successfully, otherwise false.</returns>
bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

/// <summary>
/// Extracts the file name from the given file path.
/// </summary>
/// <param name="path :">The file path.</param>
/// <returns>The extracted file name.</returns>
std::string GetFileName(std::string const& path);

/// <summary>
/// Represents an image file.
/// </summary>
struct ImageFile
{
	std::string					name;
	std::filesystem::path		path;
	bool						errorOnLoadImage;
	ID3D11ShaderResourceView*	image = NULL;
	int							my_image_width = 0;
	int							my_image_height = 0;


	/// <summary>
	/// Initializes the image by loading it from the file path.
	/// </summary>
	void InitImage()
	{
		errorOnLoadImage = !LoadTextureFromFile(path.string().c_str(), &image, &my_image_width, &my_image_height);
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