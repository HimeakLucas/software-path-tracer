#include "renderer.h"
#include "vec3.h"

void Renderer::render(const scene& scene, const camera& camera) {
	
	float height = camera.image_height;
	float width = camera.image_width;
	float samples_per_pixel = camera.samples_per_pixel;
	float pixel_samples_scale = 1/samples_per_pixel;
	float max_depth = camera.max_depth;

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

	vec3 albedo(0.686, 0.239, 0.851);
	vec3 ray_color(1, 1, 1);
	ray bouncing_ray = r;


	for(int i = 0; i <= depth; i++) {
		hit_record rec = closest_hit(scene, bouncing_ray);
		if(rec.hit_something) {
			vec3 bounce_direction = rec.normal + random_unit_vector();
			bouncing_ray.dir = bounce_direction;
			bouncing_ray.orig = rec.hit_point;
			ray_color *= albedo; //component wise multiplication;
		}
		else {
			vec3 unit_direction = unit_vector(bouncing_ray.direction());
			auto a = 0.5 * (unit_direction.y() + 1.0);
			color sky_color = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); 
			return ray_color * sky_color;
		}
	}

	return color(0, 0, 0);
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
	if (root < 0.001) {
		root = (h + sqrtd) / a;
		if(root < 0.001) {
			return rec;	
		}
	}

	rec.hit_something = true;
	rec.distance = root;
	rec.hit_point = r.at(rec.distance);

	vec3 outward_normal = (rec.hit_point - sphere.center) / sphere.radius;
	rec.set_face_normal(r, outward_normal);

	
	return rec;
}
