#pragma once

struct InitialConfig {
    std::string rng_start_state = "";
    double initial_temperature = 1000.0;
    double cooling_rate = 0.95;
    size_t inner_loop_iterations = 1000;
    double min_temperature = 1e-3;
};