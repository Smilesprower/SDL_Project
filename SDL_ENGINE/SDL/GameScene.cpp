#include "stdafx.h"
#include "GameScene.h"


GameScene::GameScene(Context& context)
	: Scene(context)
{
}

GameScene::~GameScene()
{
}

void GameScene::handleEvents(SDL_Event& e)
{
}

void GameScene::update()
{
}

void GameScene::render(SDL_Renderer* renderer)
{
}

void GameScene::onEnter()
{
	getContext()->eventHandler.addCallback(SceneID::Game, Command::JUMP, &GameScene::add, this);
	getContext()->eventHandler.addCallback(SceneID::Game, Command::QUIT, &GameScene::exit, this);
	std::cout << "Entering Game Scene" << std::endl;
}

void GameScene::onExit()
{
	getContext()->eventHandler.removeCallback(SceneID::Game);
	std::cout << "Leaving Game Scene" << std::endl;
}

void GameScene::add()
{
	requestPopScene();
	requestPushScene(SceneID::Title);
}

void GameScene::exit()
{
	requestClearScene();
}
