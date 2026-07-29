#include "path_tracer/scene.h"
#include "path_tracer/camera.h"
#include "path_tracer/renderer.h"
#include "path_tracer/benchmark.h"
#include "path_tracer/scene_loader.h"

int main(int argc, char* argv[]) {

	std::string scene_path = argv[1];

	SceneLoader loader(scene_path);

	scene world;
	loader.build_world(world);


	camera cam;
	loader.build_camera(cam);

	std::string set_camera = argv[2];

	if (set_camera == "0") {
		cam.image_width = 400;
		cam.samples_per_pixel = 100;
		cam.max_depth = 3;

	
	} else if (set_camera == "1") {
		cam.image_width = 900;
		cam.samples_per_pixel = 900;
		cam.max_depth = 8;

	} else if (set_camera == "2") {
		cam.image_width = 1280;
		cam.samples_per_pixel = 1500;
		cam.max_depth = 8;


	} else if (set_camera == "3") {
		cam.image_width = 1920;
		cam.samples_per_pixel = 10000;
		cam.max_depth = 10;
	}

	cam.initialize();

	Renderer render;
	{
		Timer timer(TimeUnit::Seconds);
		render.render(world, cam);
	}
}
