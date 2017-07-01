#pragma once
#include <SDL.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <string>
#include <memory>
#include "Identifiers.h"


struct EventInfo {
	int m_sceneID;
	int m_eventType;
	int m_keyCode;
	EventInfo(const int sceneID, const int eventType, const int keyCode)
		: m_sceneID(sceneID)
		, m_eventType(eventType)
		, m_keyCode(keyCode)
	{}
};

using CallbackContainer = std::unordered_map<Command::ID, std::function<void()>>;
using Callbacks = std::unordered_map<SceneID::ID, CallbackContainer>;
using Bindings = std::unordered_map<Command::ID, std::vector<std::unique_ptr<EventInfo>>>;

class EventHandler
{
public:
	EventHandler();
	~EventHandler();

	bool loadBindings();
	bool addBinding(Command::ID name, std::vector<std::unique_ptr<EventInfo>> eventInfo);
	bool removeBinding(Command::ID name);

	void HandleEvent(SceneID::ID sceneID, SDL_Event& e);

	template<class T>
	bool addCallback(const SceneID::ID sceneID, const Command::ID& name, void(T::*func)(), T* instance){
		
		auto itr = m_callbacks.emplace(sceneID, CallbackContainer()).first;
		auto temp = std::bind(func, instance);
		return itr->second.emplace(name, temp).second;
	}
	bool removeCallback(const SceneID::ID sceneID) {
		auto it = m_callbacks.find(sceneID);
		m_callbacks.erase(it);
		return true;
	}

private:
	std::vector<Command::ID> m_commands;
	Bindings m_bindings;
	Callbacks m_callbacks;
};

