#pragma once

#include <string>
#include <cstdint>

class RNGWrapper {
public:
    void init();

    void set_seed(const std::string & in_seed);
    void set_start_state(const std::string & filename);
    void set_start_state(uint64_t in_s1, uint64_t in_s2, uint64_t in_s3, uint64_t in_s4);

    void save(const std::string & filename) const;
    float get_next();
private:
    // Default values (explicit unsigned long long suffix)
    uint64_t s1{12732784810903672734ULL};
    uint64_t s2{15167739637760160934ULL};
    uint64_t s3{4959043459477566120ULL};
    uint64_t s4{8362410840755353800ULL};

    uint64_t seed{6172840429334713770ULL}; // Default seed (documentation: "0x55AA55AA55AA55AA")

    // internal flags to prefer an explicitly provided state over seed
    bool state_given{false};
    bool seed_given{false};
};