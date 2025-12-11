#include "cnf_parser/cnf_define.hpp"
#include <cstddef>
#include <iostream>
#include <algorithm>

#include <cnf_structs/formula.hpp>
#include <cnf_parser/cnf_parser.hpp>
#include <rng_wrapper/rng_wrapper.hpp>

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

    auto filename = get_command(argv, argv+argc, "-file");
    std::cout << filename << std::endl;
    if(filename.empty()) {
        filename = ".data/wuf20-71-M/wuf20-01.mwcnf";
    }

    CNFFileParser parser{filename};
    Formula formula;
    CNFDefine def{parser.fill_formula(formula)};

    exit(0);
}

