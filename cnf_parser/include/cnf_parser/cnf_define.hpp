#pragma once

#include <cstddef>
#include <vector>

struct CNFDefine {
    size_t number_of_literals;
    size_t number_of_clauses;
    std::vector<size_t> literal_weights;
};