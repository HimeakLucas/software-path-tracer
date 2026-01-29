#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "scene.h"
#include "ray.h"
#include "color.h"
#include "interval.h"	

class Renderer {
private:
	struct hit_record {
		bool hit_something;
		vec3 hit_point;
		vec3 normal;
		double distance;
		bool hit_from_outside = true;
		material mat;

		void set_face_normal(const ray& r, const vec3& outward_normal) {
			//assumes outward_normal to be a unit vector
			
			hit_from_outside = dot(r.direction(), outward_normal) < 0;
			normal = hit_from_outside ? outward_normal : -outward_normal; 
			//normal always points against the ray
			//this way we decide de surface side at geometry time
		}

	};

public:

	
	hit_record closest_hit(const scene& scene, const ray& ray);
	color trace_ray(const scene& scene, const ray& ray, int depth);
	hit_record hit_sphere(const sphere& sphere, const ray& ray);

	int m_num_bouces = 10;

	Renderer() = default;
	void render(const scene& scene, const camera& camera);


};


#endif
