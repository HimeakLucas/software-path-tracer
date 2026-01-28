#include "vec3.h"
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include <vector>

int main() {

	sphere ball1(point3(0 , 0, -1.3), 0.5);
	material mat1;
	mat1.albedo = vec3(0.1, 0.2, 0.5);
	mat1.smoothness = 0.0;
	mat1.specular_probability = 0.0;
	ball1.mat = mat1;

	sphere ball2(point3(-1, 0, -1), 0.5);
	material mat2;
	mat2.albedo = vec3(0.8, 0.6, 0.2);
	mat2.emission_color = vec3(0.8, 0.6, 0.2);
	mat2.emission_strength = 1.0;
	ball2.mat = mat2;

	sphere ball3(point3(1 , 0, -1), 0.5);
	material mat3;
	mat3.albedo = vec3(0.3, 0.8, 0.2);
	mat3.smoothness = 0.95;
	mat3.specular_probability = 0.95;
	ball3.mat = mat3;

	sphere earth(point3(0, -100.5, -1), 100);
	material mat4;
	mat4.albedo = vec3(0.8, 0.7, 0.9);
	mat4.smoothness = 0.0;
	mat4.specular_probability = 0.0;
	earth.mat = mat4;

	scene world;
	world.spheres.push_back(ball1);
	world.spheres.push_back(ball2);
	world.spheres.push_back(ball3);
	world.spheres.push_back(earth);

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1920;
	cam.samples_per_pixel = 2000;
	cam.max_depth = 50;
	cam.initialize();


	Renderer render;
	render.render(world, cam);
	
}
