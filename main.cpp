#include "vec3.h"
#include "sphere.h"
#include "camera.h"

int main() {

	sphere ball(point3(0, 0, -1), 0.5);
	sphere earth(point3(0, -100.5, -1), 100);

	 
	std::vector<sphere> world {ball, earth};

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1920;
	cam.samples_per_pixel = 200;
	cam.max_depth = 8;

	cam.render(world);
}
