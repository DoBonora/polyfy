#include "../include/Polynomial.hpp"

#include <algorithm>
#include <gmpxx.h>
#include <iterator>
//! Default constructor
Polynomial::Polynomial() : monomials() {}

Polynomial::Polynomial(const std::map<Term, Monomial, std::greater<>> &ms)
    : monomials(ms) {}

Polynomial::Polynomial(const std::initializer_list<Monomial> monoms) {
  for (auto m : monoms)
    monomials[m.t] = m;
}

Polynomial::Polynomial(const Polynomial &other) : monomials(other.monomials) {}

Polynomial &Polynomial::operator=(const Polynomial &other) {
  monomials = other.monomials;
  return *this;
}

Polynomial Polynomial::operator*(const Polynomial &other) const {
  std::map<Term, Monomial, std::greater<>> new_monomials;

  for (auto [_, m] : other.monomials) {
    for (auto [_, n] : monomials) {
      Monomial prod = n * m;
      auto it = new_monomials.find(prod.t);
      if (it != new_monomials.end())
        new_monomials[prod.t] = it->second + prod;
      else
        new_monomials[prod.t] = prod;
    }
  }
  return Polynomial(new_monomials);
}

Polynomial Polynomial::operator*(const Monomial &m) const {
  if(m.is_one())
    return *this;
  
  std::map<Term, Monomial, std::greater<>> new_monomials;
  for (auto [_, n] : monomials) {
    Monomial prod = n * m;
    auto it = new_monomials.find(prod.t);
    if (it != new_monomials.end())
      new_monomials[prod.t] = it->second + prod;
    else
      new_monomials[prod.t] = prod;
  }
  return Polynomial(new_monomials);
}

Polynomial Polynomial::operator*(int32_t factor) const {
  std::map<Term, Monomial, std::greater<>> new_monomials;
  for (auto [t, m] : monomials)
    new_monomials[t] = m * factor;
  return Polynomial(new_monomials);
}

Polynomial Polynomial::operator+(const Polynomial &other) const {
  std::map<Term, Monomial, std::greater<>> new_monomials;
  auto it1 = monomials.begin();
  auto it2 = other.monomials.begin();

  for (; it1 != monomials.end() && it2 != other.monomials.end();) {
    while (it1 != monomials.end() && it1->first > it2->first) {
      new_monomials.insert(*it1);
      ++it1;
    }
    if (it1 == monomials.end())
      break;

    while (it2 != other.monomials.end() && it2->first > it1->first) {
      new_monomials.insert(*it2);
      ++it2;
    }

    if (it2 == other.monomials.end())
      break;

    if (it1->first == it2->first) {
      Monomial sum = it1->second + it2->second;
      if (!sum.is_zero())
        new_monomials[sum.t] = sum;

      ++it1;
      ++it2;
    } else {
      new_monomials.insert(*it1);
      ++it1;
      // if(it2 != other.monomials.end())
      //   new_monomials.insert(*it2); // TODO: might insert too often?
    }
  }

  while (it1 != monomials.end()) {
    new_monomials[it1->first] = it1->second;
    ++it1;
  }

  while (it2 != other.monomials.end()) {
    new_monomials[it2->first] = it2->second;
    ++it2;
  }
  return Polynomial(new_monomials);
}

Polynomial Polynomial::operator+(const Monomial &m) const {
  std::map<Term, Monomial, std::greater<>> new_monomials;
  auto it = monomials.find(m.t);

  new_monomials.insert(monomials.begin(), monomials.end());
  if (it != monomials.end()) {
    Monomial sum = it->second + m;
    if (!sum.is_zero())
      new_monomials[sum.t] = sum;
    else
      new_monomials.erase(sum.t);
  }

  return Polynomial(new_monomials);
}

Polynomial Polynomial::operator-(const Polynomial &other) const {
  std::map<Term, Monomial, std::greater<>> new_monomials;
  auto it1 = monomials.begin();
  auto it2 = other.monomials.begin();

  for (; it1 != monomials.end() && it2 != other.monomials.end();) {
    while (it1 != monomials.end() && it1->first > it2->first) {
      new_monomials.insert(*it1);
      ++it1;
    }
    if (it1 == monomials.end())
      break;

    while (it2 != other.monomials.end() && it2->first > it1->first) {
      new_monomials[it2->first] = -it2->second;
      ++it2;
    }

    if (it2 == other.monomials.end())
      break;

    if (it1->first == it2->first) {
      Monomial diff = it1->second - it2->second;
      if (!diff.is_zero())
        new_monomials[diff.t] = diff;

      ++it1;
      ++it2;
    } else {
      new_monomials.insert(*it1);
    }
  }

  while (it1 != monomials.end()) {
    new_monomials[it1->first] = it1->second;
    ++it1;
  }

  while (it2 != other.monomials.end()) {
    new_monomials[it2->first] = -it2->second;
    ++it2;
  }
  return Polynomial(new_monomials);
}

// TODO: better way than to just instantiate a new polynomial?
Polynomial &Polynomial::operator*=(const Polynomial &other) {
  (*this) = (*this) * other;
  return *this;
}

Polynomial &Polynomial::operator*=(const Monomial &m) {
  (*this) = (*this) * m;
  return *this;
}

Polynomial Polynomial::operator*=(int32_t factor) {
  for (auto [t, m] : monomials)
    monomials[t] = factor * m;
  return *this;
}

Polynomial &Polynomial::operator+=(const Polynomial &other) {
  auto it1 = monomials.begin();
  auto it2 = other.monomials.begin();

  for (; it1 != monomials.end() && it2 != other.monomials.end();) {
    while (it1 != monomials.end() && it1->first > it2->first) {
      ++it1;
    }
    if (it1 == monomials.end())
      break;

    while (it2 != other.monomials.end() && it2->first > it1->first) {
      monomials.insert(*it2);
      ++it2;
    }
    if (it2 == other.monomials.end())
      break;

    if (it1->first == it2->first) {
      it1->second += it2->second;
      if (it1->second.is_zero()) {
        auto temp = it1;
        ++it1;
        monomials.erase(temp); // TODO: potential source of error
      } else {
        ++it1;
      }
      ++it2;
    }
  }

  while (it2 != other.monomials.end()) {
    monomials[it2->first] = it2->second;
    ++it2;
  }

  return *this;
}
Polynomial &Polynomial::operator+=(const Monomial &m) {
  if(m.is_one())
    return *this;
  
  auto it = monomials.find(m.t);
  if (it != monomials.end()) {
    it->second += m;
    if (it->second.is_zero())
      monomials.erase(it);
  } else {
    if (m.coeff != 0)
      monomials[m.t] = m;
  }

  return *this;
}

Polynomial &Polynomial::operator-=(const Polynomial &other) {
  auto it1 = monomials.begin();
  auto it2 = other.monomials.begin();

  for (; it1 != monomials.end() && it2 != other.monomials.end();) {
    while (it1 != monomials.end() && it1->first > it2->first) {
      ++it1;
    }
    if (it1 == monomials.end())
      break;

    while (it2 != other.monomials.end() && it2->first > it1->first) {
      monomials[it2->first] = -it2->second;
      ++it2;
    }
    if (it2 == other.monomials.end())
      break;

    if (it1->first == it2->first) {
      it1->second -= it2->second;
      if (it1->second.is_zero()) {
        auto temp = it1;
        it1++;
        monomials.erase(temp); // TODO: potential source of error
      } else {
        ++it1;
      }
      ++it2;
    }
  }

  while (it2 != other.monomials.end()) {
    monomials[it2->first] = -it2->second;
    ++it2;
  }
  return *this;
}

Polynomial &Polynomial::operator-=(const Monomial &m) {
  auto it = monomials.find(m.t);
  if (it != monomials.end()) {
    it->second -= m;
    if (it->second.is_zero())
      monomials.erase(it);
  } else {
    if (m.coeff != 0)
      monomials[m.t] = m;
  }

  return *this;
}

Polynomial Polynomial::operator-() const {
  std::map<Term, Monomial, std::greater<>> new_monomials;
  for (auto [t, m] : monomials)
    new_monomials[t] = -m;

  return Polynomial(new_monomials);
}
/* only considers monomial ordering, coefficients are irrelevant*/
bool Polynomial::operator<(const Polynomial &other) const {
  return other > (*this);
}

bool Polynomial::operator<=(const Polynomial &other) const {
  return other >= (*this);
}

bool Polynomial::operator>(const Polynomial &other) const {
  auto it1 = monomials.begin(), it2 = other.monomials.begin();
  for (; it1 != monomials.end() && it2 != other.monomials.end() &&
         it1->second == it2->second;
       ++it1, ++it2)
    ;
  return (it1 != monomials.end() && it2 == other.monomials.end()) ||
         (it1 != monomials.end() && it2 != other.monomials.end() &&
          it1->second > it2->second);
}

bool Polynomial::operator>=(const Polynomial &other) const {
  auto it1 = monomials.begin(), it2 = other.monomials.begin();
  for (; it1 != monomials.end() && it2 != other.monomials.end() &&
         it1->second == it2->second;
       ++it1, ++it2)
    ;
  return (it2 == other.monomials.end()) ||
         (it1 != monomials.end() && it2 != other.monomials.end() &&
          it1->second >= it2->second);
}

bool Polynomial::operator==(const Polynomial &other) const {
  auto it1 = monomials.begin(), it2 = other.monomials.begin();
  for (; it1->second == it2->second; ++it1, ++it2)
    ;
  return it1 == monomials.end() && it2 == other.monomials.end();
}

bool Polynomial::operator!=(const Polynomial& other) const{
  return ! (*this == other);
}

const mpz_class& Polynomial::lc() const { return monomials.begin()->second.coeff; }
const Monomial& Polynomial::lm() const { return monomials.begin()->second; }

const Term& Polynomial::lt() const { return monomials.begin()->second.t; }

mpz_class Polynomial::num_monomials() const { return monomials.size(); }

bool Polynomial::can_reduce(const Polynomial &other) const {
  return other.reducible_by(*this);
}

bool Polynomial::reducible_by(const Polynomial &other) const {
  for (auto [t, m] : monomials) {
    if (other.lm().factor(m))
      return true;
  }
  return false;
}

bool Polynomial::can_lead_reduce(const Polynomial &other) const {
  return lm().factor(other.lm());
}

bool Polynomial::lead_reducible_by(const Polynomial &other) const {
  return other.can_lead_reduce(*this);
}

bool Polynomial::lead_reduce(Polynomial &other) const {
  if (!can_lead_reduce(other))
    return false;

  mpz_class lcm_ = lcm(lc(), other.lc());
  mpz_class fac1 = lcm_ / lc();
  mpz_class fac2 = lcm_ / other.lc();
  
  Term variable_fac;
  std::set_difference(other.lt().variables.begin(), other.lt().variables.end(),
                      lt().variables.begin(), lt().variables.end(),
                      std::inserter(variable_fac.variables,
                                    variable_fac.variables.begin()), std::greater<>());

  other *= fac2;
  other -= (*this) * Monomial(fac1, variable_fac);
  return true;
}
/*changes other */
bool Polynomial::reduce(Polynomial &other) const {
  auto it = other.monomials.begin();
  for (; !lm().factor(it->second); ++it)
    ;

  if (it == other.monomials.end())
    return false;
  return true;
}

Polynomial Polynomial::operator*(const mpz_class &i) const {
  if(i == 1)
    return *this;
  
  std::map<Term, Monomial, std::greater<>> new_monomials;
  for (auto [t, m] : monomials)
    new_monomials[t] = m * i;
  return Polynomial(new_monomials);
}
Polynomial Polynomial::operator*=(const mpz_class &i) {
  if(i == 1)
    return *this;
  
  for (auto [t, m] : monomials)
    monomials[t] = m * i;
  return *this;
}

Polynomial operator*(int32_t factor, const Polynomial &p) { return p * factor; }
