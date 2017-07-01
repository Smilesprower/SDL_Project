#include "stdafx.h"
#include "Application.h"
#include "TitleScene.h"
#include "GameScene.h"

Application::Application()
	: m_quit(false)
{
}


Application::~Application()
{
}

bool Application::initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
		m_window.reset(SDL_CreateWindow("SDL WINDOW ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800,600, SDL_WINDOW_INPUT_FOCUS));
		m_renderer.reset(SDL_CreateRenderer(m_window.get(), 0, SDL_RENDERER_ACCELERATED));
		SDL_SetRenderDrawColor(m_renderer.get(), 50, 150, 215, 255);
		return true;
	}
	return false;
}

void Application::setUp()
{
	createScenes();
	m_sceneManager.pushScene(SceneID::Title);
}

void Application::cleanUp()
{
}

void Application::loop()
{
	while (!m_quit)
	{
		m_sceneManager.onEvent(m_event);
		m_sceneManager.update();

		SDL_RenderClear(m_renderer.get());
		m_sceneManager.render(m_renderer.get());
		SDL_RenderPresent(m_renderer.get());

		if (m_sceneManager.isSceneStackEmpty()) {
			m_quit = true;
		}
	}
}

void Application::createScenes()
{
	m_sceneManager.createScene<TitleScene>(SceneID::Title);
	m_sceneManager.createScene<GameScene>(SceneID::Game);
}
