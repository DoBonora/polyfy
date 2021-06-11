#include "../include/Ideal.hpp"
#include "../include/Term.hpp"

#include <gmpxx.h>
#include <regex>

#include <algorithm>
#include <iostream>

bool Ideal::add_variable(const std::string &var) {
  uint32_t ord = variables.size();

  if (variables.find(var) == variables.end()) {
    variables[var] = ord;
    return true;
  }
  return false;
}

Polynomial Ideal::from_string(const std::string &s) const {
  Polynomial poly;

  std::regex reg_first_mon("^[[:s:]]*([[:alnum:][:s:]\\*]+)");

  std::smatch match;
  auto pos = s.cbegin();
  auto end = s.cend();
  bool found = std::regex_search(pos, end, match, reg_first_mon);
  if (found) {
    Monomial m = monom_from_string(match[1]);
    poly += m;
    pos = match.suffix().first;
  }

  std::regex reg("([\\+\\-])[[:s:]]*([[:alnum:]\\*[:s:]]+)");
  for (; std::regex_search(pos, end, match, reg); pos = match.suffix().first) {
    int8_t sign = match[1] == '-' ? -1 : 1;
    Monomial m = monom_from_string(match[2]);
    
    poly += sign * m;
  }
  return poly;
}

Monomial Ideal::monom_from_string(const std::string &s) const {
  std::regex reg_coeff("^([\\-0-9][0-9]*)[[:s:]]*\\*?");
  std::smatch match;
  bool has_coeff = std::regex_search(s, match, reg_coeff);

  mpz_class coeff = 1;
  if (has_coeff) {
    coeff = std::stoi(match[1]);
  }

  std::set<uint32_t, std::greater<uint32_t>> monom_vars;

  std::regex reg_vars("[[:alpha:]][[:alnum:]]*");
  auto pos = s.cbegin();
  auto end = s.cend();
  for (; std::regex_search(pos, end, match, reg_vars);
       pos = match.suffix().first) {
    auto res_it = variables.find(match.str());
    if (res_it != variables.end()) {
      uint32_t var_ord = res_it->second;
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

  for(const Polynomial& g: generators) {
    while(g.can_lead_reduce(rem)) {
      g.lead_reduce(rem);
      if(rem == zero())
        return rem;
    }
  }
  
  return rem;
}

Polynomial Ideal::zero() const { return Polynomial(); }

Ideal::UndefinedVarException::UndefinedVarException(const std::string &var)
    : var(var) {}

const char *Ideal::UndefinedVarException::what() const noexcept {
  return ("Ideal does not contain variable " + var).c_str();
}
