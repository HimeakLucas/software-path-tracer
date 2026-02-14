#include "vec3.h"
#include "yaml-cpp/yaml.h"
#include "scene.h"
#include <map>
#include <string>
#include "camera.h"

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

class SceneLoader {
public:
	
	SceneLoader(const std::string& yaml_path) {m_scene_file = YAML::LoadFile(yaml_path);}

	void build_world(scene& world) {
		parse_material();
		parse_spheres(world);
		parse_ambient_light(world);
	};

	void build_camera(camera& cam) {parse_camera(cam);}

private:

	YAML::Node m_scene_file;
	std::map<std::string, material> m_material_lib;

	void parse_material() {

		auto materials_node = m_scene_file["materials"];

		if (materials_node) {
			for (auto it = materials_node.begin(); it != materials_node.end(); it++) {
				std::string name = it->first.as<std::string>();
				YAML::Node data = it->second;
				
				material mat;

				//values in brackets are just fallbacks
				mat.albedo = data["albedo"].as<vec3>(vec3(0.5, 0.5, 0.5));
				mat.smoothness = data["smoothness"].as<double>(0.0);
				mat.specular_probability = data["specular_probability"].as<double>(0.0);

				mat.specular_color = data["specular_color"].as<vec3>(vec3(1.0, 1.0, 1.0));
				mat.emission_strength = data["emission_strength"].as<double>(0.0);
				mat.emission_color = data["emission_color"].as<vec3>(vec3(1.0, 1.0, 1.0));

				m_material_lib[name] = mat;
			}
		}
	}

	void parse_spheres(scene& world) {
		auto world_node = m_scene_file["world"];

		for (auto object: world_node) {
			point3 center = object["center"].as<vec3>();
			double radius = object["radius"].as<double>();

			material mat;
			auto mat_name = object["material"].as<std::string>();
			mat = m_material_lib[mat_name];

			world.spheres.push_back(sphere(center, radius, mat));
		}
	}

	void parse_ambient_light(scene& world) { 
		world.ambient_light_strength = m_scene_file["ambient_light_strength"].as<double>(0.0);
	}

	void parse_camera(camera& cam) {
		auto cam_node = m_scene_file["camera"];

		cam.center = cam_node["center"].as<vec3>(vec3(0, 0, 0));
		cam.vfov = cam_node["fov"].as<double>(90.0);
	}
};


