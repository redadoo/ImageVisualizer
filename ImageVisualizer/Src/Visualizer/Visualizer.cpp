#include "Visualizer.h"

using namespace App;

void Visualizer::CheckFolder()
{
	std::string path = ReadFilePath();

	std::cout << "watching " << path << " for changes...\n";

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

	while (true) {

		DWORD result = WaitForSingleObject(overlapped.hEvent, 0);

		if (result == WAIT_OBJECT_0) {
			DWORD bytes_transferred;
			GetOverlappedResult(file, &overlapped, &bytes_transferred, FALSE);

			FILE_NOTIFY_INFORMATION* event = (FILE_NOTIFY_INFORMATION*)change_buf;

			for (;;) 
			{
				DWORD name_len = event->FileNameLength / sizeof(wchar_t);

				switch (event->Action) 
				{
					case FILE_ACTION_ADDED: {
						wprintf(L"       Added: %.*s\n", name_len, event->FileName);
						SetIsFolderChanged(true);
					} break;

					case FILE_ACTION_REMOVED: {
						wprintf(L"     Removed: %.*s\n", name_len, event->FileName);
						SetIsFolderChanged(true);
					} break;

					case FILE_ACTION_MODIFIED: {
						wprintf(L"    Modified: %.*s\n", name_len, event->FileName);
						SetIsFolderChanged(true);
					} break;

					case FILE_ACTION_RENAMED_OLD_NAME: {
						wprintf(L"Renamed from: %.*s\n", name_len, event->FileName);
						SetIsFolderChanged(true);
					} break;

					case FILE_ACTION_RENAMED_NEW_NAME: {
						wprintf(L"          to: %.*s\n", name_len, event->FileName);
						SetIsFolderChanged(true);
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
}

void Visualizer::StartUpPage()
{

	ImGui::SetNextWindowSize({ 1280, 800 }, ImGuiCond_Once);
	ImGui::Begin("app", &StartUpPageDraw, ImGuiWindowFlags_NoCollapse);
	{
		SearchFlag();
		

		if (ImGuiHelper::InputTextWithPos("Folder to visualize", &tmpPath, { 200, 85 }, ImGuiInputTextFlags_EnterReturnsTrue) || GetIsFolderChanged())
			SearchFolder();
		
		if (openWindows)
			ShowImage();

		ShowError();

		ImGui::Separator();

		if (dirPathIsCorrect)
			ShowFile();

	}
	ImGui::End();
}

void Visualizer::ShowError()
{
	if (!dirPathIsCorrect)
		ImGuiHelper::TextWithPos("path not found", { 80, 50 }, false);
}

void Visualizer::AddFilesVector(std::filesystem::path path)
{
	if (GetFileType(path.string()) == Image)
	{
		ImageFile imageFile;
		imageFile.path = path;
		imageFile.InitImage();
		Imagefiles.push_back(imageFile);
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

	if (FolderToVisualize != tmpPath || GetIsFolderChanged())
	{
		std::cout << "cambi \n";

		if (GetIsFolderChanged())
			SetIsFolderChanged(false);
		FolderToVisualize = tmpPath;
		std::filesystem::path correct_path = std::filesystem::u8path(FolderToVisualize);

		if (exists_file(correct_path.string()))
		{
			if (realTimeChange)
			{
				threadForCheckDir = std::thread(&Visualizer::CheckFolder, this);
			}

			dirPathIsCorrect = true;

			try
			{
				for (const auto& entry : std::filesystem::directory_iterator(correct_path))
					AddFilesVector(entry);
			}
			catch (const std::exception& e) {
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
		else
		{
			dirPathIsCorrect = false;
		}
	}
}

void Visualizer::ShowFile()
{
	ImGuiStyle& style = ImGui::GetStyle();
	size_t buttons_count = Imagefiles.size();
	float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
	ImVec2 buttonSize = { 80,50 };
	for (int n = 0; n < Imagefiles.size(); n++)
	{
		ImageFile entry = Imagefiles.at(n);

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

void Visualizer::ShowImage()
{
	ImGui::SetNextWindowSize({ ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y}, ImGuiCond_Once);
	ImGui::Begin("visualizer", &openWindows, ImGuiWindowFlags_NoCollapse);
	{
		ImVec2 imageSize(Imagefiles.at(indexImageToDisplay).my_image_width, Imagefiles.at(indexImageToDisplay).my_image_height);
		ImGui::Image(Imagefiles.at(indexImageToDisplay).image, imageSize);
	}
	ImGui::End();
}