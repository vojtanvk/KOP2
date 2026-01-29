#include "cnf_parser/cnf_define.hpp"
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <cmath>

#include <cnf_structs/formula.hpp>
#include <cnf_parser/cnf_parser.hpp>
#include <numeric>
#include <optional>
#include <rng_wrapper/rng_wrapper.hpp>
#include <annealer/annealer.hpp>
#include <annealer/measured_performance.hpp>



std::optional<std::string> get_command(char ** begin, char ** end, const std::string & option);
void print_help();
void annealer_setup(char ** begin, char ** end, InitialConfig & config);

int main(int argc, char ** argv) {

    auto help_flag = get_command(argv, argv+argc, "-h");
    if(help_flag) {
        print_help();
        exit(0);
    }

    if constexpr(DEBUG)
        std::cout << "Just to be clear, DEBUG is ON" << std::endl;

    auto filename = get_command(argv, argv+argc, "-file");
    
    if(!filename) {
        std::cerr << "Error: No input file specified. Use -file <filename> to specify the CNF input file.\n";
        exit(1);
    }
    
    CNFFileParser parser{filename.value()};
    Formula formula;
    CNFDefine def{parser.fill_formula(formula)};

    
    auto confidence_str = get_command(argv, argv+argc, "-confidence");
    double confidence = 0.95;
    if(confidence_str) {
        confidence = std::stod(confidence_str.value());
    }

    size_t repetitions = static_cast<size_t>(std::ceil(std::log(1.0 - confidence) / std::log(1-measured_performance.at(def.number_of_literals).probability)));
    
    InitialConfig config = measured_performance.at(def.number_of_literals).config;
    annealer_setup(argv, argv+argc, config);
    

    size_t best_score=0;
    auto best_assignment = std::vector<bool>(def.number_of_literals, false);
    size_t last_checkpoint = 0;

    Annealer annealer{def, config, formula};

    for(size_t rep = 0; rep < repetitions; ++rep) {
        
        annealer=Annealer{def, config, formula};

        auto final_assignment = annealer.outer_loop(formula);
        size_t final_score = 0;

        for(size_t i=0; i<final_assignment.size(); ++i) {
            if(final_assignment[i]) {
                final_score += def.literal_weights[i];
            }
        }

        if(final_score >= best_score) {
            best_score = final_score;
            best_assignment = final_assignment;
            last_checkpoint = rep;
        }
    }

    auto rng_save_str = get_command(argv, argv+argc, "-rng_save");
    if(rng_save_str) {
        annealer.save_rng_state(rng_save_str.value());
    }

    if constexpr (DEBUG) {
        std::cout << "DEBUG: Input file: " << filename.value() << "\n";
        std::cout << "DEBUG: Input parameters:\n";
        std::cout << "  Initial temperature: " << config.initial_temperature << "\n";
        std::cout << "  Cooling rate: " << config.cooling_rate << "\n";
        std::cout << "  Inner loop iterations: " << config.inner_loop_iterations << "\n";
        std::cout << "  Minimum temperature: " << config.min_temperature << "\n";
        std::cout << "  Repetitions: " << repetitions << "\n";
        std::cout << "-- End of input parameters --\n";
    }

    if constexpr (CALIBRATION) {
        std::cout << "CALIBRATION MODE: Best score found: " << best_score << "\n";
        std::cout << "Last checkpoint at repetition: " << last_checkpoint << "\n";
    }
    
    if(!formula.is_satisfied(best_assignment)) {
        best_score = 0;
        if constexpr (DEBUG) {
            std::cout << "DEBUG: best assignment DOES NOT satisfy the formula.\n";
        }
    }

    std::cout << best_score;

    for(size_t i=0; i<best_assignment.size(); ++i) {
        if(best_assignment[i]) {
            std::cout << " " << static_cast<int> (i+1);
        } else {
            std::cout << " " << static_cast<int>(-(i+1));
        }
    }
    std::cout << " 0" << std::endl;


    exit(0);
}

void annealer_setup(char ** begin, char ** end, InitialConfig & config) {
    auto rng_state_str = get_command(begin, end, "-rng_start");
    if(rng_state_str) {
        config.rng_start_state = rng_state_str.value();
    }

    auto temp_str = get_command(begin, end, "-temp");
    if(temp_str) {
        double temp = std::stod(temp_str.value());
        config.initial_temperature = temp;
    }

    auto cooling_str = get_command(begin, end, "-cooling");
    if(cooling_str) {
        double cooling = std::stod(cooling_str.value());
        config.cooling_rate = cooling;
    }

    auto inner_iter_str = get_command(begin, end, "-inner_iters");
    if(inner_iter_str) {
        size_t inner_iters = std::stoul(inner_iter_str.value());
        config.inner_loop_iterations = inner_iters;
    }

    auto min_temp_str = get_command(begin, end, "-min_temp");
    if(min_temp_str) {
        double min_temp = std::stod(min_temp_str.value());
        config.min_temperature = min_temp;
    }
}

std::optional<std::string> get_command(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if(itr != end) {
        if (++itr != end)
        {
            return {*itr};
        }
        // retrieved option has no value but is present
        return {""};
    }
    return std::nullopt;
}

void print_help() {
    std::cout << "Usage: annealer -file <filename> [options]\n";
    std::cout << "Options:\n";
    std::cout << "  -h                 Show this help message\n";
    std::cout << "  -file <filename>   Specify the input CNF file\n";
    std::cout << "  -confidence <value> Set the confidence level (default: 0.95)\n";
    std::cout << " ---------------------- Annealer parameters ------------------ \n";
    std::cout << "  -temp <value>      Set initial temperature\n";
    std::cout << "  -cooling <value>   Set cooling rate\n";
    std::cout << "  -inner_iters <n>   Set number of inner loop iterations \n";
    std::cout << "  -min_temp <value>  Set minimum temperature\n";
    std::cout << "  -rng_start <state_in> Set RNG start state\n";
    std::cout << "  -rng_save <state_out> Where RNG state will be saved\n";
}