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

static constexpr size_t buffer_size = 1024;

CNFDefine CNFFileParser::fill_formula(Formula & to_fill) {
    // Strip header
    auto define = strip_header();
    define.literal_weights.resize(define.number_of_literals);

    char s[buffer_size];
    file.getline(s, buffer_size);
    CNFFileParser::parse_weights(s,define);
    file.getline(s, buffer_size);

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

    if constexpr (DEBUG) {
        std::cout << "DEBUG: Parsed " << clause_cnt << " clauses. Expected: " << define.number_of_clauses << std::endl;
    }
    assert(clause_cnt == define.number_of_clauses);
    return define;
}

CNFDefine CNFFileParser::strip_header() {
    char c = 0;
    char s[buffer_size];
    while(file.good() && c != 'p') {
        file.getline(s, buffer_size);
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
    for(auto & w : define.literal_weights){
        is >> w;
    }
}
