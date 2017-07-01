// SDL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define SDL_MAIN_HANDLED
#pragma comment(lib, "SDL2.lib") 

#include "Application.h"


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main(int argc, char** argv)
{
	Application app;

	app.initialize();
	app.setUp();
	app.loop();
	app.cleanUp();

	_CrtDumpMemoryLeaks();
	system("pause");
	return 0;
}
