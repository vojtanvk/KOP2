#include <cstddef>
#include <iostream>
#include <algorithm>

#include <cnf_structs/formula.hpp>
#include <cnf_parser/cnf_parser.hpp>
#include <rng_wrapper/rng_wrapper.hpp>
#include <solver/prob_sat_solver.hpp>

std::string get_command(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return {*itr};
    }
    return {};
}

int main(int argc, char ** argv) {

    std::string filename = "test_data";
    std::string cmd_filename = get_command(argv, argv + argc, "-f");
    if(!cmd_filename.empty()) {
        filename = cmd_filename;
    }

    CNFFileParser parser{filename};

    Formula formula{};
    auto define = parser.fill_formula(formula);

    RNGWrapper rng;
    rng.init();

    std::vector<bool> assignment;
    assignment.resize(define.number_of_literals, false);

    ProbSatSolver solver(define);
    solver.init();

    std::string rng_state_file = get_command(argv, argv + argc, "-rng_state");

    solver.get_rng().set_start_state(rng_state_file);
   
   
    std::string separator = ",";
    std::string cmd_separator = get_command(argv, argv + argc, "-s");
    if(!cmd_separator.empty()) {   
        separator = cmd_separator;
    }

    size_t repetitions = 1;
    std::string cmd_repetitions = get_command(argv, argv + argc, "-r");
    if(!cmd_repetitions.empty()) {   
        repetitions = std::stoul(cmd_repetitions);
    }
   
    size_t max_flips = 300;
    std::string cmd_max_flips = get_command(argv, argv + argc, "-flips");
    if(!cmd_max_flips.empty()) {   
        max_flips = std::stoul(cmd_max_flips);
    }
    

    size_t total_flips=0;
    solver.generate_assignment();
    for(size_t rep = 0; rep < repetitions && !solver.succesful(formula); ++rep) {
        solver.generate_assignment();
        size_t flips = 0;    
        


        while(!solver.step(formula) && flips < max_flips) {
            ++flips;
        }
        total_flips+=flips;
    }

    std::cerr << total_flips << separator << max_flips*repetitions << separator << formula.get_satisfied_clauses(solver.get_assignment()).size() << separator << define.number_of_clauses << "\n";
    solver.print_assignment();

    // save to file if option given
    std::string state_savefile = get_command(argv, argv+argc, "-save_rng");
    // std::cout << "State savefile: " << state_savefile << "\n";
    if(!state_savefile.empty()) {
        solver.get_rng().save(state_savefile);
    }

    exit(0);
}

