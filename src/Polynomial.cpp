#include "../include/Polynomial.hpp"

//! Default constructor
Polynomial::Polynomial(): monomials() {}

Polynomial::Polynomial(const std::set<Monomial>& ms): monomials(ms) {}

Polynomial::Polynomial(const Polynomial& other): monomials(other.monomials) {}

Polynomial::Polynomial(const Monomial& m): monomials({m}) {}

//TODO
Polynomial Polynomial::operator*(const Polynomial& other) const {
  std::set<Monomial> new_monomials;

  for(auto m: other.monomials) {
    for(auto n: monomials) {
      new_monomials.insert(n*m);
    }
  }

  return Polynomial(new_monomials);
}

Polynomial Polynomial::operator*(const Monomial& m) const {
  std::set<Monomial> new_monomials;
  for(auto n: monomials) {
    new_monomials.insert(m*n);
  }
  return Polynomial(new_monomials);
}


Polynomial Polynomial::operator+(const Polynomial& other) const {
  std::set<Monomial> new_monomials;
  //TODO
  return Polynomial();
}

Polynomial Polynomial::operator+(const Monomial& m) const;
Polynomial Polynomial::operator-(const Polynomial& other) const;
Polynomial Polynomial::operator-(const Monomial& m) const;

  
Polynomial Polynomial::operator*=(const Polynomial& other) ;
Polynomial Polynomial::operator*=(const Monomial& m) ;
Polynomial Polynomial::operator+=(const Polynomial& other) ;
Polynomial Polynomial::operator+=(const Monomial& m) ;
Polynomial Polynomial::operator-=(const Polynomial& other) ;
Polynomial Polynomial::operator-=(const Monomial& m) ;

mpz_class Polynomial::lc() const {
  return monomials.begin()->coeff;
}
Monomial Polynomial::lm() const {
  return *monomials.begin();
}

Term Polynomial::lt() const {
  return monomials.begin()->t;
}

mpz_class Polynomial::num_monomials() const {
  return monomials.size();
}
