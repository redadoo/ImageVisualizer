#include "FileHelper.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"


/// <summary>
/// Converts a null-terminated string to an unsigned integer using a hash function.
/// </summary>
/// <param name="str">A pointer to a null-terminated character array representing the input string.</param>
/// <param name="h">Optional: The current hash value during recursive calls (default is 0).</param>
/// <returns>
/// Returns the computed unsigned integer value based on the input string using a hash algorithm.
/// </returns>
constexpr unsigned int str2int(const char* str, int _h = 0)
{
	 return !str[_h] ? 5381 : (str2int(str, _h + 1) * 33) ^ str[_h];
}

bool exists_file(const std::string& pathName) 
 {
    std::string correct_path = std::filesystem::u8path(pathName).string();
	struct stat buffer;
	return (stat(correct_path.c_str(), &buffer) == 0);
 }

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
			 return Text;
		 default:
			 return None;
		 }
	 }
	 return None;
 }

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

std::string GetFileName(std::string const& path)
 {
     return path.substr(path.find_last_of("/\\") + 1);
 }

bool ReadContentFile(std::string filePath, std::vector<std::string> *content)
{
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        content->push_back(line);
    }

    file.close();
    return true;
}
