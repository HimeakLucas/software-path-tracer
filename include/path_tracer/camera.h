#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"
#include "utils.h"

class camera  {
public:

	double aspect_ratio= 1.0;
	int image_width = 100; 
	int samples_per_pixel = 10;	
	int max_depth = 10;
	int image_height;

	double vfov= 90;
	point3 look_from = point3(0, 0, 0);
	point3 look_at = point3(0, 0, -1);
	vec3 vup = vec3(0, 1, 0);


	ray get_ray(int i, int j) const {
		auto offset = sample_square();
		auto pixel_sample = pixel_00_loc
				+ ((i + offset.x()) * pixel_delta_u)
				+ ((j + offset.y()) * pixel_delta_v);

		auto ray_origin = center;
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, unit_vector(ray_direction));
	}

	void initialize() {
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_samples_scale = 1.0 / samples_per_pixel;

		center = look_from;
		auto focal_length = (look_from - look_at).length();

		w = unit_vector(look_from - look_at);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		double vfov_radians = utils::degrees_to_radians(vfov);
		auto viewport_height = 2 * std::tan(vfov_radians /2) * focal_length;
		auto viewport_width = viewport_height * (double(image_width)/image_height);

		auto viewport_u = u * viewport_width;
		auto viewport_v = -v * viewport_height;

		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		auto viewport_upper_left = center
					-(w * focal_length)
					-viewport_u/2
					-viewport_v/2;

		pixel_00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}



private:
	point3 pixel_00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	double pixel_samples_scale; //color scale factor for a sum of pixel samples
	vec3 u, v, w;
	point3 center = point3(0, 0, 0);

	vec3 sample_square() const {
		return vec3(utils::random_double() - 0.5, utils::random_double() - 0.5, 0);
	}

};

#endif
