#include "include/annealer/annealer.hpp"
#include "cnf_structs/formula.hpp"
#include "rng_wrapper/rng_wrapper.hpp"


#include <cmath>
#include <numeric>

#include <cassert>
#include <iostream>

Annealer::Annealer(CNFDefine & define, const InitialConfig & config, const Formula & formula) : weights{define.literal_weights}, current_assignment(define.number_of_literals, false), temperature{config.initial_temperature}, cooling_rate{config.cooling_rate}, inner_loop_iterations{config.inner_loop_iterations}, min_temperature(config.min_temperature), rng{} {
    if(!config.rng_start_state.empty()) {
        rng.set_start_state(config.rng_start_state);
    }

    size_t total_weight = std::accumulate(weights.begin(), weights.end(), 0);
    for(const auto & w : weights) {
        normalized_weights.push_back(static_cast<double>(w) / static_cast<double>(total_weight));
    }
    rng.init();
    generate_assignment();

    current_score = evaluate(current_assignment, formula);
    best_assignment = current_assignment;
    best_score = current_score;
}

Annealer& Annealer::set_rng_start_state(std::string& state) {
    rng.set_start_state(state);
    return *this;   
}

void Annealer::save_rng_state(const std::string & filename) {
    rng.save(filename);
}

Annealer& Annealer::set_initial_temperature(double temp) {
    temperature = temp;
    return *this;
}
Annealer& Annealer::set_cooling_rate(double rate) {
    assert(rate > 0.0 && rate < 1.0);
    cooling_rate = rate;
    return *this;
}
Annealer& Annealer::set_inner_loop_iterations(size_t iterations) {
    inner_loop_iterations = iterations;
    return *this;
}
Annealer& Annealer::set_min_temperature(double min_temp) {
    assert(min_temp >= 0.0);
    min_temperature = min_temp;
    return *this;
}

void Annealer::generate_assignment() {
    for(size_t i=0; i<current_assignment.size(); ++i) {
        current_assignment[i] = rng.get_next() > 0.5;
    }
}

Annealer::Assignment Annealer::outer_loop(const Formula & formula) {
    assert(min_temperature < temperature);
    while(temperature > min_temperature) {
        inner_loop(formula);
        cool();
    }
    return best_assignment;
}

void Annealer::inner_loop(const Formula & formula) {
    for(size_t iter=0; iter<inner_loop_iterations; ++iter) {
#if DEBUG || CALIBRATION
        ++cycles;
#endif

        auto neighbour = get_neighbour(rng, current_assignment);
        double neighbour_score = evaluate(neighbour, formula);
        double how_much_worse = current_score - neighbour_score;

        auto change_assignment = [&](){
            current_assignment = neighbour;
            current_score = neighbour_score;

            // Upon changing state cool faster (through shorter inner loop)
            ++iter;
        };

        // The neighbour is actually better than current state
        if(how_much_worse < 0) {
            if(neighbour_score > best_score) {
                best_assignment = neighbour;
                best_score = neighbour_score;
                #if DEBUG || CALIBRATION
                stat.temperature = temperature;
                stat.cycles = cycles;

                if constexpr(DEBUG) {
                    std::cout << "DEBUG: New best found at " << stat.temperature << "° after " << stat.cycles << " cycles with a score of " << best_score << "." << std::endl; 
                }
                #endif
            }
            change_assignment();
        } else {
            // random(0,1) < e^(-δ/T) [δ > 0]
            // δ -> 0 => e^... -> 1 (0/any -> 0)
            // δ -> inf => e^... -> 0 (-inf/any -> -inf)
            // T -> inf => e^... -> 1 (any/inf -> 0)
            // T -> 0 => e^... -> 0 (-any/0 -> -inf)
            double acceptance_prob = std::exp(-how_much_worse / temperature);
            if(rng.get_next() < acceptance_prob) {
                change_assignment();
            }
        }
    }
}

void Annealer::cool() {
    temperature *= cooling_rate;
}

const Annealer::Assignment & Annealer::get_current_assignment() const {
    return current_assignment;
}

Annealer::Assignment Annealer::get_neighbour(RNGWrapper & rng, const Assignment& current_assignment) {
    Assignment neighbour = current_assignment;
    size_t index = rng.get_next_idx(neighbour.size()-1);
    neighbour[index] = !neighbour[index];
    return neighbour;
}

double Annealer::evaluate(const Assignment& assignment, const Formula & formula) const {
    double score = 0.0;
    for(size_t i=0; i<assignment.size(); ++i) {
        if(assignment[i]) {
            score += normalized_weights[i];
        }
    }
    score -= formula.count_unsatisfied_clauses(assignment);
    if constexpr(DEBUG) {
        assert(score <= 1.0 && score >= -static_cast<double>(formula.get_unsatisfied_clauses(assignment).size()));
    }
    return score;
}