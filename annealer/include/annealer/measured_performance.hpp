#include <map>
#include "utils.hpp"


struct MeasuredPerformanceEntry {
    double probability;
    InitialConfig config;
};

extern const std::map<size_t, MeasuredPerformanceEntry> measured_performance = {
    {20, {0.5956, {.initial_temperature=2, .inner_loop_iterations=1000, .min_temperature=1e-1, .cooling_rate=0.3}}},
    {36, {0.135, {.initial_temperature=2, .inner_loop_iterations=1000, .min_temperature=1e-1, .cooling_rate=0.3}}},
    {50, {0.075, {.initial_temperature=2, .inner_loop_iterations=1000, .min_temperature=0.0333, .cooling_rate=0.3}}},
    {75, {0.016, {.initial_temperature=2, .inner_loop_iterations=1000, .min_temperature=0.01, .cooling_rate=0.3}}}
};