#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_MONOMIAL_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_MONOMIAL_HPP_

#include <gmpxx.h>

#include "../include/Term.hpp"

    struct Monomial {
public:
  friend class Ideal;
  mpz_class coeff;
  Term t;

  Monomial() : coeff(0){};
  Monomial(int32_t coeff) : coeff(coeff){};

  Monomial(int32_t coeff, const Term &t);
  Monomial(const mpz_class &coeff) : coeff(coeff){};
  Monomial(const mpz_class &coeff, const Term &t);
  Monomial(const Term &t);

  Monomial operator*(const Monomial &other) const;
  Monomial operator*(int32_t factor) const;
  Monomial operator*(const mpz_class &i) const;
  
  Monomial &operator*=(const Monomial &other);
  Monomial &operator*=(int32_t factor);
  Monomial &operator*=(const mpz_class &i);

  Monomial operator-() const;

  bool operator<(const Monomial &other) const;
  bool operator<=(const Monomial &other) const;
  bool operator>(const Monomial &other) const;
  bool operator>=(const Monomial &other) const;
  bool operator==(const Monomial &other) const;
  bool operator!=(const Monomial &other) const;

  bool is_zero() const;
  bool is_one() const;

  bool factor(const Monomial &other) const;
  bool multiple(const Monomial &other) const;
};

Monomial operator*(int32_t factor, const Monomial &m);

#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_MONOMIAL_HPP_ */
