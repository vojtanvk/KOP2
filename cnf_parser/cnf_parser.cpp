#include "include/cnf_parser/cnf_parser.hpp"

#include <iostream>
#include <cstring>
#include <cassert>
#include <sstream>

CNFFileParser::CNFFileParser(const std::string &in_filename) {
    file = std::ifstream(in_filename, file.in);
}
CNFFileParser::~CNFFileParser() { file.close(); }

CNFDefine CNFFileParser::fill_formula(Formula & to_fill) {
    // Strip header
    auto define = strip_header();

    // Fill formula with clauses
    size_t clause_cnt = 0;
    std::vector<Literal> lits;
    while(true) {
        
        int id;
        file >> id;
        // std::cout << "Read literal id: " << id << "\n"; 

        assert(static_cast<size_t>(std::abs(id)) <= define.number_of_literals);

        if(!file.good()) {
            break;
        }

        if(id) {
            // std::cout << "Adding literal " << id << " to current clause.\n";
            lits.push_back(Literal{id});
            // std::cout << "Current clause literals: ";
            // for(const auto & l : lits) {
            //     std::cout << (l.as_is ? "" : "-") << l.id << " ";
            // }
            // std::cout << "\n";
        } else {
            // std::cout << "End of clause reached. Clause literals: ";
            // for(const auto & l : lits) {
            //     std::cout << (l.as_is ? "" : "-") << l.id << " ";
            // }
            // std::cout << "\n";


            ++clause_cnt;
            Clause clause{{lits.begin(), lits.end()}};
            to_fill.add_clause(clause);
            // clause.print_clause();
            lits.clear();
        }
    }

    assert(clause_cnt == define.number_of_clauses);
    return define;
}

CNFDefine CNFFileParser::strip_header() {
    char c = 0;
    char s[200];
    while(file.good() && c != 'p') {
        file.getline(s, 200);
        c = s[0];
    }
    return parse_header(s);
}

CNFDefine CNFFileParser::parse_header(const std::string & s) {
    // Line should look like 
    // p cnf <number_of_literals> <number_of_clauses>

    assert(strncmp("p cnf", s.data(), 5) == 0);

    std::istringstream is{s.substr(5, s.size())};

    CNFDefine out{};
    is >> out.number_of_literals >> out.number_of_clauses;

    return out;
}

