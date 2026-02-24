#ifndef utils_h
#define utils_h

#include <cstdint>
#include <functional>
#include <thread>
namespace utils {

	const double pi = 3.141592653589793;

	static uint64_t splitmix64(uint64_t &x) noexcept {
		uint64_t z = (x += 0x9e3779b97f4a7c15ull);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
		z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
		return z ^ (z >> 31);
	}

	struct pcg_engine {
		uint64_t state;
		uint64_t inc;

		pcg_engine(uint64_t seed) {
			std::thread::id  tid = std::this_thread::get_id();
			uint64_t tid_hash = std::hash<std::thread::id>{}(tid);
			uint64_t a = tid_hash * 123456789ull;
			uint64_t b = tid_hash * 987654321ull;	
			state = splitmix64(a);
			inc = splitmix64(b);
		
			//warm-up
			pcg32();
			pcg32();
		}

		// based on the minimum c implementation of: https://www.pcg-random.org/download.html
		inline uint32_t pcg32()
		{
		    uint64_t oldstate = state;
		    // advance internal state
		    state = oldstate * 6364136223846793005ull + (inc|1);
		    // calculate output function (xsh rr), uses old state for max ilp
		    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
		    uint32_t rot = oldstate >> 59u;
		    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
		}

		inline uint64_t pcg64() {
			uint64_t upper = static_cast<uint64_t>(pcg32());
			uint64_t lower = static_cast<uint64_t>(pcg32());
			return (upper << 32) | lower;
		}


	};


	inline double u64_to_unit_double(uint64_t r) {
		const uint64_t top53 = r >> 11; //61 - 1  = 53 bits (size of the mantissa of a double)
		
		const double inv = 1.0 / 9007199254740992.0; // 1 / 2^53
		return static_cast<double>(top53) * inv;
	}

	inline pcg_engine &engine() {
		thread_local pcg_engine eng(234085976428ull);
		return eng;
	}
	
	// inline double random_double() {
	// 	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	// 	static std::mt19937 generator;
	// 	return distribution(generator);
	// }

	inline double random_double() {
		uint64_t r = engine().pcg64();
		return u64_to_unit_double(r);
	}

	inline double random_double(double min, double max) {
		return min + (max - min) * random_double();
	}

	inline double degrees_to_radians(double theta) {
		return (pi * theta) / 180;
	}
}

#endif
