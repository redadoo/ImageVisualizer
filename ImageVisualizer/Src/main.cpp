#ifdef __linux__

#include "Engine/EngineLinux.h"

int main()
{
	Engine engine = Engine();
	engine.InitEngine();
    engine.Render();
    engine.CleanUp();
}

#elif _WIN32

#include "Engine/EngineWin32.h"
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

#endif 
