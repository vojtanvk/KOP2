#include "include/annealer/annealer.hpp"
#include "cnf_structs/formula.hpp"
#include "rng_wrapper/rng_wrapper.hpp"


#include <cmath>
#include <numeric>

#include <cassert>

Annealer::Annealer(CNFDefine & define) : weights{define.literal_weights}, current_assignment(define.number_of_literals, false), current_score{0.0}, temperature{1000.0}, cooling_rate{0.99}, inner_loop_iterations{100}, min_temperature(1.0), rng{}, best_score(0.0) {
    size_t total_weight = std::accumulate(weights.begin(), weights.end(), 0);
    for(const auto & w : weights) {
        normalized_weights.push_back(static_cast<double>(w) / static_cast<double>(total_weight));
    }
    rng.init();
    generate_assignment();

    current_score = evaluate(current_assignment, Formula{});
    best_assignment = current_assignment;
    best_score = current_score;
}

void Annealer::generate_assignment() {
    for(size_t i=0; i<current_assignment.size(); ++i) {
        current_assignment[i] = rng.get_next() > 0.5;
    }
}

Annealer::Assignment Annealer::outer_loop(const Formula & formula) {
    while(temperature > min_temperature) {
        if constexpr (DEBUG) {
            std::cout << "Current temperature: " << temperature << "\n";
        }
        inner_loop(formula);
        cool();
    }
    return best_assignment;
}

void Annealer::inner_loop(const Formula & formula) {
    for(size_t iter=0; iter<inner_loop_iterations; ++iter) {
        auto neighbour = get_neighbour(rng, current_assignment);
        double neighbour_score = evaluate(neighbour, formula);
        double score_diff = neighbour_score - current_score;

        auto change_assignment = [&](){
            current_assignment = neighbour;
            current_score = neighbour_score;

            // Upon changing state cool faster (through shorter inner loop)
            ++iter;
        };

        if(score_diff > 0) {
            if(neighbour_score > best_score) {
                best_assignment = neighbour;
                best_score = neighbour_score;
            }
            change_assignment();
        } else {
            double acceptance_prob = std::exp(-score_diff / temperature);
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
        std::cout << "Evaluated score: " << score << "\n";
        assert(score <= 1.0 && score >= -static_cast<double>(formula.get_unsatisfied_clauses(assignment).size()));
    }
    return score;
}