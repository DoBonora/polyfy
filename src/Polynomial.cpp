#include "../include/Polynomial.hpp"

#include <algorithm>
#include <gmpxx.h>
#include <iostream>
#include <iterator>

const mpz_class Polynomial::const_0 = 0;
const Monomial Polynomial::monom_0 = Monomial();

// TODO: better way than to just instantiate a new polynomial?
Polynomial &Polynomial::operator*=(const Polynomial &rhs) {
  monoms_t prod;
  if (rhs.monomials.empty()) {
    monomials.clear();
    return *this;
  }
  // TODO: can this be more efficient?
  for (auto &m : monomials) {
    for (auto &n : rhs.monomials) {
      prod.emplace_back(m.coeff * n.coeff, m.t * n.t);
    }
  }
  monomials = prod;
  sort_monomials();
  aggregate_equal_monoms();
  return *this;
}

Polynomial &Polynomial::operator*=(const Monomial &m) {
  if (m.is_one())
    return *this;
  if (m.is_zero() || this->is_zero()) {
    monomials.clear();
    return *this;
  }
  return (*this) *= Polynomial(m);
}

Polynomial &Polynomial::operator*=(const mpz_class &factor) {
  if (factor == 1)
    return *this;

  if (factor == 0) {
    monomials.clear();
    return *this;
  }

  for (auto &m : monomials)
    m *= factor;

  return *this;
}

Polynomial &Polynomial::operator*=(int32_t constant) {
  return (*this) *= mpz_class(constant);
}

Polynomial &Polynomial::operator+=(const Polynomial &rhs) {
  auto m = monomials.begin();
  auto n = rhs.monomials.begin();

  while (m != monomials.end() && n != rhs.monomials.end()) {
    while (m != monomials.end() && m->t > n->t) {
      ++m;
    }

    if (m == monomials.end())
      break;

    while (n != rhs.monomials.end() && n->t > m->t) {
      monomials.insert(m, *n);
      ++n;
    }

    if (n == rhs.monomials.end())
      break;

    if (m->t == n->t) {
      m->coeff += n->coeff;
      if (m->is_zero())
        m = monomials.erase(m);
      ++n;
    }
  }

  for (; n != rhs.monomials.end(); ++n) {
    monomials.push_back(*n);
  }
  return *this;
}

Polynomial &Polynomial::operator+=(const Monomial &m) {
  Polynomial p(m);

  return *this += p;
}

// Implemented explicitly to avoid two traversals of list
Polynomial &Polynomial::operator-=(const Polynomial &rhs) {
  auto m = monomials.begin();
  auto n = rhs.monomials.begin();

  while (m != monomials.end() && n != rhs.monomials.end()) {
    while (m != monomials.end() && m->t > n->t) {
      ++m;
    }

    if (m == monomials.end())
      break;

    while (n != rhs.monomials.end() && n->t > m->t) {
      monomials.insert(m, -*n);
      ++n;
    }

    if (n == rhs.monomials.end())
      break;

    if (m->t == n->t) {
      m->coeff -= n->coeff;
      if (m->is_zero())
        m = monomials.erase(m);
      ++n;
    }
  }

  for (; n != rhs.monomials.end(); ++n) {
    monomials.push_back(-*n);
  }
  return *this;
}

Polynomial &Polynomial::operator-=(const Monomial &m) { return *this += (-m); }

Polynomial &Polynomial::operator+=(int32_t constant) {
  return (*this) += mpz_class(constant);
}

Polynomial &Polynomial::operator-=(int32_t constant) {
  return (*this) -= mpz_class(constant);
}

Polynomial Polynomial::operator-() const {
  Polynomial p;
  //  p.monomials.resize(monomials.size());
  for (const Monomial &m : *this)
    p.monomials.push_back(-m);
  return p;
}

Polynomial &Polynomial::operator+=(const mpz_class &constant) {
  return (*this) += Monomial(constant);
}

Polynomial &Polynomial::operator-=(const mpz_class &constant) {
  return *this -= Monomial(constant);
}

bool operator<(const Polynomial &lhs, const Polynomial &rhs) {
  return rhs > lhs;
}

bool operator<=(const Polynomial &lhs, const Polynomial &rhs) {
  return rhs >= lhs;
}

bool operator>(const Polynomial &lhs, const Polynomial &rhs) {
  auto m = lhs.begin(), n = rhs.begin();
  for (; m != lhs.end() && n != rhs.end() && *m == *n; m++, n++)
    ;

  return (m != lhs.end() && n == rhs.end()) ||
         (m != lhs.end() && n != rhs.end() && *m > *n);
}

bool operator>=(const Polynomial &lhs, const Polynomial &rhs) {
  auto m = lhs.begin(), n = rhs.begin();
  for (; m != lhs.end() && n != rhs.end() && *m == *n; m++, n++)
    ;

  return (m == lhs.end() && n == rhs.end()) ||
         (m != lhs.end() && n == rhs.end()) ||
         (m != lhs.end() && n != rhs.end() && *m > *n);
}

bool operator==(const Polynomial &lhs, const Polynomial &rhs) {
  auto m = lhs.begin(), n = rhs.begin();
  for (; m != lhs.end() && n != rhs.end() && *m == *n; m++, n++)
    ;

  return (m == lhs.end() && n == rhs.end());
}

bool operator!=(const Polynomial &lhs, const Polynomial &rhs) {
  return !(lhs == rhs);
}

const mpz_class &Polynomial::lc() const {
  if (this->is_zero())
    return monom_0.coeff;
  return monomials.begin()->coeff;
}

const Monomial &Polynomial::lm() const {
  if (this->is_zero())
    return monom_0;
  return *monomials.begin();
}

const Term &Polynomial::lt() const {
  if (this->is_zero())
    return monom_0.t;
  return monomials.begin()->t;
}

mpz_class Polynomial::num_monomials() const { return monomials.size(); }

// bool Polynomial::can_reduce(const Polynomial &rhs) const {
//   return rhs.reducible_by(*this);
// }

// bool Polynomial::reducible_by(const Polynomial &rhs) const {
//   for (auto [t, m] : monomials) {
//     if (rhs.lm().factor(m))
//       return true;
//   }

//   return false;
// }

bool Polynomial::can_lead_reduce(const Polynomial &rhs) const {
  return lm().factor(rhs.lm());
}

bool Polynomial::lead_reducible_by(const Polynomial &rhs) const {
  return rhs.can_lead_reduce(*this);
}

bool Polynomial::lead_reduce(Polynomial &rhs) const {
  if (!can_lead_reduce(rhs))
    return false;

  mpz_class lcm_ = lcm(lc(), rhs.lc());
  mpz_class fac1 = lcm_ / lc();
  mpz_class fac2 = lcm_ / rhs.lc();
  Term variable_fac;
  std::set_difference(
      rhs.lt().variables.begin(), rhs.lt().variables.end(),
      lt().variables.begin(), lt().variables.end(),
      std::inserter(variable_fac.variables, variable_fac.variables.begin()),
      std::greater<>());

  rhs *= fac2;
  rhs -= (*this) * Monomial(fac1, variable_fac);
  return true;
}

void Polynomial::linear_lm_lead_reduce(Polynomial &rhs) const {
  Term fac_to_lcm(rhs.lt());
  fac_to_lcm.variables.erase(*rhs.lt().variables.begin());
  if (this->lc() > 0)
    rhs -= (*this) * fac_to_lcm * rhs.lc();
  else
    rhs += (*this) * fac_to_lcm * rhs.lc();
}

void Polynomial::sort_monomials() {
  monomials.sort(
      [&](const Monomial &m1, const Monomial &m2) { return m1.t > m2.t; });
}

void Polynomial::aggregate_equal_monoms() {
  for (auto m = monomials.begin(); m != monomials.end();) {
    auto next = std::next(m);
    while (next != monomials.end() && m->t == next->t) {
      next->coeff += m->coeff;
      monomials.erase(m);
      m = next;
      next = std::next(next);
    }
    if (m->coeff == 0) {
      m = monomials.erase(m);
    } else {
      m++;
    }
  }
}
