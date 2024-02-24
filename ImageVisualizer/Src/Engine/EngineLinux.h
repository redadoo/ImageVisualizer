#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_sdl2.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif


class Engine
{
public:
	static void InitValues();
	static void Render();
	static void InitEngine();
	static void CleanUp();
	static bool ShouldQuit();

private:

};
