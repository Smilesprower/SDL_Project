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
	getContext()->eventHandler.addCallback(SceneID::Title, Command::BUTTONA, &TitleScene::add, this);
	getContext()->eventHandler.addCallback(SceneID::Title, Command::BUTTONB, &TitleScene::exit, this);
	std::cout << "Entering Title Scene" << std::endl;
}

void TitleScene::onExit()
{
	getContext()->eventHandler.removeCallback(SceneID::Game);
	std::cout << "Leaving Title Scene" << std::endl;
}

void TitleScene::add(EventInfo* eventInfo)
{
	std::cout << "Hello\n";
	//requestPopScene();
	//requestPushScene(SceneID::Game);
}

void TitleScene::exit(EventInfo* eventInfo)
{
	//std::cout << eventInfo->m_mousePos.x << " " << eventInfo->m_mousePos.y << std::endl;
}
