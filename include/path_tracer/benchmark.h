#include <chrono>
#include <iomanip>
#include <iostream>
#include <format>
#include <string>

enum class TimeUnit {
	Microseconds,
	Miliseconds,
	Seconds
};

class Timer {

public:
	Timer(TimeUnit output_unit = TimeUnit::Miliseconds) :
		m_start_time_point(std::chrono::high_resolution_clock::now()),
		m_output_unit(output_unit),
		m_stopped(false) 
	{}
	

	~Timer() {
		Stop();
	}

	void Stop() {
		auto end_time_point = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_time_point).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time_point).time_since_epoch().count();

		auto duration_us = end - start;

		switch(m_output_unit) {
			case TimeUnit::Microseconds:
				std::cerr << duration_us << "us\n";
				break;
			case TimeUnit::Miliseconds: {
				float ms = duration_us * 1e-3;
				std::cerr << ms << " ms\n";
				break;
			}
			case TimeUnit::Seconds: {
				float s = duration_us * 1e-6;
				std::cerr << std::setprecision(3) << s << "s\n";
				break;
			}
		}
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time_point;
	bool m_stopped;
	TimeUnit m_output_unit;
};
