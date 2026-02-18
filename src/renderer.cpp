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

			vec3 bounce_dir;
			if (is_specular_bounce) {
				float fuzz = (1 - rec.mat.smoothness);
				vec3 specular_direction = reflect(bouncing_ray.direction(), rec.normal);
				bounce_dir = specular_direction + fuzz * random_in_unit_sphere();
				
				bouncing_ray.dir = unit_vector(bounce_dir);
				bouncing_ray.orig = rec.hit_point;

				vec3 emitted_light = material.emission_color * material.emission_strength;
				incoming_light += emitted_light * ray_color;
				ray_color *= material.specular_color;


			} else {

				vec3 diffuse_direction = rec.normal + random_unit_vector();
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

	for (const auto& triangle : scene.triangles) {

		hit_record triangle_rec = Renderer::hit_triangle(triangle, r);

		if (triangle_rec.hit_something) {
			triangle_rec.hit_something = true;
			if (triangle_rec.distance < closest_so_far) {
				scene_rec = triangle_rec;
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


Renderer::hit_record Renderer::hit_triangle(const triangle& triangle, const ray& r) {
	hit_record rec;

	point3 v0 = triangle.vertices[0];
	point3 v1 = triangle.vertices[1];
	point3 v2 = triangle.vertices[2];

	vec3 v01 = v1 - v0;
	vec3 v12 = v2 - v1;
	vec3 v20 = v0 - v2;

	vec3 v02 = v2 - v0;

	vec3 normal = unit_vector(cross(v01, v02));

	//check if the ray is parallel to the plane
	double dem = dot(normal, r.direction());
	if (dem == 1e-8) {
		rec.hit_something = false;
		return rec;
	}

	//plane equation: Ax + By + Cz + D = 0
	double D = -dot(normal, v0); //could be any point in the plane instead of v0

	double distance = - (dot(normal, r.origin()) + D) / (dem);

	if(distance < 0.00000001) {
		rec.hit_something = false;
		return rec;
	}

	point3 P = r.at(distance);
	


	vec3 v0P = P - v0;
	if( dot(cross(v01, v0P),  normal) < 0) 	{
		rec.hit_something = false;
		return rec;
	}

	vec3 v1P = P - v1;
	if( dot(cross(v12, v1P),  normal) < 0) 	{
		rec.hit_something = false;
		return rec;
	}

	vec3 v2P = P - v2;
	if( dot(cross(v20, v2P),  normal) < 0) 	{
		rec.hit_something = false;
		return rec;
	}

	rec.hit_something = true;
	rec.distance = distance;
	rec.hit_point = r.at(rec.distance);
	rec.mat = triangle.mat;
	rec.set_face_normal(r, normal);

	return rec;
}
