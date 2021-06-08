#include "../include/Monomial.hpp"

Monomial::Monomial(uint32_t coeff, const Term& t): coeff(coeff), t(t){}

Monomial::Monomial(const mpz_class& coeff, const Term& t): coeff(coeff), t(t){}


Monomial Monomial::operator*(const Monomial& other) const {
  return Monomial(coeff*other.coeff, t*other.t);
}

Monomial Monomial::operator*(uint32_t var) const {
  return Monomial(coeff, t*Term(var));
}

Monomial Monomial::operator+(const Monomial& other) const {
  return Monomial(coeff+other.coeff, t);
}

Monomial Monomial::operator-(const Monomial& other) const {
    return Monomial(coeff-other.coeff, t);
}


Monomial& Monomial::operator*=(const Monomial& other) {
  t = t*other.t;
  return *this;
}

Monomial& Monomial::operator*=(uint32_t var)  {
  t = t*Term(var);
  return *this;
}

Monomial& Monomial::operator+=(const Monomial& other) {
  coeff += other.coeff;
  return *this;
}

Monomial& Monomial::operator-=(const Monomial& other) {
  coeff -= other.coeff;
  return *this; 
}

bool Monomial::operator<(const Monomial& other) const { return t < other.t; }
bool Monomial::operator<=(const Monomial& other) const { return t <= other.t; }
bool Monomial::operator>(const Monomial& other) const { return t > other.t; }
bool Monomial::operator>=(const Monomial& other) const { return t >= other.t; }
bool Monomial::operator==(const Monomial& other) const { return t == other.t; }



