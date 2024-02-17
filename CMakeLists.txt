cmake_minimum_required(VERSION 3.27)
project(ImageVisualizer)

set(CMAKE_CXX_STANDARD 17)

include_directories(ImageVisualizer/Src/Engine)
include_directories(ImageVisualizer/Src/FileHelper)
include_directories(ImageVisualizer/Src/FileManager)
include_directories(ImageVisualizer/Src/ImGui)
include_directories(ImageVisualizer/Src/ImGuiHelper)
include_directories(ImageVisualizer/Src/stb)
include_directories(ImageVisualizer/Src/Visualizer)

add_executable(ImageVisualizer
        ImageVisualizer/Src/Engine/Engine.cpp
        ImageVisualizer/Src/Engine/Engine.h
        ImageVisualizer/Src/FileHelper/FileHelper.cpp
        ImageVisualizer/Src/FileHelper/FileHelper.h
        ImageVisualizer/Src/FileManager/FileManager.cpp
        ImageVisualizer/Src/FileManager/FileManager.h
        ImageVisualizer/Src/ImGui/imconfig.h
        ImageVisualizer/Src/ImGui/imgui.cpp
        ImageVisualizer/Src/ImGui/imgui.h
        ImageVisualizer/Src/ImGui/imgui_demo.cpp
        ImageVisualizer/Src/ImGui/imgui_draw.cpp
        ImageVisualizer/Src/ImGui/imgui_impl_dx11.cpp
        ImageVisualizer/Src/ImGui/imgui_impl_dx11.h
        ImageVisualizer/Src/ImGui/imgui_impl_dx9.cpp
        ImageVisualizer/Src/ImGui/imgui_impl_dx9.h
        ImageVisualizer/Src/ImGui/imgui_impl_win32.cpp
        ImageVisualizer/Src/ImGui/imgui_impl_win32.h
        ImageVisualizer/Src/ImGui/imgui_internal.h
        ImageVisualizer/Src/ImGui/imgui_stdlib.cpp
        ImageVisualizer/Src/ImGui/imgui_stdlib.h
        ImageVisualizer/Src/ImGui/imgui_tables.cpp
        ImageVisualizer/Src/ImGui/imgui_themes.cpp
        ImageVisualizer/Src/ImGui/Imgui_themes.h
        ImageVisualizer/Src/ImGui/imgui_widgets.cpp
        ImageVisualizer/Src/ImGui/imstb_rectpack.h
        ImageVisualizer/Src/ImGui/imstb_textedit.h
        ImageVisualizer/Src/ImGui/imstb_truetype.h
        ImageVisualizer/Src/ImGuiHelper/ImGuiHelper.cpp
        ImageVisualizer/Src/ImGuiHelper/ImGuiHelper.h
        ImageVisualizer/Src/stb/stb_image.h
        ImageVisualizer/Src/stb/stb_image_resize2.h
        ImageVisualizer/Src/Visualizer/Visualizer.cpp
        ImageVisualizer/Src/Visualizer/Visualizer.h
        ImageVisualizer/Src/Main.cpp)
