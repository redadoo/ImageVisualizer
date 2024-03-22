#ifndef ENGINE_H
#define ENGINE_H

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
	void				RunEngine();
	bool				done;
	SDL_Window*			window;
	SDL_WindowFlags 	windowFlags;
	ImGuiConfigFlags   	ConfFlags;
	SDL_GLContext		glContext;

private:
	void Render();
	void InitEngine();
	void CleanUp();
	void ShouldQuit();

};

#endif // ENGINE_H