#pragma once

#include <vector>
#include <map>

#include "clause.hpp"
#include "literal.hpp"

struct Formula {
private:
    std::vector<Clause> data;
public:
    Formula() = default;

    void add_clause(const Clause & in_c);

    bool is_satisfied(const std::vector<bool> & in_assignment) const;

    std::vector<Clause> get_unsatisfied_clauses(const std::vector<bool> & in_assignment) const;
    const std::vector<Clause> & get_clauses_containing_literal(size_t lit_idx) const;
    std::vector<Clause> get_satisfied_clauses(const std::vector<bool> & in_assignment) const;

    size_t count_unsatisfied_clauses(const std::vector<bool> & in_assignment) const;

    std::map<size_t, std::vector<Clause>> clauses_by_literal;
};