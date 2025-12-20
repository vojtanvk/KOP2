#include "include/annealer/annealer.hpp"
#include "rng_wrapper/rng_wrapper.hpp"

using namespace Annealer;

void Annealer::init(RNGWrapper &rng, Assignment& assignment) {
    rng.init();
    generate_assignment(rng, assignment);
}

void Annealer::generate_assignment(RNGWrapper & rng, Assignment & assignment) {
    for(size_t i=0; i<assignment.size(); ++i) {
        assignment[i] = rng.get_next() > 0.5;
    }
}

size_t Annealer::final_score(const std::vector<size_t> & weights, const Assignment& assignment) {
    size_t out=0;
    for(size_t i=0; i<weights.size(); ++i) {
        out += assignment[i]*weights[i];
    }   
    return out;
}

Assignment Annealer::get_neighbour(RNGWrapper & rng, const Assignment& current_assignment) {
    Assignment neighbour = current_assignment;
    size_t index = rng.get_next_idx(neighbour.size()-1);
    neighbour[index] = !neighbour[index];
    return neighbour;
}

