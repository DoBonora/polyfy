#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_

#include "../include/Monomial.hpp"
#include "../include/Term.hpp"
#include <gmpxx.h>
#include <initializer_list>
#include <map>
#include <string>

class Polynomial {
public:
  //! Default constructor
  Polynomial();

  Polynomial(const std::map<Term, Monomial, std::greater<>> &ms);
  Polynomial(const std::initializer_list<Monomial> monomials);

  Polynomial(const Polynomial &other);
  // //! Copy constructor
  // Polynomial(const Polynomial &other);

  // //! Move constructor
  // Polynomial(Polynomial &&other) noexcept;

  // //! Destructor
  // virtual ~Polynomial() noexcept;

  //! Copy assignment operator
  Polynomial &operator=(const Polynomial &other);

  // //! Move assignment operator
  // Polynomial& operator=(Polynomial &&other) noexcept;

  Polynomial operator*(const Polynomial &other) const;
  Polynomial operator*(const Monomial &m) const;
  Polynomial operator*(int32_t factor) const;
  Polynomial operator+(const Polynomial &other) const;
  Polynomial operator+(const Monomial &m) const;
  Polynomial operator-(const Polynomial &other) const;
  Polynomial operator-(const Monomial &m) const;

  Polynomial &operator*=(const Polynomial &other);
  Polynomial &operator*=(const Monomial &m);
  Polynomial operator*=(int32_t factor);
  Polynomial &operator+=(const Polynomial &other);
  Polynomial &operator+=(const Monomial &m);
  Polynomial &operator-=(const Polynomial &other);
  Polynomial &operator-=(const Monomial &m);

  Polynomial operator-() const;
  /* only considers monomial ordering, coefficients are irrelevant*/
  bool operator<(const Polynomial &other) const;
  bool operator<=(const Polynomial &other) const;
  bool operator>(const Polynomial &other) const;
  bool operator>=(const Polynomial &other) const;
  bool operator==(const Polynomial &other) const;
  bool operator!=(const Polynomial& other) const;
  const mpz_class& lc() const;
  const Monomial& lm() const;
  const Term& lt() const;

  mpz_class num_monomials() const;

  bool can_reduce(const Polynomial &other) const;
  bool reducible_by(const Polynomial &other) const;

  bool can_lead_reduce(const Polynomial &other) const;
  bool lead_reducible_by(const Polynomial &other) const;

  bool lead_reduce(Polynomial &other) const;
  bool reduce(Polynomial &other) const;

protected:
private:
  std::map<Term, Monomial, std::greater<>> monomials;

  Polynomial operator*(const mpz_class& i) const; 
  Polynomial operator*=(const mpz_class& i);
};

Polynomial operator*(int32_t factor, const Polynomial &p);

#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_ */
