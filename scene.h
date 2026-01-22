#ifndef SCENE_H
#define SCENE_H

#include "vec3.h"

struct sphere{

	sphere(point3 c, double r) : center(c), radius(r) {}
	point3 center;
	double radius;
};

struct scene{
	std::vector<sphere> spheres;
};

#endif
