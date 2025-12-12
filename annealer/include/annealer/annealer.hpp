#pragma once

#include <numeric>
#include <rng_wrapper/rng_wrapper.hpp>
#include <vector>

namespace Annealer {
    using Assignment = std::vector<bool>;

    void init(RNGWrapper &rng, Assignment& assignment);
    void generate_assignment(RNGWrapper& rng, Assignment& assingment);
    size_t final_score(const std::vector<size_t> & weights, const Assignment& assignment);
}
