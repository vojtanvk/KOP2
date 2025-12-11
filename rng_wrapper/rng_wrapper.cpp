#include "include/rng_wrapper/rng_wrapper.hpp"

#include "rng/xoshiro256plus.h"
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

void RNGWrapper::init() {

	// TODO make this optional and configurable
	set_seed("rng_wrapper/config/seed");
	set_start_state("rng_wrapper/config/rng_state");

	// prefer explicit state if provided, else seed
	if (state_given) {
		rng_set_state_uint(s1, s2, s3, s4);
	} else if (seed_given) {
		rng_set_seed(seed);
	} else {
		// default seed used by rngctrl
		rng_set_seed(0x55AA55AA55AA55AAULL);
	}
}

void RNGWrapper::set_seed(const std::string & in_seed) {
	// try to interpret in_seed as: "time", hex serial of state, filename or numeric seed
	if (in_seed == "time") {
		// use current time via rngctrl-style: set_seed with time-like value
		// there's no direct helper, but xoshiro splitmix accepts any 64-bit
		// use a simple time-based seed
		uint64_t t = (uint64_t)time(nullptr);
		seed = t;
		seed_given = true;
		return;
	}

	// numeric literal?
	char* endptr = nullptr;
	unsigned long long val = strtoull(in_seed.c_str(), &endptr, 0);
	if (endptr != in_seed.c_str() && *endptr == '\0') {
		seed = (uint64_t)val;
		seed_given = true;
		return;
	}

	// otherwise treat as filename containing seed
	std::ifstream in(in_seed);
	if (!in) {
		throw std::runtime_error("set_seed: input is not 'time', number, or readable file");
	}
	// try to read seed from file
	uint64_t file_seed = 0;
	if (!(in >> file_seed)) {
		throw std::runtime_error("set_seed: failed to parse seed from file");
	}
	seed = file_seed;
	seed_given = true;
}

void RNGWrapper::set_start_state(const std::string & filename) {
	// try to deserialize state from string directly first
	rng_state_t st;
	char buf[128];
	if (filename.size() < sizeof(buf)) {
		strncpy(buf, filename.c_str(), sizeof(buf));
		buf[sizeof(buf)-1] = '\0';
		if (rng_state_deseri(buf, &st)) {
			s1 = st.s[0]; s2 = st.s[1]; s3 = st.s[2]; s4 = st.s[3];
			state_given = true;
			rng_set_state(&st);
			return;
		}
	}

	// otherwise treat as filename and attempt to read file
	std::ifstream in(filename);
	if (!in) throw std::runtime_error("set_start_state: cannot open file");
	std::string line;
	std::getline(in, line);
	if (!rng_state_deseri((char*)line.c_str(), &st)) {
		throw std::runtime_error("set_start_state: invalid state in file");
	}
	s1 = st.s[0]; s2 = st.s[1]; s3 = st.s[2]; s4 = st.s[3];
	state_given = true;
	rng_set_state(&st);
}

void RNGWrapper::set_start_state(uint64_t in_s1, uint64_t in_s2, uint64_t in_s3, uint64_t in_s4) {
	s1 = in_s1; s2 = in_s2; s3 = in_s3; s4 = in_s4;
	state_given = true;
}

void RNGWrapper::save(const std::string & filename) const {
	rng_state_t st;
	rng_get_state(&st);
	std::ofstream out(filename);
	if (!out) throw std::runtime_error("save: cannot open file for writing");
	char buf[RNG_STATE_CHARS+4];
	if (!rng_state_seri(buf, &st)) {
		throw std::runtime_error("save: failed to serialize state");
	}
	out << buf << "\n";
	// std::cout << "State saved " << filename << "\n";
}

float RNGWrapper::get_next() {
	double d = rng_next_double();
	return static_cast<float>(d);
}