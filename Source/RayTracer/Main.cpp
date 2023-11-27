#include <iostream>
#include "Renderer.h"

int main(int argc, char* argv[])
{
	std::cout << "Hello world!\n";

	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
	renderer->Initialize();
	renderer->CreateWindow("RayTrace", 400, 300);

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
	}

	renderer->Shutdown();

	return 0;
}