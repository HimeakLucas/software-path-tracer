#include "path_tracer/scene.h"
#include "path_tracer/camera.h"
#include "path_tracer/renderer.h"
#include "path_tracer/benchmark.h"
#include "path_tracer/scene_loader.h"

int main() {

	SceneLoader loader("demos/emissive_demo.yaml");

	scene world;
	loader.build_world(world);


	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.initialize();

	
	Renderer render;
	{
		Timer timer(TimeUnit::Seconds);
		render.render(world, cam);
	}
	
}
