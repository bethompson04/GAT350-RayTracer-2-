#include "Renderer.h"
#include "Ray.h"
#include "Canvas.h"
#include "Random.h"
#include "Camera.h"
#include "Material.h"
#include "Sphere.h"
#include "Scene.h"
#include <iostream>

int main(int argc, char* argv[])
{
	const int width = 800;
	const int height = 600;

	seedRandom((unsigned int)time(nullptr));

	std::cout << "Hello world!\n";

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("RayTrace", width, height);

	Canvas canvas(width, height, renderer);

	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 0, 1 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, 70.0f, aspectRatio);

	Scene scene(20); // sky color could be set with the top and bottom color
	scene.SetCamera(camera);

	// Create material
	auto lambertian = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
	auto metal = std::make_shared<Metal>(color3_t{ 1,1,1 }, 0.0f);

	// create objects -> add to scene
	
	for (int i = 0; i < 5; i++)
	{
		std::shared_ptr<Material> material = (random(0, 2) == 1) ? std::dynamic_pointer_cast<Material>(lambertian) : std::dynamic_pointer_cast<Material>(metal);

		auto sphere = std::make_unique<Sphere>(glm::vec3{ random(-1,2), random(-1,2), random(-5,-10)}, random01(), material);
		scene.AddObject(std::move(sphere));
	}

	// render scene
	canvas.Clear({ 0, 0, 0, 1 });
	scene.Render(canvas, 20);
	canvas.Update();

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		// dots
		//canvas.Clear({ 0, 0, 0, 1 });
		//for (int i = 0; i < 1000; i++) canvas.DrawPoint({random(0,400), random(0,300)}, {random01(), random01(), random01(), 1});
		//canvas.Update()

		renderer.PresentCanvas(canvas);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
	}

	renderer.Shutdown();

	return 0;
}