#include "include/cnf_structs/literal.hpp"

#include <cmath>

Literal::Literal(long int in_l, size_t in_w) : id(std::abs(in_l)-1), as_is(in_l > 0), weight(in_w) {}

bool Literal::operator<(const Literal & other) const {
    return id < other.id;
}

bool Literal::operator>(const Literal & other) const {
    return id > other.id;
}

bool Literal::operator==(const Literal & other) const {
    return id == other.id;
}

bool Literal::operator!=(const Literal & other) const {
    return id != other.id;
}

bool Literal::operator>=(const Literal & other) const {
    return id >= other.id;
}

bool Literal::operator<=(const Literal & other) const {
    return id <= other.id;
}