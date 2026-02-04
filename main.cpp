#include "path_tracer/scene.h"
#include "path_tracer/camera.h"
#include "path_tracer/renderer.h"
#include "path_tracer/benchmark.h"
#include "path_tracer/scene_loader.h"

int main(int argc, char* argv[]) {

	SceneLoader loader("demos/emissive_demo.yaml");

	scene world;
	loader.build_world(world);


	camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.focal_length = 1.0;
	cam.center = point3(0, 0, 0.7);

	std::string set_camera = argv[1];

	if (set_camera == "0") {
		cam.aspect_ratio = 16.0 / 9.0;
		cam.image_width = 400;
		cam.samples_per_pixel = 100;
		cam.max_depth = 50;

	
	} else if (set_camera == "1") {
		cam.aspect_ratio = 16.0 / 9.0;
		cam.image_width = 900;
		cam.samples_per_pixel = 600;
		cam.max_depth = 50;

	
	} else if (set_camera == "2") {
		cam.aspect_ratio = 16.0 / 9.0;
		cam.image_width = 1920;
		cam.samples_per_pixel = 5000;
		cam.max_depth = 50;
	}

	cam.initialize();

	Renderer render;
	{
		Timer timer(TimeUnit::Seconds);
		render.render(world, cam);
	}
}
