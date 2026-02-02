#include "vec3.h"
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


