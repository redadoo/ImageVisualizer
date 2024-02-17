#include "Visualizer.h"


Visualizer::Visualizer()
{
	mediaWindow = false;
	pathUpdated = false;
	ascending = true;
	descending = false;
	nameOrder = true;
	dataCreationOrder = false;
	typeOrder = false;
	allFileFilter = true;
	searchBarSize = 500;
}

Visualizer::~Visualizer()
{

}

void Visualizer::ShowMainPage()
{
	ImGui::SetNextWindowSize({ 1280, 800 }, ImGuiCond_Once);
	ImGui::Begin("visualizer", NULL, ImGuiWindowFlags_MenuBar);

	setPosWidget();

	closeMainPage = ImGuiHelper::ButtonWithPos("x", { 20,20 }, { ImGui::GetContentRegionMax().x - 20, 55 });

	MenuBar();
	
	ManageRealTimeThread();

	ImGui::PushItemWidth(searchBarSize);
	if (ImGuiHelper::InputTextWithPos("folder", &tmpPath, searchBarPos, ImGuiInputTextFlags_EnterReturnsTrue, true))
		SearchFolder();

	if (dirPathIsCorrect)
		ShowFile();
	
	if (mediaWindow)
	{
		if (fileToShow.at(indexFileToDisplay).type == FileType::Image)
		{
			std::cout << "luca\n";
			ShowImage();
		}
		if (fileToShow.at(indexFileToDisplay).type == FileType::Text)
		{
			std::cout << "toni\n";
			ShowText();
		}
	}

	ImGui::End();
}

void Visualizer::setPosWidget()
{
	//handle search bar resizing with window

	searchBarSize = 500;
	searchBarPos = { (ImGui::GetContentRegionMax().x / 2) - (searchBarSize / 2) , 55 };
	if (ImGui::GetContentRegionMax().x < searchBarSize + 35)
	{
		searchBarSize = ImGui::GetContentRegionMax().x - 50;
		searchBarPos.x = 0;
	}
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
		
		if (ImGui::BeginMenu("order by"))
		{
			ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
			ImGuiHelper::MenuItemSwitchable("ascending", &ascending, { &descending });
			ImGuiHelper::MenuItemSwitchable("descending", &descending, { &ascending });
			if (ImGuiHelper::MenuItemSwitchable("name", &nameOrder, { &dataCreationOrder ,&typeOrder }))
				fileManager.SortFiles(FileOrder::Alphabetical, ascending, fileToShow);
			if (ImGuiHelper::MenuItemSwitchable("Data creation", &dataCreationOrder, { &nameOrder, &typeOrder }))
				fileManager.SortFiles(FileOrder::DataCreation, ascending, fileToShow);
			if (ImGuiHelper::MenuItemSwitchable("type", &typeOrder, { &nameOrder, &dataCreationOrder }))
				fileManager.SortFiles(FileOrder::Type, ascending , fileToShow);
			ImGui::Separator();
			ImGui::PopItemFlag();
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("filter by"))
		{
			FileType fileType = FileType::None;
			ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
			if (ImGuiHelper::MenuItemSwitchable("all file", &allFileFilter, { &imageFilter,&textFilter }))
				fileType = FileType::All;
			if (ImGuiHelper::MenuItemSwitchable("image", &imageFilter, { &allFileFilter,&textFilter }))
				fileType = FileType::Image;
			if (ImGuiHelper::MenuItemSwitchable("text", &textFilter, { &imageFilter,&allFileFilter }))
				fileType = FileType::Text;
			if (fileType != FileType::None && fileType != fileManager.GetFileType())
			{
				fileToShow.clear();
				fileManager.SetFileType(fileType);
				fileManager.GetFiles(fileToShow);
			}
			ImGui::PopItemFlag();
			ImGui::EndMenu();
		}
		
		ImGui::EndMenuBar();
	}
}

void Visualizer::ManageRealTimeThread()
{
	//if (realTimeChange)
	//{
	//	if (!threadfolderCheck.joinable())
	//		threadfolderCheck = std::thread(&Visualizer::CheckFolder, this);

	//	if (haveFolderHaveChanged.load(std::memory_order_acquire))
	//	{
	//		ScanAndAddFiles(true);
	//		haveFolderHaveChanged.store(false, std::memory_order_release);
	//	}
	//}
	//else if (!realTimeChange && threadfolderCheck.joinable())
	//	exitFolderCheck.store(true, std::memory_order_release);
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
	size_t		filesSize = fileToShow.size();

	for (size_t n = 0; n < filesSize; n++)
	{
		File entry = fileToShow.at(n);
		if (entry.type == FileType::Text)
			fileManager.GetFileLogo()->ShowLogo();
		else if (entry.type == FileType::Image)
			ImGui::ImageButton("##image", entry.imageFile.image, buttonSize);
		if (ImGui::IsItemClicked())
		{
			mediaWindow = true;
			indexFileToDisplay = n;
		}
		ImGuiHelper::SameLineMax(n, style, filesSize, ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetItemRectSize().x);
	}
}

void Visualizer::CheckFolder()
{
	/*std::unique_lock<std::mutex> lock(pathFolderMutex);
	cv.wait(lock, [this] { return pathUpdated; });
	
	std::string path = currentFolder;
	std::wstring stemp = std::wstring(path.begin(), path.end());

	LPCWSTR sw = stemp.c_str();

	HANDLE file = CreateFile(path.c_str(),
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

			 Queue the next event
			BOOL success = ReadDirectoryChangesW(
				file, change_buf, 1024, TRUE,
				FILE_NOTIFY_CHANGE_FILE_NAME |
				FILE_NOTIFY_CHANGE_DIR_NAME |
				FILE_NOTIFY_CHANGE_LAST_WRITE,
				NULL, &overlapped, NULL);

		}
	}
	exitFolderCheck.store(false, std::memory_order_release);*/
}

void Visualizer::ShowImage()
{
	int offSetWindowSize = 20;
	
	ImVec2 imageSize((float)fileToShow.at(indexFileToDisplay).imageFile.my_image_width, (float)fileToShow.at(indexFileToDisplay).imageFile.my_image_height);
	ImGui::Begin(fileToShow.at(indexFileToDisplay).name.c_str(), &mediaWindow, ImGuiWindowFlags_AlwaysAutoResize);
	{
		auto& imageFile = fileToShow.at(indexFileToDisplay).imageFile;
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		ImGui::BeginTabBar("ImageBar", tab_bar_flags);

		if (ImGui::BeginTabItem("Image"))
		{
			ImGuiHelper::ImageCentered(imageFile.image, imageSize);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Info"))
		{
			std::string sizeText = std::to_string(fileToShow.at(indexFileToDisplay).fileSize);

			ImGui::Text(fileToShow.at(indexFileToDisplay).name.c_str());
			ImGui::Text(fileToShow.at(indexFileToDisplay).path.string().c_str());
			ImGui::Text("Image size is : %d x %d", imageFile.my_image_height, imageFile.my_image_width);
			ImGui::Text("byte size %s", sizeText);
			ImGui::EndTabItem();
		}
	}
	ImGui::End();
}

void Visualizer::ShowText()
{
	int offSetWindowSize = 20;
	ImGui::Begin(fileToShow.at(indexFileToDisplay).name.c_str(), &mediaWindow, ImGuiWindowFlags_AlwaysAutoResize);
	{
		auto& textFile = fileToShow.at(indexFileToDisplay).textFile;
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
			std::string sizeText = std::to_string(fileToShow.at(indexFileToDisplay).fileSize);

			ImGui::Text(fileToShow.at(indexFileToDisplay).name.c_str());
			ImGui::Text(fileToShow.at(indexFileToDisplay).path.string().c_str());
			ImGui::Text(fileToShow.at(indexFileToDisplay).path.string().c_str());
			ImGui::Text("byte size %s", sizeText);
			ImGui::EndTabItem();
		}
	}
	ImGui::End();
}

void Visualizer::ScanAndAddFiles(bool isFolderModified)
{
	SetPath(tmpPath);

	fileToShow.clear();

	std::filesystem::path correct_path = std::filesystem::u8path(tmpPath);
	for (const auto& entry : std::filesystem::directory_iterator(correct_path))
		fileManager.AddFiles(entry);
	fileManager.GetFiles(fileToShow);
}

void Visualizer::SetPath(const std::string& newPath)
{
	{
		std::lock_guard<std::mutex> lock( fileManager.pathFolderMutex);
		currentFolder = newPath;
		pathUpdated = true;
	}
	fileManager.cv.notify_all();
}
