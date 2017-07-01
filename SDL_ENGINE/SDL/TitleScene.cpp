#include "stdafx.h"
#include "TitleScene.h"
#include "EventHandler.h"

TitleScene::TitleScene(Context& context)
	: Scene(context)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::update()
{
}

void TitleScene::render(SDL_Renderer* renderer)
{
}

void TitleScene::onEnter()
{
	getContext()->eventHandler.addCallback(SceneID::Title, Command::JUMP, &TitleScene::add, this);
	std::cout << "Entering Title Scene" << std::endl;
}

void TitleScene::onExit()
{
	getContext()->eventHandler.removeCallback(SceneID::Title);
	std::cout << "Leaving Title Scene" << std::endl;
}

void TitleScene::add()
{
	requestPopScene();
	requestPushScene(SceneID::Game);
}