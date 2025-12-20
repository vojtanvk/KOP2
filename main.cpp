#include "cnf_parser/cnf_define.hpp"
#include <cstddef>
#include <iostream>
#include <algorithm>

#include <cnf_structs/formula.hpp>
#include <cnf_parser/cnf_parser.hpp>
#include <numeric>
#include <rng_wrapper/rng_wrapper.hpp>
#include <annealer/annealer.hpp>

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

    if constexpr(DEBUG)
        std::cout << "Just to be clear, DEBUG is ON" << std::endl;

    auto filename = get_command(argv, argv+argc, "-file");
    std::cout << filename << std::endl;
    if(filename.empty()) {
        filename = ".data/wuf20-71-M/wuf20-01.mwcnf";
    }

    RNGWrapper rng;
    auto comm_rng_state = get_command(argv, argv+argc, "-rng_start");
    if(!comm_rng_state.empty()) {
        rng.set_start_state(comm_rng_state);
    }

    CNFFileParser parser{filename};
    Formula formula;
    CNFDefine def{parser.fill_formula(formula)};

    Annealer annealer{def};


    
    auto final_assignment = annealer.outer_loop(formula);



    // FINAL SCORE PRINTING
    size_t final_score = 0;
    std::cout << "{";
    for(size_t i=0; i<final_assignment.size(); ++i) {
        std::cout << (int) final_assignment[i] << "*" << def.literal_weights[i];
        if(i!=final_assignment.size()-1) {
            std::cout << ",";
        }
        if(final_assignment[i]) {
            final_score += def.literal_weights[i];
        }
    }
    std::cout << "} == " << final_score << std::endl;



    exit(0);
}

