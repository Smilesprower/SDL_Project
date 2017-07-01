#include "stdafx.h"
#include "EventHandler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

EventHandler::EventHandler()
{
	SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
	SDL_JoystickEventState(SDL_ENABLE);

	int numberOfJoysticks = SDL_NumJoysticks();
	for (int i = 0; i < numberOfJoysticks; i++){
		SDL_Joystick * joypad = SDL_JoystickOpen(i);
	}
	loadBindings();
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
			int command, scene, action, key = 0;
			std::vector<std::unique_ptr<EventInfo>> bindings;
			ss >> command;
			while (ss >> scene >> delimTwo >> action >> delimTwo >> key) {
				bindings.emplace_back(std::make_unique<EventInfo>(EventInfo{ scene, action, key }));
			}
			addBinding(static_cast<Command::ID>(command), std::move(bindings));
		}
	}
	file.close();
	return true;
}

bool EventHandler::addBinding(Command::ID name, std::vector<std::unique_ptr<EventInfo>> eventInfo)
{
	if (m_bindings.find(name) != m_bindings.end())
		return false;
	return m_bindings.emplace(name, std::move(eventInfo)).second;
	return false;
}

bool EventHandler::removeBinding(Command::ID commandID)
{
	auto itr = m_bindings.find(commandID);
	if (itr == m_bindings.end()) { 
		return false; 
	}
	m_bindings.erase(itr);
	return true;
}

void EventHandler::HandleEvent(SceneID::ID sceneID, SDL_Event& e)
{
	while (SDL_PollEvent(&e) != 0) {
		for (auto& b_itr : m_bindings) {
			for (auto& e_itr : b_itr.second){
				EventInfo* eventInfo = e_itr.get();
				if (sceneID == eventInfo->m_sceneID) {
					switch (e.type) {
					case SDL_KEYDOWN:
						if (e.type == eventInfo->m_eventType
							&& eventInfo->m_keyCode == e.key.keysym.scancode) {
							m_commands.push_back(b_itr.first);
						}
						break;
					case SDL_JOYBUTTONDOWN:
						if (e.type == eventInfo->m_eventType
							&& eventInfo->m_keyCode == e.jbutton.button) {
							m_commands.push_back(b_itr.first);
						}
						break;
					case SDL_JOYHATMOTION:
						if ( e.type == eventInfo->m_eventType
							&& eventInfo->m_keyCode == e.jhat.value) {
							m_commands.push_back(b_itr.first);
						}
						break;
					}
				}
			}
		}  
	}  

	for (const auto& itr : m_commands) {
		auto stateCallbacks = m_callbacks.find(sceneID);
		auto callItr = stateCallbacks->second.find(itr);
		if (callItr != stateCallbacks->second.end()) {
			callItr->second();
		}
	}
	m_commands.clear();
}
