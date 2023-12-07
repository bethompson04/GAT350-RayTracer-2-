#include "Renderer.h"
#include "Ray.h"
#include "Canvas.h"
#include "Random.h"
#include "Camera.h"
#include "Material.h"
#include "Sphere.h"
#include "Scene.h"
#include "Mesh.h"
#include "Plane.h"
#include "Triangle.h"
#include <iostream>

void InitScene01(Scene& scene, const Canvas& canvas);
void InitScene02(Scene& scene, const Canvas& canvas);
void InitSceneCornell(Scene& scene, const Canvas& canvas);

int main(int argc, char* argv[])
{
	const int width = 800;
	const int height = 600;
	const int samples = 10;
	const int depth = 5;

	seedRandom((unsigned int)time(nullptr));

	std::cout << "Hello world!\n";

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("RayTrace", width, height);

	Canvas canvas(width, height, renderer);

	//Scene scene(glm::vec3{ 0 }, glm::vec3{ 0 });
	Scene scene(glm::vec3{ 1.0f }, glm::vec3{ 0.5f, 0.7f, 1.0f }); // sky color could be set with the top and bottom color

	InitSceneCornell(scene, canvas);

	// render scene
	canvas.Clear({ 0, 0, 0, 1 });
	scene.Render(canvas, samples, depth);
	canvas.Update();

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

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

void InitScene01(Scene& scene, const Canvas& canvas)
{

	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 1, 10 }, glm::vec3{ 0, 1, 0 }, glm::vec3{ 0, 1, 0 }, 40.0f, aspectRatio);
	scene.SetCamera(camera);

	// Create material
	auto lambertian = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
	auto metal = std::make_shared<Metal>(color3_t{ 1,1,1 }, 0.0f);


	for (int x = -10; x < 10; x++)
	{
		for (int z = -10; z < 10; z++)
		{

			std::shared_ptr<Material> material;

			// create random material
			float r = random01();
			if (r < 0.3f)		material = std::make_shared<Lambertian>(glm::rgbColor(glm::vec3{ random(0, 360), 1.0f, 1.0f }));
			else if (r < 0.6f)	material = std::make_shared<Metal>(color3_t{ random(0.5f, 1.0f) }, random(0, 0.5f));
			else if (r < 0.9f)	material = std::make_shared<Dielectric>(color3_t{ 1.0f }, random(1.1f, 2));
			else				material = std::make_shared<Emissive>(glm::rgbColor(glm::vec3{ random(0, 360), 1.0f, 1.0f }), 1.5f);

			// set random sphere radius
			float radius = random(0.2f, 0.3f);
			// create sphere using random radius and material
			auto sphere = std::make_unique<Sphere>(glm::vec3{ x + random(-0.5f, 0.5f), radius, z + random(-0.5f, 0.5f) }, radius, material);
			// add sphere to the scene
			scene.AddObject(std::move(sphere));
		}
	}

	auto material = std::make_shared<Lambertian>(color3_t{ 0.4f });
	auto plane = std::make_unique<Plane>(glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 }, material);
	scene.AddObject(std::move(plane));
}

void InitScene02(Scene& scene, const Canvas& canvas)
{
	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 2, 10 }, glm::vec3{ 0, 1, 0 }, glm::vec3{ 0, 1, 0 }, 40.0f, aspectRatio);
	scene.SetCamera(camera);

	// Create material
	//auto metal = std::make_shared<Metal>(color3_t{ 1,1,1 }, 0.0f);
	auto emissive = std::make_shared<Emissive>(color3_t{1,1,1}, 1.5f);


	auto material = std::make_shared<Lambertian>(color3_t{ 0.4f });
	auto plane = std::make_unique<Plane>(glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 }, material);
	scene.AddObject(std::move(plane));

	material = std::make_shared<Lambertian>(color3_t{ 1, 1, 1 });
	auto mesh = std::make_unique<Mesh>(material);
	mesh->Load("Models/cube-1.obj", glm::vec3{ 0, 0.5f, 0 }, glm::vec3{ 0, 45, 0 });
	scene.AddObject(std::move(mesh));
}

void InitSceneCornell(Scene& scene, const Canvas& canvas)
{

	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 2, 10 }, glm::vec3{ 0, 1, 0 }, glm::vec3{ 0, 1, 0 }, 40.0f, aspectRatio);
	scene.SetCamera(camera);

	auto emissive = std::make_shared<Emissive>(color3_t{ 1,1,1 }, 1.5f);
	auto lightSource = std::make_unique<Mesh>(emissive);
	lightSource->Load("Models/cube-1.obj", glm::vec3{ 0,4.5f,0 }, glm::vec3{ 0,0,0 });
	scene.AddObject(std::move(lightSource));

	auto material = std::make_shared<Lambertian>(color3_t{ 1 });
	auto cube = std::make_unique<Mesh>(material);
	cube->Load("Models/cube-1.obj", glm::vec3{ -1,0.5f,-0.5f }, glm::vec3{ 0,45,0 });
	scene.AddObject(std::move(cube));

	auto mirror = std::make_shared<Metal>(color3_t{ 0 }, 0.0f);
	auto sphere = std::make_unique<Sphere>(glm::vec3{ 1,0.5f,0.5f }, 1.0f, mirror);
	scene.AddObject(std::move(sphere));

	auto backWall = std::make_unique<Plane>(glm::vec3{ 0,0,-1 }, glm::vec3{ 0,0,1 }, material);
	scene.AddObject(std::move(backWall));

	auto floor = std::make_unique<Plane>(glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 }, material);
	scene.AddObject(std::move(floor));

	auto roof = std::make_unique<Plane>(glm::vec3{ 0,4,0 }, glm::vec3{ 0,-1,0 }, material);
	scene.AddObject(std::move(roof));

	material = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
	auto leftWall = std::make_unique<Plane>(glm::vec3{ -2.5f,0,0 }, glm::vec3{ 1,0,0 }, material);
	scene.AddObject(std::move(leftWall));

	material = std::make_shared<Lambertian>(color3_t{ 0, 1, 0 });
	auto rightWall = std::make_unique<Plane>(glm::vec3{ 2.5f,0,0 }, glm::vec3{ -1,0,0 }, material);
	scene.AddObject(std::move(rightWall));




}