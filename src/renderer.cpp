#include "path_tracer/renderer.h"
#include "path_tracer/vec3.h"

void Renderer::render(const scene& scene, const camera& camera) {
	
	int height = camera.image_height;
	int width = camera.image_width;
	int samples_per_pixel = camera.samples_per_pixel;
	float pixel_samples_scale = 1.0/samples_per_pixel;
	int max_depth = camera.max_depth;

	std::cout << "P3\n" << width << ' ' << height << "\n255\n";

	for(int j = 0; j < height; j++) {
		std::clog << "\rScanLines remaining: " << (height - j) << ' ' << std::flush;
		for(int i = 0; i < width; i++){
			color pixel_color(0, 0, 0);
			
			for(int sample = 0; sample < samples_per_pixel; sample++) {
				ray r = camera.get_ray(i, j);
				pixel_color += trace_ray(scene, r, max_depth);
			}

			write_color(std::cout, pixel_samples_scale * pixel_color);
		}
	}
	std::clog << "\rDone                                   \n";
};

color Renderer::trace_ray(const scene& scene, const ray& r, int depth) {

	vec3 ray_color(1, 1, 1);
	vec3 incoming_light(0, 0, 0);
	ray bouncing_ray = r;
	double ambient_light_strength = scene.ambient_light_strength;

	for(int i = 0; i <= depth; i++) {
		hit_record rec = closest_hit(scene, bouncing_ray);
		if(rec.hit_something) {

			material material = rec.mat;

			bool is_specular_bounce = material.specular_probability >= utils::random_double();

			vec3 diffuse_direction = rec.normal + random_unit_vector();

			vec3 bounce_dir;
			if (is_specular_bounce) {

				vec3 specular_direction = reflect(bouncing_ray.direction(), rec.normal);
				bounce_dir = lerp(material.smoothness, diffuse_direction, specular_direction);
				bouncing_ray.dir = unit_vector(bounce_dir);
				bouncing_ray.orig = rec.hit_point;

				vec3 emitted_light = material.emission_color * material.emission_strength;
				incoming_light += emitted_light * ray_color;

				ray_color *= material.specular_color;
			} else {

				bounce_dir = diffuse_direction;
				bouncing_ray.dir = unit_vector(bounce_dir);
				bouncing_ray.orig = rec.hit_point;

				vec3 emitted_light = material.emission_color * material.emission_strength;
				incoming_light += emitted_light * ray_color;

				ray_color *= material.albedo;
			}
		}
		else {
			vec3 unit_direction = unit_vector(bouncing_ray.direction());
			auto a = 0.5 * (unit_direction.y() + 1.0);
			color sky_color = ambient_light_strength * lerp(a, color(0.5, 0.7, 1.0), color(1, 1, 1));
			incoming_light += sky_color * ray_color;
			return incoming_light;
		}
	}

	return incoming_light;
}




Renderer::hit_record Renderer::closest_hit(const scene& scene, const ray& r) {
	hit_record scene_rec;
	scene_rec.hit_something = false;
	auto closest_so_far = interval::infinity;

	for (const auto& sphere : scene.spheres) {

		hit_record sphere_rec = Renderer::hit_sphere(sphere, r);

		if (sphere_rec.hit_something) {
			sphere_rec.hit_something = true;
			if (sphere_rec.distance < closest_so_far) {
				scene_rec = sphere_rec;
				closest_so_far = scene_rec.distance;
			}
		}
	}
	return scene_rec;
};
			
Renderer::hit_record Renderer::hit_sphere(const sphere& sphere, const ray& r) {
	hit_record rec;
	rec.hit_something = false;	

	vec3 oc = sphere.center - r.origin();
	auto a = r.direction().length_squared();
	auto h = dot(r.direction(), oc);
	auto c = dot(oc, oc) - sphere.radius * sphere.radius;
	auto discriminant = h * h - (a * c);

	if (discriminant < 0) {
		return rec;
	}

	auto sqrtd = std::sqrt(discriminant);
	
	auto root = (h - sqrtd) / a;
	if (root < 0.00000001) {
		root = (h + sqrtd) / a;
		if(root < 0.00000001) {
			return rec;	
		}
	}

	rec.hit_something = true;
	rec.distance = root;
	rec.hit_point = r.at(rec.distance);
	rec.mat = sphere.mat;

	vec3 outward_normal = (rec.hit_point - sphere.center) / sphere.radius;
	rec.set_face_normal(r, outward_normal);

	
	return rec;
}
