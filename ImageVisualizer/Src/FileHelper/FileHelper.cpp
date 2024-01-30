#include "FileHelper.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

/// <summary>
/// Checks if a file path exists in the file system.
/// </summary>
/// <param name="pathName">A constant reference to a std::string representing the file path to be checked for existence.</param>
/// <returns>
/// Returns true if the specified file path exists, and false otherwise.
/// </returns>
 bool exists_file(const std::string& pathName) 
 {
	struct stat buffer;
	return (stat(pathName.c_str(), &buffer) == 0);
 }

/// <summary>
/// Converts a null-terminated string to an unsigned integer using a hash function.
/// </summary>
/// <param name="str">A pointer to a null-terminated character array representing the input string.</param>
/// <param name="h">Optional: The current hash value during recursive calls (default is 0).</param>
/// <returns>
/// Returns the computed unsigned integer value based on the input string using a hash algorithm.
/// </returns>
 constexpr unsigned int str2int(const char* str, int h = 0)
 {
	 return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
 }

/// <summary>
/// Determines the file type based on the provided file name's extension.
/// </summary>
/// <param name="fileName">A std::string containing the file name with extension.</param>
/// <returns>
/// Returns a value of the enum FileType, representing the detected file type.
/// Possible return values include Image for common image file extensions (jpg, png, tga, bmp, psd, gif, hdr, pic),
/// and None for file types that are not explicitly handled.
/// </returns>
 FileType GetFileType(std::string fileName)
 {
	 size_t dotPosition = fileName.find_last_of(".");
	 if (dotPosition != std::string::npos)
	 {
		 std::string fileExtension = fileName.substr(dotPosition + 1);
		 switch (str2int(fileExtension.c_str()))
		 {
		 case str2int("jpg"):
			 return Image;
		 case str2int("png"):
			 return Image;
		 case str2int("tga"):
			 return Image;
		 case str2int("bmp"):
			 return Image;
		 case str2int("psd"):
			 return Image;
		 case str2int("gif"):
			 return Image;
		 case str2int("hdr"):
			 return Image;
		 case str2int("pic"):
			 return Image;
		 case str2int("txt"):
			 return Image;
		 default:
			 return None;
		 }
	 }
	 return None;
 }

 /// <summary>
 /// Loads a texture from a file using the STB Image library and creates a Direct3D 11 shader resource view.
 /// </summary>
 /// <param name="filename">The file path of the image to be loaded.</param>
 /// <param name="out_srv">A pointer to a pointer that will store the created ID3D11ShaderResourceView.</param>
 /// <param name="out_width">A pointer to an integer that will store the width of the loaded image.</param>
 /// <param name="out_height">A pointer to an integer that will store the height of the loaded image.</param>
 /// <returns>
 /// Returns true if the texture is successfully loaded and the shader resource view is created;
 /// otherwise, returns false and outputs the failure reason to the standard output.
 /// </returns>
 bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
 {
     // Load from disk into a raw RGBA buffer
     int image_width = 0;
     int image_height = 0;
     unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
     if (image_data == NULL)
     {
         std::cout << stbi_failure_reason() << "\n";
         return false;
     }

     // Create texture
     D3D11_TEXTURE2D_DESC desc;
     ZeroMemory(&desc, sizeof(desc));
     desc.Width = image_width;
     desc.Height = image_height;
     desc.MipLevels = 1;
     desc.ArraySize = 1;
     desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
     desc.SampleDesc.Count = 1;
     desc.Usage = D3D11_USAGE_DEFAULT;
     desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
     desc.CPUAccessFlags = 0;

     ID3D11Texture2D* pTexture = NULL;
     D3D11_SUBRESOURCE_DATA subResource;
     subResource.pSysMem = image_data;
     subResource.SysMemPitch = desc.Width * 4;
     subResource.SysMemSlicePitch = 0;
     Engine::pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

     // Create texture view
     D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
     ZeroMemory(&srvDesc, sizeof(srvDesc));
     srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
     srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
     srvDesc.Texture2D.MipLevels = desc.MipLevels;
     srvDesc.Texture2D.MostDetailedMip = 0;
     Engine::pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
     pTexture->Release();

     *out_width = image_width;
     *out_height = image_height;
     stbi_image_free(image_data);

     return true;
 }