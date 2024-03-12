NAME 		= visualizer

IMGUI_DIR   = ImGui
CC			= clang++ -std=c++20 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/
FLAGS		= -Wall -Wextra #-ftime-trace
RM			= rm -rf

OBJDIR      = .objFiles

FILES		=  main Engine/EngineLinux Visualizer/Visualizer FileHelper/FileHelper FileManager/FileManager ImGuiHelper/ImGuiHelper 
FILES       += $(IMGUI_DIR)/imgui $(IMGUI_DIR)/imgui_demo $(IMGUI_DIR)/imgui_draw $(IMGUI_DIR)/imgui_tables $(IMGUI_DIR)/imgui_widgets $(IMGUI_DIR)/imgui_stdlib $(IMGUI_DIR)/imgui_themes
FILES       += $(IMGUI_DIR)/Backends/imgui_impl_sdl2 $(IMGUI_DIR)/Backends/imgui_impl_opengl3

SRC			= $(FILES:=.cpp)
OBJ			= $(addprefix $(OBJDIR)/, $(FILES:=.o))
HEADER		= stb/stb_image.h Engine/EngineLinux.h $(IMGUI_DIR)/Backends/imgui_impl_sdl2.h $(IMGUI_DIR)/Backends/imgui_impl_opengl3.h $(IMGUI_DIR)/Backends/imgui_impl_opengl3_loader.h Visualizer/Visualizer.h FileHelper/FileHelper.h FileManager/FileManager.h ImGuiHelper/ImGuiHelper.h
UNAME_S     := $(shell uname -s)
LINUX_GL_LIBS = -lGL

FLAGS += -g -Wformat
LIBS = 

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) -ldl `sdl2-config --libs`

	FLAGS += `sdl2-config --cflags`
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	FLAGS += `sdl2-config --cflags`
	FLAGS += -I/usr/local/include -I/opt/local/include
endif

ifeq ($(OS), Windows_NT)
    ECHO_MESSAGE = "MinGW"
    LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

    CXXFLAGS += `pkg-config --cflags sdl2`
endif



.PHONY: all clean fclean re leaks 

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	@echo $(CURSIVE)$(GRAY) "     - Making object files..." $(NONE)
	@echo $(CURSIVE) $(GRAY) "     - Compiling $(NAME)..." $(NONE)
	@$(CC)  $(FLAGS) $(OBJ) $(OPTS) -o $(NAME) $(LIBS)
	@echo $(GREEN)"- Compiled -"$(NONE)

$(OBJDIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) $(OPTS) -c $< -o $@ 

compile_and_run: re
	@./$(NAME)

run:
	@./$(NAME)

leaks:
	@valgrind --suppressions=Valgrid/sup --leak-check=full --show-leak-kinds=all ./$(NAME)

clean:
	@$(RM) $(OBJDIR) $(OBJ)

fclean: clean
	@$(RM) $(NAME)

re: fclean all
