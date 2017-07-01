#include "stdafx.h"
#include "Scene.h"
#include "SceneManager.h"

Scene::Scene(Context& context)
	: m_context(&context)
{
}

Scene::~Scene()
{
}

void Scene::requestClearScene()
{
	m_context->sceneChanges.push_back(Context::SceneChanges{ Actions::CLEAR_SCENE_STACK });
}
void Scene::requestPushScene(SceneID::ID scene)
{
	m_context->sceneChanges.push_back(Context::SceneChanges{ Actions::PUSH_SCENE, scene });
}
void Scene::requestPopScene()
{
	m_context->sceneChanges.push_back(Context::SceneChanges{ Actions::POP_SCENE });
}
