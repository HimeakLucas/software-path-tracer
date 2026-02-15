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
		cam.aspect_ratio = 16.0 / 9.0;
		cam.image_width = 400;
		cam.samples_per_pixel = 100;
		cam.max_depth = 50;

	
	} else if (set_camera == "1") {
		cam.aspect_ratio = 16.0 / 9.0;
		cam.image_width = 900;
		cam.samples_per_pixel = 900;
		cam.max_depth = 50;

	} else if (set_camera == "2") {
		cam.aspect_ratio = 16.0 / 9.0;
		cam.image_width = 1080;
		cam.samples_per_pixel = 1500;
		cam.max_depth = 50;


	} else if (set_camera == "3") {
		cam.aspect_ratio = 16.0 / 9.0;
		cam.image_width = 1920;
		cam.samples_per_pixel = 15000;
		cam.max_depth = 50;
	}

	cam.initialize();

	Renderer render;
	{
		Timer timer(TimeUnit::Seconds);
		render.render(world, cam);
	}
}
