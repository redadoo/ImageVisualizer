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
	ImGui::Begin("visualizer", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

  	setPosWidget();

	closeMainPage = ImGuiHelper::ButtonWithPos("x", { 20,20 }, { ImGui::GetContentRegionMax().x - 20, 55 });

/*	MenuBar();
	
	ManageRealTimeThread(); */

	ImGui::PushItemWidth(searchBarSize);
	if (ImGuiHelper::InputTextWithPos("folder", &tmpPath, searchBarPos, ImGuiInputTextFlags_EnterReturnsTrue, true))
		SearchFolder();

/* 	if (dirPathIsCorrect)
		ShowFile();
	
	if (mediaWindow)
	{
		if (fileToShow.at(indexFileToDisplay).type == ImageFileType)
			ShowImage();
		if (fileToShow.at(indexFileToDisplay).type == TextFileType)
			ShowText();
	} */
	ImGui::End();
}

bool Visualizer::isMainPageClosed() const { return closeMainPage; }

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
			/* if (ImGui::BeginMenu("Theme", &showThemeMenu))
			{
				for (int i = 0; i < ImGuiThemes::ThemesCount; i++)
				{
					const ImGuiThemes::Themes theme = ImGuiThemes::IndexToTheme(i);

					const char* name = ImGuiThemes::ThemesToString(theme);
					if (name != NULL && ImGui::MenuItem(name))
					{
						InitTheme(ImGuiThemes::IndexToTheme(i));
					}
				}
				ImGui::EndMenu();
			} */

			ImGui::SeparatorText("search flags");
			ImGuiHelper::HelpMarker("Enable this field to see the change within the folder in real time (file removed, file added, file name changed)"); ImGui::SameLine();
			
			if (ImGui::MenuItem("Realtime visualization", nullptr, &realTimeChange)) {}
			
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
			ImGui::Separator();
			if (ImGuiHelper::MenuItemSwitchable("name", &nameOrder, { &dataCreationOrder ,&typeOrder }))
				fileManager.SortFiles(FileOrder::Alphabetical, ascending, fileToShow);
			if (ImGuiHelper::MenuItemSwitchable("Data creation", &dataCreationOrder, { &nameOrder, &typeOrder }))
				fileManager.SortFiles(FileOrder::DataCreation, ascending, fileToShow);
			if (ImGuiHelper::MenuItemSwitchable("type", &typeOrder, { &nameOrder, &dataCreationOrder }))
				fileManager.SortFiles(FileOrder::Type, ascending , fileToShow);
			ImGui::PopItemFlag();
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("filter by"))
		{
			FileType fileType = NoneFileType;
			ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);
			if (ImGuiHelper::MenuItemSwitchable("all file", &allFileFilter, { &imageFilter,&textFilter }))
				fileType = All;
			if (ImGuiHelper::MenuItemSwitchable("image", &imageFilter, { &allFileFilter,&textFilter }))
				fileType = ImageFileType;
			if (ImGuiHelper::MenuItemSwitchable("text", &textFilter, { &imageFilter,&allFileFilter }))
				fileType = TextFileType;
			if (fileType != NoneFileType && fileType != fileManager.GetFileType())
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
	this->fileButtonSize = {80, 50};

	for (size_t n = 0; n < fileToShow.size(); n++)
	{
		const GenericFile entry = fileToShow.at(n);
		
		if (entry.type == TextFileType)
			fileManager.GetFileLogo()->ShowLogo();
		if (entry.type == ImageFileType)
			ImGui::ImageButton("##image", (void*)(intptr_t)entry.imageFile.image.texture, fileButtonSize);
		if (ImGui::IsItemClicked())
		{
			mediaWindow = true;
			indexFileToDisplay = n;
		}
		ImGuiHelper::SameLineMax(n, ImGui::GetStyle(), fileToShow.size(), ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetItemRectSize().x);
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
/* 	const ImVec2 imageSize(static_cast<float>(fileToShow.at(indexFileToDisplay).imageFile.my_image_width),
		static_cast<float>(fileToShow.at(indexFileToDisplay).imageFile.my_image_height));

	ImGui::Begin(fileToShow.at(indexFileToDisplay).name.c_str(), &mediaWindow, ImGuiWindowFlags_AlwaysAutoResize);
	{
		const auto& imageFile = fileToShow.at(indexFileToDisplay).imageFile;
		ImGui::BeginTabBar("ImageBar", ImGuiTabBarFlags_None);

		if (ImGui::BeginTabItem("Image"))
		{
			ImGuiHelper::ImageCentered(imageFile.image.GetTexture(), imageSize);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Info"))
		{
			const char *sizeText = std::to_string(fileToShow.at(indexFileToDisplay).fileSize).c_str();

			ImGui::Text("%s", fileToShow.at(indexFileToDisplay).name.c_str());
			ImGui::Text("%s", fileToShow.at(indexFileToDisplay).path.string().c_str());
			ImGui::Text("Image size is : %d x %d", imageFile.my_image_height, imageFile.my_image_width);
			ImGui::Text("byte size %s", sizeText);
			ImGui::EndTabItem();
		}
	}
	ImGui::End(); */
}

void Visualizer::ShowText()
{
	ImGui::Begin(fileToShow.at(indexFileToDisplay).name.c_str(), &mediaWindow, ImGuiWindowFlags_AlwaysAutoResize);
	{
		const auto& textFile = fileToShow.at(indexFileToDisplay).textFile;
		ImGui::BeginTabBar("ImageBar", ImGuiTabBarFlags_None);

		if (ImGui::BeginTabItem("Text"))
		{
			for (size_t i = 0; i < textFile.content.size(); i++)
			{
				ImGui::Text("%s", textFile.content.at(i).c_str());
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Info"))
		{
			std::string sizeText = std::to_string(fileToShow.at(indexFileToDisplay).fileSize);

			ImGui::Text("%s", fileToShow.at(indexFileToDisplay).name.c_str());
			ImGui::Text("%s", fileToShow.at(indexFileToDisplay).path.string().c_str());
			ImGui::Text("byte size %s", sizeText.c_str());
			ImGui::EndTabItem();
		}
	}
	ImGui::End();
}

void Visualizer::ScanAndAddFiles(bool isFolderModified)
{
	(void)isFolderModified;
	
	SetPath(tmpPath);

	fileToShow.clear();

	try
	{
		std::filesystem::path correct_path = std::filesystem::path(tmpPath);
		for (const auto& entry : std::filesystem::directory_iterator(correct_path))
		{
			fileManager.AddFiles(entry);
		}
		fileManager.GetFiles(fileToShow);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

}

void Visualizer::SetPath(const std::string& newPath)
{
	(void)newPath;
	{
		std::lock_guard lock( fileManager.pathFolderMutex);
		currentFolder = newPath;
		pathUpdated = true;
	}
	fileManager.cv.notify_all();
}