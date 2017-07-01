#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include <vector>

class TitleScene : public Scene
{
public:
	TitleScene(Context& context);
	~TitleScene();

	void update();
	void render(SDL_Renderer * renderer);
	void onEnter();
	void onExit();

	void add();  // TO BE REMOVED
	void exit(); // TO BE REMOVED

private:
};