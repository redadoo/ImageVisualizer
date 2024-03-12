
#ifndef ENGINE_H
#define ENGINE_H

#include <Winsock2.h>
#include <Windows.h>
#include <tchar.h>
#include "d3d11.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui.h"
#include "../Visualizer/Visualizer.h"
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// The EngineGui contains the backend (dx11) and initialize the gui
/// </summary>
class Engine
{
private:
	static ID3D11DeviceContext* pd3dDeviceContext;
	static IDXGISwapChain* pSwapChain;
	static ID3D11RenderTargetView* pMainRenderTargetView;

	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	static ID3D11Device* pd3dDevice;
	inline static ImGuiConfigFlags   ConfFlags;
	inline static HWND hwnd;
	inline static WNDCLASSEX wc;
	static HMODULE hCurrentModule;
	inline static bool bDone = false;
	inline static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	static void InitValues();
	static void Render();
	static void InitEngine();
	static void CleanUp();
	static bool ShouldQuit();
};

#endif