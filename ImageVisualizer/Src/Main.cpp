#define _WIN32_WINNT 0x0601
#pragma comment(lib, "crypt32")
#pragma comment(lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#define _HAS_STD_BYTE 0
#include "Engine/Engine.h"

/// <summary>
///  function to attach a console to the programm (for debug purpose)
/// </summary>
void CreateConsole()
{
    FILE* fDummy;
    AllocConsole();
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    CreateConsole();

    Engine::InitEngine();
    Engine::Render();
    Engine::CleanUp();
}

