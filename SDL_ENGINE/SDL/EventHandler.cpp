#include "stdafx.h"
#include "EventHandler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

EventHandler::EventHandler()
{
	// Move to init function
	// <
	SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
	SDL_JoystickEventState(SDL_ENABLE);

	int numberOfJoysticks = SDL_NumJoysticks();
	for (int i = 0; i < numberOfJoysticks; i++){
		SDL_Joystick * joypad = SDL_JoystickOpen(i);
	}
	loadBindings();
	//>
}

EventHandler::~EventHandler()
{
	m_bindings.clear();
	m_callbacks.clear();
}

bool EventHandler::loadBindings()
{
	std::ifstream file("../bindings.cfg"); //Do a check on this if it fails exist
	if (!file.is_open()) { std::cout << "! Failed loading bindings.cfg." << std::endl; return false; }
	char delimOne = '#';
	char delimTwo = ':';
	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] != delimOne) {
			std::stringstream ss(line);
			int command, action, key = 0;
			std::vector<std::unique_ptr<EventInfo>> bindings;
			ss >> command;
			while (ss >> action >> delimTwo >> key) {
				bindings.emplace_back(std::make_unique<EventInfo>(EventInfo{ action, key }));
			}
			addBinding(static_cast<Command::ID>(command), std::move(bindings));
		}
	}
	file.close();
	return true;
}

bool EventHandler::addBinding(const Command::ID name, std::vector<std::unique_ptr<EventInfo>> eventInfo)
{
	if (m_bindings.find(name) != m_bindings.end())
		return false;
	return m_bindings.emplace(name, std::move(eventInfo)).second;
	return false;
}

void EventHandler::removeBinding()
{
	m_bindings.clear();
}

void EventHandler::HandleEvent(const SceneID::ID sceneID, SDL_Event& e)
{
	// This will not work change back to original idea;

	EventInfo * eventInfo = nullptr;

	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
		case SDL_KEYDOWN:
			handleKeyDownEvent(e);
			break;
		case SDL_KEYUP:
			handleKeyUpEvent(e);
			break;
		case SDL_JOYHATMOTION:
			handleJoyHatMotionEvent(e);
			break;
		case SDL_JOYBUTTONDOWN:
			handleJoyButtonDownEvent(e);
			break;
		case SDL_JOYBUTTONUP:
			handleJoyButtonUpEvent(e);
			break;
		case SDL_MOUSEMOTION:
			handleMouseMotionEvent(e);
			break;
		case SDL_MOUSEBUTTONDOWN:
			handleMouseButtonDownEvent(e);
			break;
		case SDL_MOUSEBUTTONUP:
			handleMouseButtonUpEvent(e);
			break;
		case SDL_QUIT:
			handleSDLQuitEvent(e);
			break;
		}
	}  
	// Check to see it you are in the correct scene before executing commands
	for (const auto& itr : m_commands) {
		auto sceneCallbacks = m_callbacks.find(sceneID);
		if (sceneCallbacks != m_callbacks.end()) {
			auto callbackItr = sceneCallbacks->second.find(itr.first);
			if (callbackItr != sceneCallbacks->second.end()) {
				callbackItr->second(itr.second);
			}
		}
		//auto defaultCallbacks = m_callbacks.find(SceneID::None);
		//if (defaultCallbacks != m_callbacks.end()) {
		//	auto callbackItr = defaultCallbacks->second.find(itr.first);
		//	if (callbackItr != defaultCallbacks->second.end()) {
		//		callbackItr->second(itr.second);
		//	}
		//}
	}

	m_commands.clear();
}

void EventHandler::handleKeyDownEvent(SDL_Event & e)
{

}

void EventHandler::handleKeyUpEvent(SDL_Event & e)
{

}

void EventHandler::handleJoyButtonUpEvent(SDL_Event & e)
{

}

void EventHandler::handleJoyHatMotionEvent(SDL_Event & e)
{

}

void EventHandler::handleJoyButtonDownEvent(SDL_Event & e)
{
	for (auto& bindItr : m_bindings) {
		for (auto& eventItr : bindItr.second) {
			EventInfo* eventInfo = eventItr.get();
			if (e.type == eventInfo->m_eventType && eventInfo->m_keyCode == e.jbutton.button) {
				m_commands.emplace_back(std::make_pair(bindItr.first, eventInfo));
			}
		}
	}
}

void EventHandler::handleMouseMotionEvent(SDL_Event & e)
{
	for (auto& bindItr : m_bindings) {
		for (auto& eventItr : bindItr.second) {
			EventInfo* eventInfo = eventItr.get();
			if (e.type == eventInfo->m_eventType) {
				eventInfo->m_mousePos.x = e.motion.x;
				eventInfo->m_mousePos.y = e.motion.y;
				m_commands.emplace_back(std::make_pair(bindItr.first, eventInfo));
			}
		}
	}
}

void EventHandler::handleMouseButtonDownEvent(SDL_Event & e)
{

}

void EventHandler::handleMouseButtonUpEvent(SDL_Event & e)
{

}

void EventHandler::handleSDLQuitEvent(SDL_Event & e)
{
	for (auto& bindItr : m_bindings) {
		for (auto& eventItr : bindItr.second) {
			EventInfo* eventInfo = eventItr.get();
			if (e.type == eventInfo->m_eventType) {
				m_commands.emplace_back(std::make_pair(bindItr.first, eventInfo));
			}
		}
	}
}

bool EventHandler::removeCallback(const SceneID::ID sceneID)
{
	auto it = m_callbacks.find(sceneID);
	if (it != m_callbacks.end()) {
		m_callbacks.erase(it);
	}
	// else callback container doesn't have that scene
	return true;
}
