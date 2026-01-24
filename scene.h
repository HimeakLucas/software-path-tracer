#ifndef SCENE_H
#define SCENE_H

#include "vec3.h"

struct material {
public:
	vec3 albedo{0.5, 0.5, 0.5};
	float roughness = 1.0;
	float metallic = 0.0;
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
