#pragma once
#include <unordered_map>
#include <functional>
#include <vector>
#include "SDLUnique.h"
#include "Identifiers.h"

namespace RealTimeEvent
{
	enum ID {
		JOYBUTTONUP,
		JOYBUTTONDOWN,
		JOYDPAD,
		JOYANALOG,
		JOYBUTTONONCE,
		JOYBUTTONREPEAT,
	};
}

struct JoyStickButtons{
	//Exapand if needed.
	std::vector<Uint8> m_buttons{ 16,0 };
};

struct EventInfo {
	int m_eventType;
	int m_keyCode;
	SDL_Point m_mousePos{ 0,0 };
	EventInfo(const int eventType, const int keyCode)
		: m_eventType(eventType)
		, m_keyCode(keyCode)
	{}
};
using JoySticks = std::unordered_map<Uint8, std::pair<UniqueSDL_JoyStick, JoyStickButtons>>;
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
	JoySticks m_joySticks;
	Bindings m_bindings;
	Callbacks m_callbacks;
};

