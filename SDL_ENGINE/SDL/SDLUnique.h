#pragma once
#include <SDL.h>
#include <memory>
	struct SDL_Deleter {
		void operator()(SDL_Surface* ptr) { 
			SDL_FreeSurface(ptr); 
		}
		void operator()(SDL_Texture* ptr) { 
			SDL_DestroyTexture(ptr); 
		}
		void operator()(SDL_Renderer* ptr) { 
			SDL_DestroyRenderer(ptr); 
		}
		void operator()(SDL_Window* ptr) { 
			SDL_DestroyWindow(ptr); 
		}
	};
	// Unique Pointers
	using UniqueSDL_Surface = std::unique_ptr<SDL_Surface, SDL_Deleter>;
	using UniqueSDL_Texture = std::unique_ptr<SDL_Texture, SDL_Deleter>;
	using UniqueSDL_Renderer = std::unique_ptr<SDL_Renderer, SDL_Deleter>;
	using UniqueSDL_Window = std::unique_ptr<SDL_Window, SDL_Deleter>;
