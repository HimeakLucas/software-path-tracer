#include "path_tracer/vec3.h"
#include "path_tracer/scene.h"
#include "path_tracer/camera.h"
#include "path_tracer/renderer.h"
#include <vector>
#include "path_tracer/benchmark.h"
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"
#include "path_tracer/yaml_parser.h"

int main() {

	YAML::Node scene_file = YAML::LoadFile("demos/scene1.yaml");

	std::map<std::string ,material> material_lib;
	auto materials_node = scene_file["materials"];

	if (materials_node) {
		for (auto it = materials_node.begin(); it != materials_node.end(); it++) {
			std::string name = it->first.as<std::string>();
			YAML::Node data = it->second;
			
			material mat;
			mat.albedo = data["albedo"].as<vec3>();
			mat.smoothness = data["smoothness"].as<double>();
			mat.specular_probability = data["specular_probability"].as<double>();

			if (data["emission_strength"]) {
				mat.emission_strength = data["emission_strength"].as<double>();
				mat.emission_color = data["emission_color"].as<vec3>();
			}

			material_lib[name] = mat;
		}
	}

	scene world;
	auto world_node = scene_file["world"];
	world.ambient_light_strength = scene_file["ambient_light_strength"].as<double>();


	for (auto object: world_node) {
		point3 center = object["center"].as<vec3>();
		double radius = object["radius"].as<double>();

		material mat;
		auto mat_name = object["material"].as<std::string>();
		mat = material_lib[mat_name];

		world.spheres.push_back(sphere(center, radius, mat));
	}

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1820;
	cam.samples_per_pixel = 2000;
	cam.max_depth = 50;
	cam.initialize();

	
	Renderer render;

	{
		Timer timer(TimeUnit::Seconds);
		render.render(world, cam);
	}
	
}
