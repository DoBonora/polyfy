#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_

#include "../include/Monomial.hpp"
#include "../include/Term.hpp"
#include <functional>
#include <gmpxx.h>
#include <initializer_list>
#include <list>
#include <map>
#include <string>

class Polynomial {
  //using monoms_t = std::list<Monomial>;
  using monoms_t = std::vector<Monomial>;
public:
  using iterator = monoms_t::reverse_iterator;
  using const_iterator = monoms_t::const_reverse_iterator;

  Polynomial() = default;

  template <typename... Args> Polynomial(Monomial m, Args... ms) {
    monomials.emplace_back(m);
    (monomials.emplace_back(std::forward<Args>(ms)), ...);
    sort_monomials();
    aggregate_equal_monoms();
  }

  Polynomial &operator*=(const Polynomial &rhs);
  Polynomial &operator*=(const Monomial &m);
  Polynomial &operator*=(const mpz_class &factor);
  Polynomial &operator*=(int32_t constant);

  /* most of the run-time is spent in this method */
  Polynomial &operator+=(const Polynomial &rhs);
  
  Polynomial &operator+=(const Monomial &m);
  Polynomial &operator-=(const Polynomial &rhs);
  Polynomial &operator-=(const Monomial &m);
  Polynomial &operator+=(const mpz_class &constant);
  Polynomial &operator-=(const mpz_class &constant);
  Polynomial &operator+=(int32_t constant);
  Polynomial &operator-=(int32_t constant);

  Polynomial operator-() const;

  const mpz_class &lc() const;
  const Monomial &lm() const;
  const Term &lt() const;

  mpz_class num_monomials() const;

  // bool can_reduce(const Polynomial &rhs) const;
  // bool reducible_by(const Polynomial &rhs) const;

  bool can_lead_reduce(const Polynomial &rhs) const;
  bool can_linear_lm_lead_reduce(const Polynomial &rhs) const;
  bool lead_reducible_by(const Polynomial &rhs) const;

  bool lead_reduce(Polynomial &rhs) const;
  // Assumes that this polynomials lm is linear
  void linear_lm_lead_reduce(Polynomial &rhs) const;
  //  bool reduce(Polynomial &rhs) const;

  bool is_zero() const { return monomials.empty(); }

  iterator begin() { return monomials.rbegin(); }
  iterator end() { return monomials.rend(); }
  const_iterator begin() const { return monomials.crbegin(); }
  const_iterator end() const { return monomials.crend(); }
  const_iterator cbegin() const { return monomials.crbegin(); }
  const_iterator cend() const { return monomials.crend(); }

private:
  friend class Ideal;
  std::vector<Monomial> monomials;
  //std::list<Monomial> monomials;
  
  void sort_monomials();
  void aggregate_equal_monoms();

  static const mpz_class const_0;
  static const Monomial monom_0;
};

inline Polynomial operator*(Polynomial lhs, const Polynomial &rhs) {
  lhs *= rhs;
  return lhs;
}
inline Polynomial operator*(Polynomial lhs, const Monomial &rhs) {
  lhs *= rhs;
  return lhs;
}
inline Polynomial operator*(const Monomial &lhs, Polynomial rhs) {
  rhs *= lhs;
  return rhs;
}
inline Polynomial operator*(Polynomial p, const mpz_class &factor) {
  p *= factor;
  return p;
}
inline Polynomial operator*(const mpz_class &factor, Polynomial p) {
  p *= factor;
  return p;
}

inline Polynomial operator*(int32_t factor, Polynomial p) {
  p *= factor;
  return p;
}

inline Polynomial operator+(Polynomial lhs, const Polynomial &rhs) {
  lhs += rhs;
  return lhs;
}
inline Polynomial operator+(Polynomial lhs, const Monomial &rhs) {
  lhs += rhs;
  return lhs;
}
inline Polynomial operator+(const Monomial &lhs, Polynomial rhs) {
  rhs += lhs;
  return rhs;
}
inline Polynomial operator+(int32_t factor, Polynomial p) {
  p += factor;
  return p;
}
// Polynomial operator+(const Polynomial &p, const mpz_class& factor);
// Polynomial operator+(const mpz_class& factor, const Polynomial &p);

inline Polynomial operator-(Polynomial lhs, const Polynomial &rhs) {
  lhs -= rhs;
  return lhs;
}
inline Polynomial operator-(Polynomial lhs, const Monomial &rhs) {
  lhs -= rhs;
  return lhs;
}
inline Polynomial operator-(const Monomial &lhs, const Polynomial &rhs) {
  Polynomial temp(lhs);
  return temp - rhs;
}
inline Polynomial operator-(int32_t factor, const Polynomial &p) {
  Polynomial temp(factor);
  return temp - p;
}

inline Polynomial operator+(const Monomial &m1, const Monomial &m2) {
  Polynomial temp(m1);
  return temp + m2;
}
inline Polynomial operator-(const Monomial &m1, const Monomial &m2) {
  Polynomial temp(m1);
  return temp - m2;
}

bool operator<(const Polynomial &lhs, const Polynomial &rhs);
bool operator<=(const Polynomial &lhs, const Polynomial &rhs);
bool operator>(const Polynomial &lhs, const Polynomial &rhs);
bool operator>=(const Polynomial &lhs, const Polynomial &rhs);
bool operator==(const Polynomial &lhs, const Polynomial &rhs);
bool operator!=(const Polynomial &lhs, const Polynomial &rhs);

#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_ */
