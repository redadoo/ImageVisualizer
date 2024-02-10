#include "Visualizer.h"


Visualizer::Visualizer()
{
	imageWindow = false;
	isFolderChange = false;
	dirPathIsCorrect = true;
	pathUpdated = false;
	exitFolderCheck = false;
	haveFolderHaveChanged = false;
}

Visualizer::~Visualizer()
{

}

void Visualizer::ShowMainPage()
{
	ImGui::SetNextWindowSize({ 1280, 800 }, ImGuiCond_Once);
	ImGui::Begin("app");

	DisplayFlag();

	closeMainPage = ImGuiHelper::ButtonWithPos(" x ", { 30,30 }, { ImGui::GetContentRegionMax().x - 30, 30 });

	ManageRealTimeThread();

	if (ImGuiHelper::InputTextWithPos("Folder to visualize", &tmpPath, { 220, 75 }, ImGuiInputTextFlags_EnterReturnsTrue))
		SearchFolder();

	if (dirPathIsCorrect)
		ShowFile();
	
	if (imageWindow)
		ShowImage();

	if (!dirPathIsCorrect)
		ImGui::Text("path not found");

	ImGui::Separator();

	ImGui::End();
}

void Visualizer::DisplayFlag()
{
	ImGui::Text("Real time visualization"); ImGui::SameLine();
	ImGui::Checkbox("##checkRealTime", &realTimeChange);
}

void Visualizer::ManageRealTimeThread()
{
	if (realTimeChange)
	{
		if (!threadfolderCheck.joinable())
			threadfolderCheck = std::thread(&Visualizer::CheckFolder, this);

		if (haveFolderHaveChanged.load(std::memory_order_acquire))
		{
			ScanAndAddImageFiles(true);
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
		ScanAndAddImageFiles(false);
}

void Visualizer::ShowFile()
{
	ImVec2		buttonSize = { 80,50 };
	ImGuiStyle& style = ImGui::GetStyle();
	size_t		buttons_count = imageFiles.size();

	for (size_t n = 0; n < buttons_count; n++)
	{
		ImageFile entry = imageFiles.at(n);
		ImVec2 currentCurPos = ImGui::GetCursorPos();

		ImGui::ImageButton("##image", entry.image, buttonSize);
		ImGuiHelper::SameLineMax(n, style, buttons_count, ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x, buttonSize.x);

		if (ImGui::IsItemClicked())
		{
			imageWindow = true;
			indexImageToDisplay = n;
			return;
		}

		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 160);
		ImGuiHelper::TextWithPos(entry.name.c_str(), { currentCurPos.x,currentCurPos.y + buttonSize.y + 10 }, false);
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

	ImVec2 imageSize(imageFiles.at(indexImageToDisplay).my_image_width, imageFiles.at(indexImageToDisplay).my_image_height);
	ImGui::Begin(imageFiles.at(indexImageToDisplay).name.c_str(), &imageWindow, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImageFile imageFile;
		imageFile = imageFiles.at(indexImageToDisplay);
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
			
			ImGui::Text(imageFile.name.c_str());
			ImGui::Text(imageFile.path.string().c_str());
			ImGui::Text(sizeText.c_str());
			ImGui::EndTabItem();
		}
	}
	ImGui::End();
}

void Visualizer::ScanAndAddImageFiles(bool isFolderModified)
{
	SetPath(tmpPath);

	std::cout << imageFiles.size() << "no \n";
	if (imageFiles.size() > 0)
		imageFiles.clear();

	std::filesystem::path correct_path = std::filesystem::u8path(tmpPath);
	for (const auto& entry : std::filesystem::directory_iterator(correct_path))
	{
		if (GetFileType(entry.path().string()) == Image)
		{
			ImageFile imageFile;
			imageFile.path = entry.path();
			imageFile.name = GetFileName(entry.path().string());
			imageFile.InitImage();
			imageFiles.push_back(imageFile);
		}
	}
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
