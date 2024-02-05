#include "Visualizer.h"

using namespace App;

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

void Visualizer::StartUpPage()
{
	ImGui::SetNextWindowSize({ 1280, 800 }, ImGuiCond_Once);
	ImGui::Begin("app", &StartUpPageDraw, ImGuiWindowFlags_NoCollapse);
	{
		SearchFlag();
		
		if (haveFolderHaveChanged.load(std::memory_order_acquire))
		{
			ScanAndAddImageFiles();
			haveFolderHaveChanged.store(false, std::memory_order_release);
		}

		if (ImGuiHelper::InputTextWithPos("Folder to visualize", &tmpPath, { 200, 85 }, ImGuiInputTextFlags_EnterReturnsTrue))
			SearchFolder();
		
		if (realTimeChange && !threadfolderCheck.joinable())
			threadfolderCheck = std::thread(&Visualizer::CheckFolder, this);
		else if (!realTimeChange && threadfolderCheck.joinable())
			exitFolderCheck.store(true, std::memory_order_release);

		if (dirPathIsCorrect)
			ShowFile();
		
		if (openWindows)
			ShowImage();

		ShowError();

		ImGui::Separator();
	}
	ImGui::End();
}

void Visualizer::ShowError()
{
	if (!dirPathIsCorrect)
		ImGui::Text("path not found");
}

void Visualizer::AddFilesVector(std::filesystem::path path)
{
	if (GetFileType(path.string()) == Image)
	{
		ImageFile imageFile;
		imageFile.path = path;
		imageFile.InitImage();
		imageFiles.push_back(imageFile);
	}
}

void Visualizer::SearchFlag()
{
	ImGui::Text("Debug mode"); ImGui::SameLine();
	ImGui::Checkbox("##checkDebugMode", &debugModeIsOn);
	ImGui::Text("Real time visualization"); ImGui::SameLine();
	ImGui::Checkbox("##checkRealTime", &realTimeChange);
	ImGui::Separator();
}

void Visualizer::SearchFolder()
{
	if (exists_file(tmpPath))
	{
		SetPath(tmpPath);
		
		dirPathIsCorrect = true;

		ScanAndAddImageFiles();
	}
	else
	{
		dirPathIsCorrect = false;
	}
	
}

void Visualizer::ShowFile()
{
	ImGuiStyle& style = ImGui::GetStyle();
	size_t buttons_count = imageFiles.size();
	float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
	ImVec2 buttonSize = { 80,50 };
	for (int n = 0; n < imageFiles.size(); n++)
	{
		ImageFile entry = imageFiles.at(n);
		ImGui::ImageButton("##image", entry.image, buttonSize);
		ImGuiHelper::SameLineMax(n, style, buttons_count, window_visible_x2, buttonSize.x);

		if (ImGui::IsItemClicked()) 
		{
			openWindows = true;
			indexImageToDisplay = n;

			return;
		}

	}
}

bool Visualizer::IsFolderChanged()
{
	return false;
}

void Visualizer::ClearImage()
{
	for (auto& imageFile : imageFiles)
	{
		imageFile.ReleaseResources();
	}
	imageFiles.clear();
}

void Visualizer::ShowImage()
{
	//int offSetWindowSize = 20;

	//ImVec2 imageSize(imageFiles.at(indexImageToDisplay).my_image_width, imageFiles.at(indexImageToDisplay).my_image_height);
	//ImGui::Begin("visualizer", &openWindows, ImGuiWindowFlags_AlwaysAutoResize);
	//{
	//	ImGuiHelper::ImageCentered(imageFiles.at(indexImageToDisplay).image, imageSize);
	//}
	//ImGui::End();
}

void Visualizer::ScanAndAddImageFiles()
{
	if (imageFiles.size() > 0)
		ClearImage();

	std::filesystem::path correct_path = std::filesystem::u8path(tmpPath);
	for (const auto& entry : std::filesystem::directory_iterator(correct_path))
		AddFilesVector(entry);
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