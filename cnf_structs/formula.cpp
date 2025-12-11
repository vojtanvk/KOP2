#include "include/cnf_structs/formula.hpp"
#include <vector>
#include <cassert>

void Formula::add_clause(const Clause & in_c) {
    data.push_back(in_c);
    for(const auto & l : in_c.get_literals()) {
        clauses_by_literal[l.id].push_back(in_c);
    }
}

bool Formula::is_satisfied(const std::vector<bool> & assignment) const {
    for(const auto & c : data) {
        if(!c.is_satisfied(assignment)) {
            return false;
        }
    }
    return true;
}

std::vector<Clause> Formula::get_unsatisfied_clauses(const std::vector<bool> & in_assignment) const {
    std::vector<Clause> res;
    for (const auto & c : data) {
        if (!c.is_satisfied(in_assignment)) {
            res.emplace_back(c);
        }
    }
    return res;
}

const std::vector<Clause> & Formula::get_clauses_containing_literal(size_t lit_idx) const {
    const auto & out = clauses_by_literal.at(lit_idx);
    assert(!out.empty());
    return out;
}

std::vector<Clause> Formula::get_satisfied_clauses(const std::vector<bool> & in_assignment) const {
    std::vector<Clause> res;
    for (const auto & c : data) {
        if (c.is_satisfied(in_assignment)) {
            res.emplace_back(c);
        }
    }
    return res;
}