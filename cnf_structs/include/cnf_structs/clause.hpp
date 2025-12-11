#pragma once

#include <set>
#include <vector>
#include <iostream>

#include "literal.hpp"

struct Clause {
private:
    std::set<Literal> data;
public:
    
    Clause(const std::set<Literal> & in_lits);

    bool is_satisfied(const std::vector<bool> & assignment) const;
    bool will_break(const std::vector<bool> & assignment, size_t to_flip) const;

    std::vector<Literal> get_literals() const;
    bool contains_literal(size_t lit_idx) const;

    bool operator<(const Clause & other) const {
        return data < other.data;
    }

    void print_clause() const {
        for (const auto & lit : data) {
            if (!lit.as_is) {
                std::cout << "-";
            }
            std::cout << lit.id << "\t";
        }
        std::cout << "0\n";
    }
};