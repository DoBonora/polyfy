#include "../include/Ideal.hpp"
#include "../include/Term.hpp"
#include "../include/Utils.hpp"
#include <cctype>
#include <gmp.h>

#include <bits/stdint-intn.h>
#include <gmpxx.h>
#include <regex>

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

bool Ideal::add_variable(const std::string &var) {
    int32_t ord = variables.size();

    if (variables.find(var) == variables.end()) {
        variables[var] = ord;
        to_name[ord] = var;
        return true;
    }
    return false;
}

Polynomial Ideal::from_string(const std::string &s) const {
    Polynomial poly;

    static std::regex reg_first_mon("^[[:s:]]*([[:alnum:][:s:]\\*]+)");

    std::smatch match;
    auto pos = s.cbegin();
    auto end = s.cend();
    bool found = std::regex_search(pos, end, match, reg_first_mon);
    if (found) {
        Monomial m = monom_from_string(match[1]);
        poly += m;
        pos = match.suffix().first;
    }

    static std::regex reg("([\\+\\-])[[:s:]]*([[:alnum:]\\*[:s:]]+)");
    for (; std::regex_search(pos, end, match, reg); pos = match.suffix().first) {
        int8_t sign = match[1] == '-' ? -1 : 1;
        std::string monom_string = match[2];
        monom_string.erase(
                remove_if(monom_string.begin(), monom_string.end(), isspace),
                monom_string.end()); // remove whitespace
        Monomial m = monom_from_string(match[2]);

        poly += sign * m;
    }
    return poly;
}

Monomial Ideal::monom_from_string(const std::string &s) const {
    static std::string delim = "*";
    size_t pos = 0;
    std::string token;
    mpz_class coeff = 1;
    std::string s_copy = s;

    if (s.size() != 0 && std::isdigit(s[0])) {
        pos = s.find(delim);
        coeff = mpz_class(s.substr(0, pos));
        s_copy = s_copy.substr(pos + 1, std::string::npos);
    }
    if (pos == std::string::npos)
        return Monomial(coeff);

    std::vector<int32_t> monom_vars;

    while ((pos = s_copy.find(delim)) != std::string::npos) {
        token = s_copy.substr(0, pos);
        auto var = variables.find(token);

        if (var != variables.end()) {
            int32_t var_ord = var->second;
            monom_vars.push_back(var_ord);
        } else {
            throw UndefinedVarException(token);
        }
        s_copy = s_copy.substr(pos + 1, std::string::npos);
    }
    token = s_copy.substr(0, pos);
    auto var = variables.find(token);

    if (var != variables.end()) {
        int32_t var_ord = var->second;
        monom_vars.push_back(var_ord);
    } else {
        throw UndefinedVarException(token);
    }

    return Monomial(coeff, Term(monom_vars));
}

void Ideal::add_generator(const Polynomial &p) { generators.insert(p); }

void Ideal::add_generator(const std::string &s) {
    add_generator(from_string(s));
}

void Ideal::add_reg_generator(const Polynomial &p) { reg_generators.emplace_back(p); }

void Ideal::add_reg_generator(const std::string &s) {
    add_reg_generator(from_string(s));
}

void Ideal::add_terminal_reg_generator(const Polynomial &p) { terminal_reg_generators.emplace_back(p); }

void Ideal::add_terminal_reg_generator(const std::string &s) {
    add_terminal_reg_generator(from_string(s));
}

Polynomial Ideal::reduce(const Polynomial &p) {
    Polynomial rem = p;
    int timesteps = input_count; //TODO: assumption: Circuit needs as many clock ticks as it has bits
    for (int i = 0; i <= timesteps; i++) {
        for (const Polynomial &g: generators) {
            std::cout << "Reducing: " << to_string(rem) << std::endl;
            std::cout << "By generator: " << to_string(g) << std::endl;
            while (g.can_lead_reduce(rem)) {
                g.linear_lm_lead_reduce(rem);
                if (rem == zero())
                    return rem;
            }
            std::cout << "Result: " << to_string(rem) << std::endl<< std::endl;
        }
        //after all generator polynoms were used a timestep happens
        if(i < timesteps && !reg_generators.empty()) {
            std::cout << "Timestep: " << i <<" start" <<std::endl;
            for (const Polynomial &g: reg_generators) {
                std::cout << "Reducing: " << to_string(rem) << std::endl;
                std::cout << "By register: " << to_string(g) << std::endl;
                g.reg_reduce(rem);
                std::cout << "Result: " << to_string(rem) << std::endl<< std::endl;
            }
            rem.sort_monomials(); //as the reg operations arent lead reduce operations
            rem.aggregate_equal_monoms(); //we can just do a single sort and aggregate after the whole step
            std::cout << "Timestep: " << i <<" finish" <<std::endl;
        }
        else {
            //TODO: Terminal Timestep
            std::cout<< "Terminal timestep" << std::endl;
            for (const Polynomial &g: terminal_reg_generators) {
                std::cout << "Reducing: " << to_string(rem) << std::endl;
                std::cout << "By register: " << to_string(g) << std::endl;
                g.reg_reduce(rem);
                std::cout << "Result: " << to_string(rem) << std::endl<< std::endl;
            }
            rem.sort_monomials(); //as the reg operations arent lead reduce operations
            rem.aggregate_equal_monoms(); //we can just do a single sort and aggregate after the whole step
            std::cout << "Terminal Timestep finish" <<std::endl;
        }


    }

    //after all timesteps happened we need to apply the initial polynoms for all registers
    return rem;
}

Polynomial Ideal::zero() const { return Polynomial(); }

std::string Ideal::to_string(const Polynomial &poly) const {
    if (poly.is_zero())
        return "0";
    std::string poly_str = "";
    for (auto &m: poly.monomials) {
        if (m.coeff >= 0)
            poly_str += "+";

        poly_str += m.coeff.get_str() + "*";
        for (auto var: m.t.variables) {
            poly_str += to_name.at(var) + "*";
        }
        if (poly_str.size() >= 1)
            poly_str[poly_str.size() - 1] = ' ';
    }
    if (poly_str.size() >= 1)
        poly_str.resize(poly_str.size() - 1);
    return poly_str;
}

void Ideal::print_generators() const {
    std::cout << "Generators:" << std::endl;
    for (auto g: generators)
        std::cout << "   " << to_string(g) << "\n";
}

void Ideal::print_reg_generators() const {
    std::cout << "Register Generators:" << std::endl;
    for (auto g: reg_generators) {
        std::cout << "   " << to_string(g) << "\n";
    }
}

void Ideal::print_terminal_reg_generators() const {
    std::cout << "Terminal Register Generators:" << std::endl;
    for (auto g: terminal_reg_generators) {
        std::cout << "   " << to_string(g) << "\n";
    }
}
