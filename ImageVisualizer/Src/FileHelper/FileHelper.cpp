#include "FileHelper.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"


using namespace FileHelper;

GenericFile::GenericFile(std::string _name, std::filesystem::path _path): name(std::move(_name)), path(std::move(_path)), type(GetFileType(name))
{
	if (type == ImageFileType)
		imageFile = ImageFile(path.string().c_str());
	else if (type == TextFileType)
		textFile = TextFile(path.string());
	
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
	errorOnLoadImage = !LoadTextureFromFile(path, image, &my_image_width, &my_image_height);
}


FileLogo::FileLogo()
{
	errorOnLoadingImage = !LoadTextureFromFile(PATH_LOGO_TXT, logo, &my_logo_width, &my_logo_height);
}

void FileLogo::ShowLogo(const ImVec2 pos) const
{
	(void)pos;
	ImGui::SetCursorPos(pos);
	ImGui::Image(logo.GetTexture(), ImVec2(60, 60));
}

void FileLogo::ShowLogo() const
{
	ImGui::ImageButton(logo.GetTexture(), ImVec2(60, 60));
}

FileLogo::~FileLogo()
{

}

bool FileHelper::exists_file(const std::string pathName)  
{
	return std::filesystem::exists(pathName);
}

TextFile::TextFile(std::string filePath)
{
	errorOnReadFile = !ReadContentFile(filePath, &content);
}

FileType FileHelper::GetFileType(std::string fileName)
{
	size_t dotPosition = fileName.find_last_of(".");
	if (dotPosition != std::string::npos)
	{
		std::string fileExtension = fileName.substr(dotPosition + 1);
		switch (str2int(fileExtension.c_str()))
		{
			case str2int("jpg"):
				return ImageFileType;
			case str2int("png"):
				return ImageFileType;
			case str2int("tga"):
				return ImageFileType;
			case str2int("bmp"):
				return ImageFileType;
			case str2int("psd"):
				return ImageFileType;
			case str2int("gif"):
				return ImageFileType;
			case str2int("hdr"):
				return ImageFileType;
			case str2int("pic"):
				return ImageFileType;
			case str2int("txt"):
				return ImageFileType;
			case str2int("ini"):
			return SystemFileType;
			default:
				return NoneFileType;
		}
	}
	return NoneFileType;
}

bool FileHelper::LoadTextureFromFile(const char* filename, Image &image, int* out_width, int* out_height)
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

#ifdef __linux__
	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
	#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	image.texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;
#elif _WIN32
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
#endif

return true;
}

std::string FileHelper::GetFileName(std::string const& path)
{
	return path.substr(path.find_last_of("/\\") + 1);
}

bool FileHelper::ReadContentFile(std::string filePath, std::vector<std::string> *content)
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

ImTextureID Image::GetTexture() const
{
    return (void*)(intptr_t)this->texture;
}

void Image::Release()
{
	#ifdef __linux__
		glDeleteTextures(1, &texture);
	#elif _WIN32
		texture->release();
	#endif
}
