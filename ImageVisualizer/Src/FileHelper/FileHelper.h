#pragma once

#include<iostream>
#include <filesystem>

#include"../Engine/Engine.h"


enum FileType
{
	Image,
	Text,
	None
};


FileType GetFileType(std::string fileName);
bool exists_file(const std::string& name);
bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

struct ImageFile
{
	std::string					name;
	std::filesystem::path		path;
	bool						errorOnLoadImage;
	ID3D11ShaderResourceView*	image = NULL;
	int							my_image_width = 0;
	int							my_image_height = 0;

	void InitImage()
	{
		errorOnLoadImage = !LoadTextureFromFile(path.string().c_str(), &image, &my_image_width, &my_image_height);
	}

	void ReleaseResources()
	{
		if (image)
		{
			image->Release();
			image = nullptr;
		}
	}

};