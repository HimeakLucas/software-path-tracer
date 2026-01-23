#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

struct material {
public:
	vec3 albedo{1.0, 1.0, 1.0};
	float roughness = 1.0;
	float metallic = 0.0;
};

#endif
