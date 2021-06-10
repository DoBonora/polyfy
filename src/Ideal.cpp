#include "../include/Ideal.hpp"
#include "../include/Term.hpp"

#include <regex>
#include <gmpxx.h>

bool Ideal::add_variable(const std::string &var) {
  uint32_t ord = variables.size();
  if (variables.find(var) != variables.end()) {
    variables[var] = ord;
    return true;
  }
  return false;
}

Polynomial Ideal::from_string(const std::string &s) const {
  char delim = '+';

  Polynomial poly;

  size_t last = 0;
  size_t next = 0;
  std::string monom_str;
  while ((next = s.find(delim)) != std::string::npos) {
    monom_str = s.substr(last, next - last);
    
    Monomial monom = monom_from_string(monom_str);
    poly += monom;
  }
  Monomial monom = monom_from_string(monom_str);
  poly += monom;

  return poly;
}

Monomial Ideal::monom_from_string(const std::string& s) const {
  std::regex reg_coeff("^[\\-0-9][0-9]*");
  std::smatch match;
  bool has_coeff = std::regex_search(s, match, reg_coeff);

  mpz_class coeff = 1;
  if(has_coeff) {
    coeff = std::stoi(match.str());
  }

  std::set<uint32_t, std::greater<uint32_t>> monom_vars;
  
  std::regex reg_vars("[a-z][[:alnum:]]*");
  auto pos = s.cbegin();
  auto end = s.cend();
  for(; std::regex_search(pos, end, match, reg_vars); pos = match.suffix().first) {
    auto res_it = variables.find(match.str());
    if(res_it != variables.end()) {
      uint32_t var_ord = res_it->second;
      monom_vars.insert(var_ord);
    } else {
      throw UndefinedVarException(match.str());
    }
  }

  return Monomial(coeff, Term(monom_vars));
}

Ideal::UndefinedVarException::UndefinedVarException(const std::string& var): var(var) {}

const char* Ideal::UndefinedVarException::what() const noexcept {
  return ("Ideal does not contain variable " + var).c_str();
}


