#include "FileHelper.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"


File::File(std::string _name, std::filesystem::path _path): name(std::move(_name)), path(std::move(_path)), type(GetFileType(name))
{

	if (type == Image)
		imageFile = ImageFile(path.string().c_str());
	else if (type == Text)
		textFile = TextFile(path.string());

    //GetFileTimeCreation(creationTime);
	WIN32_FILE_ATTRIBUTE_DATA	fileInfo;
	if (GetFileAttributesEx(path.string().c_str(), GetFileExInfoStandard, &fileInfo) != 0) {

		FileTimeToSystemTime(&fileInfo.ftLastAccessTime, &lastAccessTime.date);
		FileTimeToSystemTime(&fileInfo.ftCreationTime, &creationTime.date);
	}
	else {
		DWORD error = GetLastError();
		std::cerr << "Failed to get file attributes. Error code: " << error << std::endl;
	}

}

//void GetFileTimeCreation(Date &date)
//{
//#ifdef __linux__ 
//
//#elif _WIN32
//
//#else
//
//#endif
//
//}


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
    
ImageFile::ImageFile(const char* path)
{
	errorOnLoadImage = !LoadTextureFromFile(path, &image, &my_image_width, &my_image_height);
}


FileLogo::FileLogo()
{
	errorOnLoadingImage = !LoadTextureFromFile(PATH_LOGO_TXT, &logo, &my_logo_width, &my_logo_height);
}

void FileLogo::ShowLogo(const ImVec2 pos) const
{
	ImGui::SetCursorPos(pos);
	ImGui::Image(logo, ImVec2(60, 60));
}

void FileLogo::ShowLogo() const
{
	ImGui::ImageButton(logo, ImVec2(60, 60));
}

FileLogo::~FileLogo()
{
	if (!errorOnLoadingImage)
		logo->Release();
}

bool exists_file(const std::string& pathName) 
 {
    std::string correct_path = std::filesystem::path(pathName).string();
	struct stat buffer;
	return (stat(correct_path.c_str(), &buffer) == 0);
 }

TextFile::TextFile(std::string filePath)
{
	errorOnReadFile = !ReadContentFile(filePath, &content);
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
		     case str2int("ini"):
		 	    return System;
		     default:
			     return NoneFileType;
		 }
	 }
	 return NoneFileType;
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


void ImageFile::ReleaseResources()
{
	if (image != NULL)
	{
		image->Release();
		image = NULL;
	}
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

bool Date::operator>(const Date &other) const
{
    //int res;
    //FILETIME test;
    //FILETIME test1;

    //FileTimeToSystemTime(&test, this->date);
    //FileTimeToSystemTime(&test1, other.date);
    //res = CompareFileTime(&test1, &test);
    //if (res == -1)
    //    return true;
    //else
        return false;
}

bool Date::operator<(const Date& other) const
{
    FILETIME ft1, ft2;
    SystemTimeToFileTime(&this->date, &ft1);
    SystemTimeToFileTime(&other.date, &ft2);
    return CompareFileTime(&ft1, &ft2) == -1;
}
