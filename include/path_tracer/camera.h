#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "interval.h"
#include "utils.h"
#include "scene.h"

class camera  {
public:

	double aspect_ratio= 1.0;
	int image_width = 100; 
	int samples_per_pixel = 10;	
	int max_depth = 10;
	int image_height;

	ray get_ray(int i, int j) const {
		auto offset = sample_square();
		auto pixel_sample = pixel_00_loc
				+ ((i + offset.x()) * pixel_delta_u)
				+ ((j + offset.y()) * pixel_delta_v);

		auto ray_origin = center;
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

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



private:
	point3 center;
	point3 pixel_00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	double pixel_samples_scale; //color scale factor for a sum of pixel samples

	vec3 sample_square() const {
		return vec3(utils::random_double() - 0.5, utils::random_double() - 0.5, 0);
	}

};

#endif
