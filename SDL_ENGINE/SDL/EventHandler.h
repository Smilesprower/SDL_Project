#pragma once
#include <SDL.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>
#include "Identifiers.h"

namespace RealTimeEvent
{
	enum ID {
		KEYBOARD = SDL_USEREVENT + 1,
		MOUSE,
		JOYSTICKBUTTONS,
		JOYSTICKJHAT,
		JOYSTICKANALOG
	};
}

struct EventInfo {
	int m_eventType;
	int m_keyCode;
	SDL_Point m_mousePos{ 0,0 };
	EventInfo(const int eventType, const int keyCode)
		: m_eventType(eventType)
		, m_keyCode(keyCode)
	{}
};

using CallbackContainer = std::unordered_map<Command::ID, std::function<void(EventInfo*)>>;
using Callbacks = std::unordered_map<SceneID::ID, CallbackContainer>;
using Bindings = std::unordered_map<Command::ID, std::vector<std::unique_ptr<EventInfo>>>;

class EventHandler
{
public:
	EventHandler();
	~EventHandler();

	bool loadBindings();
	bool addBinding(const Command::ID name, std::vector<std::unique_ptr<EventInfo>> eventInfo);
	bool removeCallback(const SceneID::ID sceneID);
	void handleOneTimeEvent(const SceneID::ID sceneID, SDL_Event& e);
	void handleRealTimeEvent(const SceneID::ID sceneID);

	template<class T>
	bool addCallback(const SceneID::ID sceneID, const Command::ID& name, void(T::*func)(EventInfo*), T* instance) {
		auto itr = m_callbacks.emplace(sceneID, CallbackContainer()).first;
		auto temp = std::bind(func, instance, std::placeholders::_1);
		return itr->second.emplace(name, temp).second;
	}


private:
	void callback(SceneID::ID sceneID, const Command::ID command, EventInfo * eventInfo);

	Bindings m_bindings;
	Callbacks m_callbacks;
};

