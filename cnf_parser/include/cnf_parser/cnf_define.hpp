#pragma once

#include <cstddef>
#include <vector>

struct CNFDefine {
    size_t number_of_literals;
    size_t number_of_clauses;

    // normalized weights
    std::vector<double> normalized_weights;

    // denormalized weights
    std::vector<size_t> denormalized_weights;
};