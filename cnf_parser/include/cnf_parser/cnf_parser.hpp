#pragma once

#include <fstream>

#include "cnf_define.hpp"
#include <cnf_structs/formula.hpp>

struct CNFFileParser {
public:
    CNFFileParser(const std::string &in_file);
    ~CNFFileParser();
    
    CNFDefine fill_formula(Formula & to_fill);

private:
    std::ifstream file;

    CNFDefine strip_header();
    static CNFDefine parse_header(const std::string& s);
    static void parse_weights(const std::string& s, CNFDefine & define);
};