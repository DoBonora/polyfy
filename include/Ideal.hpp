#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_IDEAL_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_IDEAL_HPP_

#include <set>
#include <stdint.h>
#include <string>
#include <unordered_map>

#include "./Monomial.hpp"
#include "./Polynomial.hpp"

class Ideal {
public:
  friend Ideal from_aig(const std::string& file);
  friend Polynomial unsigned_mult_spec(const Ideal& I);

  /* variable is assumed to be greater then all others inserted before */
  bool add_variable(const std::string &var);
  int32_t get_variable(const std::string &var) { return variables[var];}
  
  Polynomial from_string(const std::string &s) const;

  void add_generator(const Polynomial &p);
  void add_generator(const std::string &s);

    void add_reg_generator(const Polynomial &p);
    void add_reg_generator(const std::string &s);

  Polynomial reduce(const Polynomial &p);

  Polynomial zero() const;

  std::string to_string(const Polynomial& poly) const;
  void print_generators() const;
  void print_reg_generators() const;

private:
  std::unordered_map<std::string, int32_t> variables;
  std::unordered_map<int32_t, std::string> to_name;
  std::set<Polynomial, std::greater<>> generators;
  std::set<Polynomial, std::greater<>> reg_generators;

  Monomial monom_from_string(const std::string &s) const;
};

#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_IDEAL_HPP_ */

