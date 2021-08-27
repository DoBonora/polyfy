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

void print_poly(const Polynomial& p) {
  if(p.is_zero())
    std::cout << 0 << "\n";

  for(auto& m: p) {
    std::cout << m.coeff << "*";
    for(auto v: m.t) {
      std::cout << "v_" << v << "*";
    }
    std::cout << "+";
  }
  std::cout << std::endl << std::endl;
  
}
Polynomial &Polynomial::operator+=(const Polynomial &rhs) {
  // print_poly(*this);
  // print_poly(rhs);
  if (this->is_zero()) {
    *this = rhs;
    return *this;
  }

  if(rhs.is_zero())
    return *this;
  
  auto m = begin();
  auto n = rhs.begin();

  /*Small optimisation. Most of the time the first monomials will cancel*/
  if (m->t == n->t && m->coeff == -n->coeff) {
    monomials.erase(m.base());
    n++;
  }
  
  while (n != rhs.end()) {
    auto insert_pos = std::lower_bound(
        monomials.begin(), monomials.end(), *n,
        [&](const Monomial &m1, const Monomial &m2) { return m1.t < m2.t; });
    if (insert_pos != monomials.end() && insert_pos->t == n->t) {
      if (insert_pos->coeff == -n->coeff) {
        monomials.erase(insert_pos);
      } else {
        insert_pos->coeff += n->coeff;
      }
    } else {
      monomials.insert(insert_pos, *n);
    }
    ++n;
  }
  // while (m != end() && n != rhs.end()) {
  //   while (m != end() && m->t > n->t) {
  //     ++m;
  //   }

  //   if (m == end())
  //     break;

  //   while (n != rhs.end() && n->t > m->t) {

  //     monomials.insert(m.base(), *n);
  //     ++n;
  //   }

  //   if (n == rhs.end())
  //     break;

  //   if (m->t == n->t) {
  //     if(m->coeff == -n->coeff)
  //       m =
  //       std::reverse_iterator<std::vector<Monomial>::iterator>(monomials.erase(m.base()));
  //     else
  //       m->coeff += n->coeff;
  //     ++n;
  //   }
  // }

  // for (; n != rhs.end(); ++n) {
  //   monomials.push_back(*n);
  // }
  return *this;
}

Polynomial &Polynomial::operator+=(const Monomial &m) {
  Polynomial p(m);

  return *this += p;
}

// Implemented explicitly to avoid two traversals of list
Polynomial &Polynomial::operator-=(const Polynomial &rhs) {
  *this += -rhs;
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
  return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(),
                                      lhs.end());
  // return
  // auto m = lhs.begin(), n = rhs.begin();
  // for (; m != lhs.end() && n != rhs.end() && *m == *n; m++, n++)
  //   ;

  // return (m != lhs.end() && n == rhs.end()) ||
  //        (m != lhs.end() && n != rhs.end() && *m > *n);
}

bool operator>=(const Polynomial &lhs, const Polynomial &rhs) {
  return lhs > rhs || lhs == rhs;
  // auto m = lhs.begin(), n = rhs.begin();
  // for (; m != lhs.end() && n != rhs.end() && *m == *n; m++, n++)
  //   ;

  // return (m == lhs.end() && n == rhs.end()) ||
  //        (m != lhs.end() && n == rhs.end()) ||
  //        (m != lhs.end() && n != rhs.end() && *m > *n);
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
  return begin()->coeff;
}

const Monomial &Polynomial::lm() const {
  if (this->is_zero())
    return monom_0;
  return *begin();
}

const Term &Polynomial::lt() const {
  if (this->is_zero())
    return monom_0.t;
  return begin()->t;
}

mpz_class Polynomial::num_monomials() const { return monomials.size(); }

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
  Term fac_to_lcm(std::vector<int32_t>(rhs.lt().variables.begin() + 1,
                                       rhs.lt().variables.end()));

  if (this->lc() > 0)
    rhs -= (*this) * fac_to_lcm * rhs.lc();
  else
    rhs += (*this) * fac_to_lcm * rhs.lc();
}

void Polynomial::sort_monomials() {
  //  monomials.sort(
  //      [&](const Monomial &m1, const Monomial &m2) { return m1.t > m2.t; });
  std::sort(
      monomials.begin(), monomials.end(),
      [&](const Monomial &m1, const Monomial &m2) { return m1.t < m2.t; });
}

void Polynomial::aggregate_equal_monoms() {
  for (auto m = monomials.begin(); m != monomials.end();) {
    auto next = std::next(m);
    while (next != monomials.end() && m->t == next->t) {
      m->coeff += next->coeff;
      next = monomials.erase(next);
    }
    if (m->coeff == 0) {
      m = monomials.erase(m);
    } else {
      m = next;
    }
  }
}
