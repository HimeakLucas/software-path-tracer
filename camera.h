#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "interval.h"
#include "utils.h"
#include "sphere.h"

class hit_record {
public:
	point3 p;
	vec3 normal;
	double t;
	bool front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		//assumes outward_normal to bo a unit vector

		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class camera  {
public:

	double aspect_ratio= 1.0;
	int image_width = 100; 
	int samples_per_pixel = 10;	
	int max_depth = 10;

	void render(const std::vector<sphere>& world) {
		initialize();

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; j++) {
			std::clog << "\rScanLines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; i++) {
				color pixel_color(0, 0, 0);

				for (int sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(i, j);
						pixel_color += ray_color(r, max_depth, world);
				}

				write_color(std::cout, pixel_samples_scale * pixel_color);
			}
		}

		std::clog << "\rDone.                                \n";
	}

private:
	int image_height;
	point3 center;
	point3 pixel_00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	double pixel_samples_scale; //color scale factor for a sum of pixel samples
	

	void initialize() {
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_samples_scale = 1.0 / samples_per_pixel;

		center = point3(0, 0, 0);

		auto focal_length = 1.0;
		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * (double(image_width)/image_height);

		auto viewport_u = vec3(viewport_width, 0, 0);
		auto viewport_v = vec3(0, -viewport_height, 0);

		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		auto viewport_upper_left = center
					-vec3(0, 0, focal_length)
					-viewport_u/2
					-viewport_v/2;

		 pixel_00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	ray get_ray(int i, int j) const {
		auto offset = sample_square();
		auto pixel_sample = pixel_00_loc
				+ ((i + offset.x()) * pixel_delta_u)
				+ ((j + offset.y()) * pixel_delta_v);

		auto ray_origin = center;
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		return vec3(utils::random_double() - 0.5, utils::random_double() - 0.5, 0);
	}

	color ray_color(const ray& r, int depth, const std::vector<sphere>& world) const {

		if (depth <= 0)
			return color(0, 0, 0);

		hit_record rec;
		if (scene_hit(world, r, interval(0.001, interval::infinity), rec)) {
			vec3 direction = rec.normal + random_unit_vector(); 
			//actually an aproximation to a cossine weighted distribution
			
			return 0.5 * ray_color(ray(rec.p, direction),depth - 1, world);
		}

		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		auto b = 0.5 * (unit_direction.x() + 1.0);
		color c = (1.0 - b) * color(1, 0.984, 0) + b * color(0.5, 0.7, 1.0);
		return 	(1.0 - a) * color(1.0, 1.0, 1.0) + a * c;
	}

	
	bool sphere_hit(const sphere& sphere, const ray& r, interval ray_t, hit_record& rec) const{
		vec3 oc = sphere.center - r.origin();
		auto a = r.direction().length_squared();
		auto h = dot(r.direction(), oc);
		auto c = dot(oc, oc) - sphere.radius * sphere.radius;
		auto discriminant = h*h - (a * c);

		if (discriminant < 0)
			return false;	

		auto sqrtd = std::sqrt(discriminant);

		auto root = (h - sqrtd) / a;
		if (!ray_t.surrounds(root)) {
			root = (h + sqrtd) / a;
			if (!ray_t.surrounds(root)) {
				return false;
			}
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - sphere.center) / sphere.radius;
		rec.set_face_normal(r, outward_normal);

		return true;
	}	

	bool scene_hit(const std::vector<sphere>& spheres, const ray& r, interval ray_t, hit_record& rec) const {
		hit_record temp_rec;
		bool hit_anything = false;
		auto closest_so_far = ray_t.max;

		for (const auto& sphere : spheres) {
			if (sphere_hit(sphere, r, interval(ray_t.min, closest_so_far), temp_rec)) {
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}	
		}
		return hit_anything;
	}
};

#endif
