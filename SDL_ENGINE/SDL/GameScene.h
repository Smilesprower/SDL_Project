#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include <vector>

class GameScene : public Scene
{
public:
	GameScene(Context& context);
	~GameScene();

	void handleEvents(SDL_Event& e);
	void update();
	void render(SDL_Renderer * renderer);
	void onEnter();
	void onExit();

	void add();

private:
};