#pragma once

#ifdef __linux__
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <GL/gl.h>
	#include "../Engine/EngineLinux.h"
	using ImageTexture = GLuint;
#elif  _WIN32
	# include"../Engine/EngineWin32.h"
	using ImageTexture = ID3D11ShaderResourceView*;
#endif

# include <vector>
# include <functional>
# include <iostream>
# include <filesystem>
# include <fstream>

# define PATH_LOGO_TXT "/Users/edoar/Desktop/GitHub/ImageVisualizer/ImageVisualizer/Logo/txt_logo.png"

namespace FileHelper
{
	/// <summary>
	/// Enum representing different file types.
	/// </summary>
	enum FileType
	{
		All,
		ImageFileType,
		TextFileType,
		SystemFileType,
		NoneFileType
	};

	struct Image
	{
    	ImageTexture texture;

		ImTextureID GetTexture() const;

		void Release();
	};

	bool		LoadTextureFromFile(const char* filename, Image &image, int* out_width, int* out_height);

	/// <summary>
	/// Represents an image file.
	/// </summary>
	struct ImageFile
	{
		Image						image;
		int							my_image_width;
		int							my_image_height;
		bool						errorOnLoadImage;

		ImageFile() = default;

		ImageFile(const char *path);

		/// <summary>
		/// Releases any resources associated with the image.
		/// </summary>
		void ReleaseResources();
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
		std::vector<std::string>	content;
		bool						errorOnReadFile;

		TextFile() = default;

		TextFile(std::string filePath);
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
	struct GenericFile
	{
		std::string					name;
		std::filesystem::path		path;
		FileType					type;
		TextFile					textFile;
		ImageFile					imageFile;
		uintmax_t					fileSize;

		GenericFile(std::string _name, std::filesystem::path _path);
	};

	/// <summary>
	///  Represents different type of file logo.
	/// </summary>
	struct FileLogo
	{
		Image						logo;
		int							my_logo_width;
		int							my_logo_height;
		bool						errorOnLoadingImage;

		FileLogo();

		void ShowLogo(ImVec2 pos) const;

		void ShowLogo() const;

		~FileLogo();
	};

	/// <summary>
	/// Checks if a file with the specified name exists.
	/// </summary>
	/// <param name="name :">The name of the file.</param>
	/// <returns>True if the file exists, otherwise false.</returns>
	bool		exists_file(const std::string name);

	/// <summary>
	/// Extracts the file name from the given file path.
	/// </summary>
	/// <param name="path :">The file path.</param>
	/// <returns>The extracted file name.</returns>
	std::string GetFileName(std::string const& path);
}