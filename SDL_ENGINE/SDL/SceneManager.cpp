#include "stdafx.h"
#include "SceneManager.h"


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}

void SceneManager::onEvent(SDL_Event & e)
{
	m_context.eventHandler.HandleEvent(m_currentScenes.back(), e);
}
void SceneManager::update()
{
	if (!m_sceneStack.empty()) {
		m_sceneStack.back()->update();
	}
	applyAwaitingChanges();
}
void SceneManager::render(SDL_Renderer * renderer)
{
	if (!m_sceneStack.empty()) {
		for (const auto& itr : m_sceneStack) {
			itr->render(renderer);
		}
	}
}

void SceneManager::pushScene(SceneID::ID scene)
{
	m_currentScenes.push_back(scene);
	m_sceneStack.push_back(addScene(scene));
	m_sceneStack.back()->onEnter();
}
void SceneManager::popScene()
{
	m_currentScenes.pop_back();
	m_sceneStack.back()->onExit();
	m_sceneStack.pop_back();
}
void SceneManager::clearSceneStack()
{
	for (const auto & itr : m_sceneStack){
		itr->onExit();
	}
	m_currentScenes.clear();
	m_sceneStack.clear();
}
bool SceneManager::isSceneStackEmpty()
{
	return (m_sceneStack.empty()) ? true : false;
}

void SceneManager::applyAwaitingChanges()
{
	for (auto const &itr : m_context.sceneChanges)
	{
		switch (itr.m_action)
		{
		case Actions::PUSH_SCENE:
			pushScene(itr.m_scene);
			break;

			case Actions::POP_SCENE:
			popScene();
			break;

		case Actions::CLEAR_SCENE_STACK:
			clearSceneStack();
			break;
		}
	}
	m_context.sceneChanges.clear();
}

std::unique_ptr<Scene> SceneManager::addScene(SceneID::ID scene)
{
	auto found = m_sceneFactory.find(scene);
	if (found == m_sceneFactory.end()) {
		return nullptr;
	}
	return found->second();
}
