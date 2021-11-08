#include "../include/Monomial.hpp"
#include <algorithm>

Monomial::Monomial(int32_t coeff, const Term &t) : coeff(coeff), t(t) {}

Monomial::Monomial(const mpz_class &coeff, const Term &t)
    : coeff(coeff), t(t) {}

Monomial::Monomial(const Term &t) : coeff(1), t(t) {}

Monomial Monomial::operator*(const Monomial &other) const {
  return Monomial(coeff * other.coeff, t * other.t);
}

Monomial Monomial::operator*(int32_t factor) const {
  return Monomial(coeff * factor, t);
}

Monomial &Monomial::operator*=(int32_t factor) {
  coeff *= factor;
  return *this;
}


Monomial Monomial::operator-() const { return Monomial(-coeff, t); }

bool Monomial::operator<(const Monomial &other) const {
  return t < other.t || (t == other.t && coeff < other.coeff);
}
bool Monomial::operator<=(const Monomial &other) const {
  return t <= other.t || (t == other.t && coeff <= other.coeff);
}
bool Monomial::operator>(const Monomial &other) const {
  return t > other.t || (t == other.t && coeff > other.coeff);
}
bool Monomial::operator>=(const Monomial &other) const {
  return t >= other.t || (t == other.t && coeff >= other.coeff);
}
bool Monomial::operator==(const Monomial &other) const {
  return t == other.t && coeff == other.coeff;
}
bool Monomial::operator!=(const Monomial &other) const {
  return !(*this == other);
}

bool Monomial::is_zero() const { return coeff == 0; }

bool Monomial::is_one() const { return coeff == 1 && t.variables.size() == 0; }

bool Monomial::factor(const Monomial &other) const {
  return std::includes(other.t.variables.begin(), other.t.variables.end(),
                       t.variables.begin(), t.variables.end(),
                       std::greater<>());
}

bool Monomial::multiple(const Monomial &other) const {
  return other.factor(*this);
}

Monomial &Monomial::operator*=(const mpz_class &i) {
  coeff *= i;
  return *this;
}

Monomial Monomial::operator*(const mpz_class &i) const {
  return Monomial(i * coeff, t);
}

Monomial operator*(int32_t factor, const Monomial &m) { return m * factor; }
