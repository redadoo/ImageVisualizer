#include "Visualizer.h"


Visualizer::Visualizer()
{
	mediaWindow = false;
	isFolderChange = false;
	pathUpdated = false;
	exitFolderCheck = false;
	haveFolderHaveChanged = false;

	fileLogo = FileLogo();
}

Visualizer::~Visualizer()
{

}

void Visualizer::ShowMainPage()
{
	ImGui::SetNextWindowSize({ 1280, 800 }, ImGuiCond_Once);
	ImGui::Begin("visualizer", NULL, ImGuiWindowFlags_MenuBar);

	closeMainPage = ImGuiHelper::ButtonWithPos("x", { 20,20 }, { ImGui::GetContentRegionMax().x - 20, 40 });

	MenuBar();
	
	DisplayButtonAndFlag();

	ManageRealTimeThread();

	ImGui::PushItemWidth(700);
	if (ImGuiHelper::InputTextWithPos("Folder", &tmpPath, { 300, 40 }, ImGuiInputTextFlags_EnterReturnsTrue))
		SearchFolder();

	if (dirPathIsCorrect)
		ShowFile();
	
	if (mediaWindow)
	{
		if (files.at(indexFileToDisplay).type == FileType::Image)
			ShowImage();
		else if (files.at(indexFileToDisplay).type == FileType::Text)
			ShowText();
	}

	if (!dirPathIsCorrect)
		ImGui::Text("path not found");

	ImGui::End();
}

void Visualizer::MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("options"))
		{
			ImGui::SeparatorText("style options");
			ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
			if (ImGui::BeginMenu("Theme", &showThemeMenu))
			{
				for (int i = 0; i < ImGuiThemes::ThemesCount; i++)
				{
					ImGuiThemes::Themes theme;
					theme = ImGuiThemes::IndexToTheme(i);

					const char* name = ImGuiThemes::ThemesToString(theme);
					if (name != NULL && ImGui::MenuItem(name))
					{
						ImGuiThemes::InitTheme(ImGuiThemes::IndexToTheme(i));
					}
				}
				ImGui::EndMenu();
			}

			ImGui::SeparatorText("search flags");
			ImGuiHelper::HelpMarker("Enable this field to see the change within the folder in real time (file removed, file added, file name changed)"); ImGui::SameLine();
			
			if (ImGui::MenuItem("Realtime visualization", NULL, &realTimeChange)) {}
			
			ImGui::Separator();
			
			if (ImGui::MenuItem("Quit"))
				closeMainPage = true;
			
			ImGui::PopItemFlag();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void Visualizer::DisplayButtonAndFlag()
{
	ImGuiHelper::ComboWithPos(filter, "filter", { 10, 40 }, 90, &indexFilter , ARRAYSIZE(filter));
	ImGuiHelper::ComboWithPos(order, "order", { 160, 40 }, 90, &indexOrder, ARRAYSIZE(order));
	ImGui::Separator();
}

void Visualizer::ManageRealTimeThread()
{
	if (realTimeChange)
	{
		if (!threadfolderCheck.joinable())
			threadfolderCheck = std::thread(&Visualizer::CheckFolder, this);

		if (haveFolderHaveChanged.load(std::memory_order_acquire))
		{
			ScanAndAddFiles(true);
			haveFolderHaveChanged.store(false, std::memory_order_release);
		}
	}
	else if (!realTimeChange && threadfolderCheck.joinable())
		exitFolderCheck.store(true, std::memory_order_release);
}

void Visualizer::SearchFolder()
{
	dirPathIsCorrect = exists_file(tmpPath);
	if (dirPathIsCorrect)
		ScanAndAddFiles(false);
}

void Visualizer::ShowFile()
{
	ImVec2		buttonSize = { 80,50 };
	ImGuiStyle& style = ImGui::GetStyle();

	for (size_t n = 0; n < files.size(); n++)
	{
		File entry = files.at(n);

		if (entry.type == FileType::Text && (indexFilter == 0 || indexFilter == 2))
		{
			fileLogo.ShowLogo();
			if (ImGui::IsItemClicked())
			{
				mediaWindow = true;
				indexFileToDisplay = n;
			}
		}
		else if (entry.type == FileType::Image && (indexFilter == 0 || indexFilter == 1))
		{
			ImGui::ImageButton("##image", entry.imageFile.image, buttonSize);
			if (ImGui::IsItemClicked())
			{
				indexFileToDisplay = n;
				mediaWindow = true;
			}
		}
		ImGuiHelper::SameLineMax(n, style, files.size(), ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetItemRectSize().x);
	}
}

void Visualizer::CheckFolder()
{
	std::unique_lock<std::mutex> lock(pathFolderMutex);
	cv.wait(lock, [this] { return pathUpdated; });
	
	std::string path = currentFolder;
	std::wstring stemp = std::wstring(path.begin(), path.end());

	LPCWSTR sw = stemp.c_str();

	HANDLE file = CreateFile(sw,
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL);
	assert(file != INVALID_HANDLE_VALUE);
	OVERLAPPED overlapped;
	overlapped.hEvent = CreateEvent(NULL, FALSE, 0, NULL);

	uint8_t change_buf[1024];
	BOOL success = ReadDirectoryChangesW(
		file, change_buf, 1024, TRUE,
		FILE_NOTIFY_CHANGE_FILE_NAME |
		FILE_NOTIFY_CHANGE_DIR_NAME |
		FILE_NOTIFY_CHANGE_LAST_WRITE,
		NULL, &overlapped, NULL);

	while (!exitFolderCheck.load(std::memory_order_acquire))
	{
		DWORD result = WaitForSingleObject(overlapped.hEvent, 0);
		if (result == WAIT_OBJECT_0) {
			DWORD bytes_transferred;
			GetOverlappedResult(file, &overlapped, &bytes_transferred, FALSE);

			FILE_NOTIFY_INFORMATION* event = (FILE_NOTIFY_INFORMATION*)change_buf;

			for (;;) 
			{
				DWORD name_len = event->FileNameLength / sizeof(wchar_t);
				isFolderChange.store(false, std::memory_order::memory_order_relaxed);
				switch (event->Action)
				{
					case FILE_ACTION_ADDED: {
						wprintf(L"       Added: %.*s\n", name_len, event->FileName);
						haveFolderHaveChanged.store(true, std::memory_order_release);
					} break;

					case FILE_ACTION_REMOVED: {
						wprintf(L"     Removed: %.*s\n", name_len, event->FileName);
						haveFolderHaveChanged.store(true, std::memory_order_release);
					} break;

					case FILE_ACTION_MODIFIED: {
						wprintf(L"    Modified: %.*s\n", name_len, event->FileName);
						haveFolderHaveChanged.store(true, std::memory_order_release);
					} break;

					case FILE_ACTION_RENAMED_OLD_NAME: {
						wprintf(L"Renamed from: %.*s\n", name_len, event->FileName);
						haveFolderHaveChanged.store(true, std::memory_order_release);
					} break;

					case FILE_ACTION_RENAMED_NEW_NAME: {
						wprintf(L"          to: %.*s\n", name_len, event->FileName);
						haveFolderHaveChanged.store(true, std::memory_order_release);
					} break;

					default: {
						printf("Unknown action!\n");
					} break;
				}

				if (event->NextEntryOffset) {
					*((uint8_t**)&event) += event->NextEntryOffset;
				}
				else {
					break;
				}
			}

			// Queue the next event
			BOOL success = ReadDirectoryChangesW(
				file, change_buf, 1024, TRUE,
				FILE_NOTIFY_CHANGE_FILE_NAME |
				FILE_NOTIFY_CHANGE_DIR_NAME |
				FILE_NOTIFY_CHANGE_LAST_WRITE,
				NULL, &overlapped, NULL);

		}
	}
	exitFolderCheck.store(false, std::memory_order_release);
}

void Visualizer::ShowImage()
{
	int offSetWindowSize = 20;
	
	ImVec2 imageSize((float)files.at(indexFileToDisplay).imageFile.my_image_width, files.at(indexFileToDisplay).imageFile.my_image_height);
	ImGui::Begin(files.at(indexFileToDisplay).name.c_str(), &mediaWindow, ImGuiWindowFlags_AlwaysAutoResize);
	{
		auto& imageFile = files.at(indexFileToDisplay).imageFile;
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		ImGui::BeginTabBar("ImageBar", tab_bar_flags);

		if (ImGui::BeginTabItem("Image"))
		{
			ImGuiHelper::ImageCentered(imageFile.image, imageSize);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Info"))
		{
			std::string sizeText;
			sizeText = "Image size is : " + std::to_string(imageFile.my_image_height) + " x " + std::to_string(imageFile.my_image_width);
			
			ImGui::Text(files.at(indexFileToDisplay).name.c_str());
			ImGui::Text(files.at(indexFileToDisplay).path.string().c_str());
			ImGui::Text(sizeText.c_str());
			ImGui::EndTabItem();
		}
	}
	ImGui::End();
}

void Visualizer::ShowText()
{
	int offSetWindowSize = 20;
	ImGui::Begin(files.at(indexFileToDisplay).name.c_str(), &mediaWindow, ImGuiWindowFlags_AlwaysAutoResize);
	{
		auto& textFile = files.at(indexFileToDisplay).textFile;
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		ImGui::BeginTabBar("ImageBar", tab_bar_flags);

		if (ImGui::BeginTabItem("Text"))
		{
			for (size_t i = 0; i < textFile.content.size(); i++)
			{
				ImGui::Text(textFile.content.at(i).c_str());
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Info"))
		{
			std::string sizeText;
			//sizeText = "Image size is : " + std::to_string(textFile.my_image_height) + " x " + std::to_string(textFile.my_image_width);

			ImGui::Text(files.at(indexFileToDisplay).name.c_str());
			ImGui::Text(files.at(indexFileToDisplay).path.string().c_str());
			ImGui::Text(sizeText.c_str());
			ImGui::EndTabItem();
		}
	}
	ImGui::End();

}


void Visualizer::ScanAndAddFiles(bool isFolderModified)
{
	SetPath(tmpPath);

	files.clear();

	std::filesystem::path correct_path = std::filesystem::u8path(tmpPath);
	for (const auto& entry : std::filesystem::directory_iterator(correct_path))
		files.push_back({ GetFileName(entry.path().string()), entry.path()});
}

void Visualizer::SetPath(const std::string& newPath)
{
	{
		std::lock_guard<std::mutex> lock(pathFolderMutex);
		currentFolder = newPath;
		pathUpdated = true;
	}
	cv.notify_all();
}
