#ifndef utils_h
#define utils_h

#include <cstdint>
#include <functional>
#include <thread>
namespace utils {

	const double pi = 3.141592653589793;

	static uint64_t splitmix64(uint64_t &x) noexcept {
		// https://prng.di.unimi.it/splitmix64.c
		uint64_t z = (x += 0x9e3779b97f4a7c15ull);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
		z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
		return z ^ (z >> 31);
	}
	
	struct xorshiro_engine {
		// xorshiro256+ implementation based on:  https://prng.di.unimi.it/xoshiro256plus.c

		uint64_t s[4]; //256-bit state
		xorshiro_engine (uint64_t seed) {

			std::thread::id  tid = std::this_thread::get_id();
			uint64_t tid_hash = std::hash<std::thread::id>{}(tid);
			uint64_t x = seed ^ (tid_hash + 0x9e3779b97f4a7c15ull);
			
			s[0] = splitmix64(x);
			s[1] = splitmix64(x);
			s[2] = splitmix64(x);
			s[3] = splitmix64(x);
			
		}


		static inline uint64_t rotl(const uint64_t x, int k) {
			return (x << k) | (x >> (64 - k));
		}

		inline uint64_t next() {
			const uint64_t result = s[0] + s[3];
			
			const uint64_t t = s[1] << 17;

			s[2] ^= s[0];
			s[3] ^= s[1];
			s[1] ^= s[2];
			s[0] ^= s[3];

			s[2] ^= t;
			
			s[3] = rotl(s[3], 45);

			return result;
		}
	};



	inline double u64_to_unit_double(uint64_t r) {
		const uint64_t top53 = r >> 11; //61 - 1  = 53 bits (size of the mantissa of a double)
		
		const double inv = 1.0 / 9007199254740992.0; // 1 / 2^53
		return static_cast<double>(top53) * inv;
	}

	//The object is created only once per thread. It works like a "global private" exclusive to the thread
	inline xorshiro_engine &xorshiro() {
		uint64_t seed = 62540987146053798ULL; //could be anything
		thread_local xorshiro_engine eng(seed); //definetly not a good aproach if eventually this code goes to GPU
		//if this eventualy goes to the gpu, we can seed based on the pixel coodinate
		return eng;
	}

	inline double random_double() {
		uint64_t r = xorshiro().next();
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
