#pragma once
#include <iostream>
#include <vector>
#include "SDL.h"
#include "Identifiers.h"
#include "EventHandler.h"

class Scene
{
public:
	struct Context
	{
		struct SceneChanges {
			Actions::ID m_action;
			SceneID::ID m_scene;
		};
		// Content Loader
		EventHandler eventHandler;
		std::vector<SceneChanges> sceneChanges;
	};

	Scene(Context& m_context);
	virtual ~Scene();
	virtual void update() = 0;
	virtual void render(SDL_Renderer * renderer) = 0;

	virtual void onEnter() = 0;
	virtual void onExit() = 0;

protected:
	void requestClearScene();					
	void requestPushScene(SceneID::ID scene);
	void requestPopScene();
	Context * getContext() { return m_context; }
private:
	Context * m_context;
};
