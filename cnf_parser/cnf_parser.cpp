#include "include/cnf_parser/cnf_parser.hpp"
#include "cnf_parser/cnf_define.hpp"

#include <iostream>
#include <cstring>
#include <cassert>
#include <sstream>
#include <vector>

CNFFileParser::CNFFileParser(const std::string &in_filename) {
    file = std::ifstream(in_filename, file.in);
}
CNFFileParser::~CNFFileParser() { file.close(); }

CNFDefine CNFFileParser::fill_formula(Formula & to_fill) {
    // Strip header
    auto define = strip_header();
    define.normalized_weights.resize(define.number_of_literals);
    define.denormalized_weights.resize(define.number_of_literals);

    char s[200];
    file.getline(s, 200);
    CNFFileParser::parse_weights(s,define);
    file.getline(s, 200);

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

    std::string def_prefix{"p mwcnf"};
    assert(strncmp(def_prefix.data(), s.data(), def_prefix.size()) == 0);

    std::istringstream is{s.substr(def_prefix.size(), s.size())};

    CNFDefine out{};
    is >> out.number_of_literals >> out.number_of_clauses;

    return out;
}

void CNFFileParser::parse_weights(const std::string & s, CNFDefine & define) {
    std::string w_prefix{"w"};
    assert(strncmp(w_prefix.data(), s.data(), w_prefix.size()) == 0);
    std::istringstream is{s.substr(w_prefix.size(), s.size())};
    double total_weight = 0.0;
    for(auto & w : define.denormalized_weights){
        is >> w;
        total_weight += w;
    }

    for(size_t i=0; i<define.denormalized_weights.size(); ++i) {
        define.normalized_weights[i] = define.denormalized_weights[i] / total_weight;
    }
}
