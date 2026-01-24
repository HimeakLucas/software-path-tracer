#include "vec3.h"
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include <vector>

int main() {

	sphere ball(point3(0, 0, -1), 0.5);
	sphere earth(point3(0, -100.5, -1), 100);

	material mat1;
	mat1.albedo = vec3(0.5, 0.7, 0.9);

	material mat2;
	mat1.albedo = vec3(0.1, 0.9, 0.4);

	ball.mat = mat1;
	earth.mat = mat2;

	scene world;
	world.spheres.push_back(ball);
	world.spheres.push_back(earth);

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.initialize();


	Renderer render;
	render.render(world, cam);
	
}
