#pragma once

#include <memory>
#include "SDLUnique.h"
#include "SceneManager.h"

class Application
{
public:
	Application();
	~Application();

	bool initialize();
	void setUp();
	void cleanUp();
	void loop();

private:
	void createScenes();

	bool m_quit;
	SDL_Event m_event;
	UniqueSDL_Window m_window;
	UniqueSDL_Renderer m_renderer;
	SceneManager m_sceneManager;
};

