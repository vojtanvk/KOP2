#pragma once

#include <cnf_structs/formula.hpp>
#include <rng_wrapper/rng_wrapper.hpp>
#include <cnf_parser/cnf_define.hpp>
#include "initial_config.hpp"

#include <vector>

class Annealer {
public:
    using Assignment = std::vector<bool>;

    Annealer(CNFDefine & define, const InitialConfig & config);

    Annealer& set_rng_start_state(std::string & state);
    void save_rng_state(const std::string & filename);
    Annealer& set_initial_temperature(double temp);
    Annealer& set_cooling_rate(double rate);
    Annealer& set_inner_loop_iterations(size_t iterations);
    Annealer& set_min_temperature(double min_temp);

    const Assignment & get_current_assignment() const;

    Annealer::Assignment outer_loop(const Formula & formula);

private:
    std::vector<size_t> weights;

    std::vector<bool> current_assignment;
    double current_score;
    
    double temperature;
    // temperature change coefficient after each outer loop iteration
    double cooling_rate;
    size_t inner_loop_iterations;
    double min_temperature;

    RNGWrapper rng;

    std::vector<double> normalized_weights;
    std::vector<bool> best_assignment;
    double best_score;

    void inner_loop(const Formula & formula);

    void cool();

    Assignment get_neighbour(RNGWrapper & rng, const Assignment& current_assignment);
    double evaluate(const Assignment& assignment, const Formula & formula) const;
    void generate_assignment();
};
