#pragma once
#include <SDL.h>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include "Scene.h"
#include "Identifiers.h"


class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void onEvent(SDL_Event& e);
	void update();											// Update the current scene
	void render(SDL_Renderer * renderer);					// Render all scenes on the stack

	void pushScene(SceneID::ID scene);						// Push scene
	void popScene();				
	void clearSceneStack();
	bool isSceneStackEmpty();								// Size of Scene Stack

	template <typename T>
	void createScene(SceneID::ID scene);

private:
	std::vector<SceneID::ID> m_currentScenes;
	Scene::Context m_context;
	std::vector<std::unique_ptr<Scene>> m_sceneStack;	
	std::map<SceneID::ID, std::function<std::unique_ptr<Scene>()>> m_sceneFactory;	

	void applyAwaitingChanges();
	std::unique_ptr<Scene> addScene(SceneID::ID scene);

	SceneManager(const SceneManager& other) = delete;				
	SceneManager& operator=(const SceneManager&) = delete;			
};

template <typename T>
void SceneManager::createScene(SceneID::ID scene) {
	m_sceneFactory[scene] = [this]() {
		return std::unique_ptr<Scene>(new T(m_context));
	};
}
