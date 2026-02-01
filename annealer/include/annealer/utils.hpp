#pragma once

struct InitialConfig {
    double initial_temperature;
    size_t inner_loop_iterations;
    double min_temperature;
    double cooling_rate;
};

struct Statistic {
    double temperature;
    size_t cycles;
};