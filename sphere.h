#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "interval.h"

struct sphere{

	sphere(point3 c, double r) : center(c), radius(r) {}


	point3 center;
	double radius;
};


#endif
