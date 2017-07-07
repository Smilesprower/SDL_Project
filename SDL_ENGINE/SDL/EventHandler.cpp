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
}

void EventHandler::handleOneTimeEvent(const SceneID::ID sceneID, SDL_Event& e)
{
	for (auto& bindItr : m_bindings) {
		for (auto& eventItr : bindItr.second) {
			EventInfo* eventInfo = eventItr.get();
				switch (e.type) {
				case SDL_KEYDOWN:
					if (e.key.keysym.scancode == eventInfo->m_keyCode) {
						callback(sceneID, bindItr.first, eventInfo);
					} break;
			}
		}
	}
}

void EventHandler::handleRealTimeEvent(const SceneID::ID sceneID)
{
	SDL_PumpEvents();
	auto keyboardState = SDL_GetKeyboardState(nullptr);

	for (auto& bindItr : m_bindings) {
		for (auto& eventItr : bindItr.second) {
			EventInfo* eventInfo = eventItr.get();
			switch (eventInfo->m_eventType) {
				case RealTimeEvent::KEYBOARD:
					if (keyboardState[eventInfo->m_keyCode]) {
						callback(sceneID, bindItr.first, eventInfo);
					} break;
			}
		}
	}
}

void EventHandler::callback(SceneID::ID sceneID, const Command::ID command, EventInfo * eventInfo)
{
	auto sceneCallbacks = m_callbacks.find(sceneID);
	if (sceneCallbacks != m_callbacks.end()) {
		auto callbackItr = sceneCallbacks->second.find(command);
		if (callbackItr != sceneCallbacks->second.end()) {
			callbackItr->second(eventInfo);
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
