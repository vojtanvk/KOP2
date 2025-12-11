#pragma once

#include <cstddef>

struct Literal {
public:
    Literal(long int in_l, size_t in_w);
    
    size_t id;
    bool as_is;
    size_t weight;

    bool operator<(const Literal & other) const;
    bool operator>(const Literal & other) const;
    bool operator==(const Literal & other) const;
    bool operator!=(const Literal & other) const;
    bool operator>=(const Literal & other) const;
    bool operator<=(const Literal & other) const;
};