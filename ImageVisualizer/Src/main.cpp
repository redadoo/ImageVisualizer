#ifdef __linux__
	#include "Engine/EngineLinux.h"
#elif _WIN32
	#include "Engine/EngineWin32.h"
#endif 

int main()
{
 	Engine engine = Engine();
	engine.RunEngine();
    

	return 0;
}