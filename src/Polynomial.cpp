#include "../include/Polynomial.hpp"

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
    new_monomials[t] = factor * m;
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

mpz_class Polynomial::lc() const { return monomials.begin()->second.coeff; }
Monomial Polynomial::lm() const { return monomials.begin()->second; }

Term Polynomial::lt() const { return monomials.begin()->second.t; }

mpz_class Polynomial::num_monomials() const { return monomials.size(); }

Polynomial operator*(int32_t factor, const Polynomial &p) { return p * factor; }
