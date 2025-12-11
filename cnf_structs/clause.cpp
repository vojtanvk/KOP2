#include "include/cnf_structs/clause.hpp"

#include <iostream>

Clause::Clause(const std::set<Literal> & in_lits) : data(in_lits) {}

bool Clause::is_satisfied(const std::vector<bool> & assignment) const {
    // A clause is satisfied if any of its literals evaluates to true.
    for (const auto & l : data) {
        if (assignment[l.id] == l.as_is) {
            return true;
        }
    }
    return false;
}

bool Clause::will_break(const std::vector<bool> & assignment, size_t to_flip) const {
    bool completed_otherwise = false;
    bool completed = false;
    for(const auto & l : data) {
        if(assignment[l.id] == l.as_is) {
            completed_otherwise |= (l.id != to_flip);
            completed = true;
        }
    }
    return !completed_otherwise && completed;
}

std::vector<Literal> Clause::get_literals() const {
    return std::vector<Literal>(data.begin(), data.end());
}

bool Clause::contains_literal(size_t lit_idx) const {
    return data.contains(Literal(static_cast<long int>(lit_idx+1), 0));
}