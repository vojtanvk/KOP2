#pragma once

#include <cstddef>
#include <cmath>

struct Literal {
public:
    size_t id;
    Literal(long int in_l);

    bool as_is;

    bool operator<(const Literal & other) const;
    bool operator>(const Literal & other) const;
    bool operator==(const Literal & other) const;
    bool operator!=(const Literal & other) const;
    bool operator>=(const Literal & other) const;
    bool operator<=(const Literal & other) const;
};