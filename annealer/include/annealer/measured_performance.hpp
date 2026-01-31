#include <map>
#include "utils.hpp"


struct MeasuredPerformanceEntry {
    double probability;
    InitialConfig config;
};

extern const std::map<size_t, MeasuredPerformanceEntry> measured_performance = {
    {20, {0.112, {.initial_temperature=2, .inner_loop_iterations=500, .min_temperature=0.333, .cooling_rate=0.1}}},
    {36, {0.06, {.initial_temperature=2, .inner_loop_iterations=500, .min_temperature=0.01, .cooling_rate=0.1}}},
    {50, {0.08, {.initial_temperature=2, .inner_loop_iterations=1000, .min_temperature=0.01, .cooling_rate=0.3}}},
    {75, {0.002, {.initial_temperature=0.2, .inner_loop_iterations=1000, .min_temperature=0.00333, .cooling_rate=0.7}}}
};

extern const std::map<size_t, double> measured_confidence = {
    {20, 0.95},
    {36, 0.9},
    {50, 0.5},
    {75, 0.5}
};