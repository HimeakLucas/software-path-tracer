#ifndef SCENE_H
#define SCENE_H

#include "vec3.h"

struct material {
public:
	vec3 albedo{0.5, 0.5, 0.5};
	float smoothness;
	float specular_probability;
};

struct sphere{

	sphere(point3 c, double r) : center(c), radius(r) {}
	point3 center;
	double radius;
	material mat;
};

struct scene{
	std::vector<sphere> spheres;
};

#endif
