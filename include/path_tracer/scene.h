#ifndef SCENE_H
#define SCENE_H

#include "vec3.h"

struct material {
public:
	vec3 albedo{0.5, 0.5, 0.5};
	vec3 specular_color{1, 1, 1};
	vec3 emission_color{0, 0, 0};
	float smoothness = 0;
	float specular_probability = 0;
	float emission_strength = 0;

};

struct sphere{

	sphere(point3 c, double r, material m) : center(c), radius(r), mat(m) {}
	point3 center;
	double radius;
	material mat;
};

struct scene{
	std::vector<sphere> spheres;
	double ambient_light_strength = 0.5;
};

#endif
