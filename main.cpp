#include "path_tracer/vec3.h"
#include "path_tracer/scene.h"
#include "path_tracer/camera.h"
#include "path_tracer/renderer.h"
#include <vector>
#include "path_tracer/benchmark.h"
#include "yaml-cpp/node/parse.h"
#include "yaml-cpp/yaml.h"

//https://github.com/jbeder/yaml-cpp/wiki/Tutorial
namespace YAML {
template<>
struct convert<vec3>{
	static Node encode(const vec3& rhs) {
		Node node;
		node.push_back(rhs.x());
		node.push_back(rhs.y());
		node.push_back(rhs.z());

		return node;	
	}
	
	static bool decode(const Node& node, vec3& rhs) {
		if(!node.IsSequence() || node.size() != 3) {
			return false;
		}

		rhs.e[0] = node[0].as<double>();
		rhs.e[1] = node[1].as<double>();
		rhs.e[2] = node[2].as<double>();
		return true;
	} 
};

}

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

			material_lib[name] = mat;
		}
	}

	scene world;
	auto world_node = scene_file["world"];

	point3 center = world_node[0]["center"].as<vec3>();
	double radius = world_node[0]["radius"].as<double>();

	material mat;
	auto mat_name = world_node[0]["material"].as<std::string>();
	mat = material_lib[mat_name];

	world.spheres.push_back(sphere(center, radius, mat));

	// if (world_node && world_node.IsSequence()) {
	// 	for (auto item : world_node) {
	// 		if (item["type"].as<std::string>() == "sphere") {
	// 			point3 center = item["center"].as<vec3>();
	// 			double radius
	// 		}
	// 	}
	// }

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 500;
	cam.max_depth = 50;
	cam.initialize();

	
	Renderer render;

	{
		Timer timer(TimeUnit::Seconds);
		render.render(world, cam);
	}
	
}
