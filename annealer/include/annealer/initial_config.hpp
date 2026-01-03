#pragma once

struct InitialConfig {
    InitialConfig(const Formula & formula) : rng_start_state(""), initial_temperature(static_cast<double>(formula.most_clauses_per_literal()+1)), cooling_rate(0.95), inner_loop_iterations(1000), min_temperature(1e-3) {}

    std::string rng_start_state;
    double initial_temperature;
    double cooling_rate;
    size_t inner_loop_iterations;
    double min_temperature;
};