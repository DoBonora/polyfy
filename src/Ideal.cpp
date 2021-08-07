#include "../include/Ideal.hpp"
#include "../include/Term.hpp"
#include "./Utils.hpp"
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
    Monomial m = monom_from_string(match[2]);

    poly += sign * m;
  }
  return poly;
}

Monomial Ideal::monom_from_string(const std::string &s) const {
  static std::regex reg_coeff("^([\\-0-9][0-9]*)[[:s:]]*\\*?");
  std::smatch match;
  bool has_coeff = std::regex_search(s, match, reg_coeff);

  mpz_class coeff = 1;
  if (has_coeff) {
    coeff = std::stoi(match[1]);
  }

  std::set<int32_t, std::greater<int32_t>> monom_vars;

  std::regex reg_vars("[[:alpha:]][[:alnum:]]*");
  auto pos = s.cbegin();
  auto end = s.cend();
  for (; std::regex_search(pos, end, match, reg_vars);
       pos = match.suffix().first) {
    auto res_it = variables.find(match.str());
    if (res_it != variables.end()) {
      int32_t var_ord = res_it->second;
      monom_vars.insert(var_ord);
    } else {
      throw UndefinedVarException(match.str());
    }
  }

  return Monomial(coeff, Term(monom_vars));
}

void Ideal::add_generator(const Polynomial &p) { generators.insert(p); }

void Ideal::add_generator(const std::string &s) {
  add_generator(from_string(s));
}

Polynomial Ideal::reduce(const Polynomial &p) {
  Polynomial rem = p;
  
  for (const Polynomial &g : generators) {
    while (g.can_lead_reduce(rem)) {
      // std::cout << to_string(rem) << "\n";
      // std::cout << to_string(g) << "\n\n";

      g.linear_lm_lead_reduce(rem);
      if (rem == zero())
        return rem;
    }
  }

  return rem;
}

Polynomial Ideal::zero() const { return Polynomial(); }

std::string Ideal::to_string(const Polynomial &poly) const {
  if (poly.is_zero())
    return "0";
  std::string poly_str = "";
  for (auto &m : poly.monomials) {
    if (m.coeff >= 0)
      poly_str += "+";

    poly_str += m.coeff.get_str() + "*";
    for (auto var : m.t.variables) {
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
  for (auto g : generators)
    std::cout << to_string(g) << "\n";
}
