#pragma once

#include "../ImGui/imgui.h"
#include "../ImGui/Backends/imgui_impl_sdl2.h"
#include "../ImGui/Backends/imgui_impl_opengl3.h"
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
	void Render();
	void InitEngine();
	void CleanUp();
	bool ShouldQuit();

private:
	SDL_Window*		window;
	SDL_WindowFlags	window_flags;
	ImGuiIO io;
	SDL_GLContext gl_context;
};
