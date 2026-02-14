#ifndef UTILS_H
#define UTILS_H

#include <random>
namespace utils {

	const double pi = 3.141592653589793;
	
	inline double random_double() {
		static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		static std::mt19937 generator;
		return distribution(generator);
	}

	inline double random_double(double min, double max) {
		return min + (max - min) * random_double();
	}

	inline double degrees_to_radians(double theta) {
		return (pi * theta) / 180;
	}
}

#endif
