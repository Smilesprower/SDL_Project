#include "stdafx.h"
#include "EventHandler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

EventHandler::EventHandler()
	: m_currentCommand(Command::NONE)
	, m_currentEvent(nullptr)
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
}

void EventHandler::HandleEvent(const SceneID::ID sceneID, SDL_Event& e)
{
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
		if (m_currentEvent != nullptr) {
			auto sceneCallbacks = m_callbacks.find(sceneID);
			if (sceneCallbacks != m_callbacks.end()) {
				auto callbackItr = sceneCallbacks->second.find(m_currentCommand);
				if (callbackItr != sceneCallbacks->second.end()) {
					callbackItr->second(m_currentEvent);
				}
			}
		}
		// Reset incase command was found in a different scene
		m_currentEvent = nullptr;
		m_currentCommand = Command::NONE;
	}
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
				m_currentCommand = bindItr.first;
				m_currentEvent = eventInfo;
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
				m_currentCommand = bindItr.first;
				m_currentEvent = eventInfo;
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
				m_currentCommand = bindItr.first;
				m_currentEvent = eventInfo;
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
