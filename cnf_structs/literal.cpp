#include "include/cnf_structs/literal.hpp"

Literal::Literal(long int in_l) : id(std::abs(in_l)-1), as_is(in_l > 0) {}

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