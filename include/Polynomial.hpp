#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_

#include "../include/Term.hpp"
#include "../include/Monomial.hpp"
#include <string>
#include <map>
#include <gmpxx.h>
#include <initializer_list>

class Polynomial
{
public:
  //! Default constructor
  Polynomial();

  Polynomial(const std::map<Term,Monomial, std::greater<>>& ms);
  Polynomial(const std::initializer_list<Monomial> monomials);

  Polynomial(const Polynomial& other);
  // //! Copy constructor
  // Polynomial(const Polynomial &other);

  // //! Move constructor
  // Polynomial(Polynomial &&other) noexcept;

  // //! Destructor
  // virtual ~Polynomial() noexcept;

  //! Copy assignment operator
  Polynomial& operator=(const Polynomial &other);

  // //! Move assignment operator
  // Polynomial& operator=(Polynomial &&other) noexcept;

  Polynomial operator*(const Polynomial& other) const;
  Polynomial operator*(const Monomial& m) const;
  Polynomial operator+(const Polynomial& other) const;
  Polynomial operator+(const Monomial& m) const;
  Polynomial operator-(const Polynomial& other) const;
  Polynomial operator-(const Monomial& m) const;

  
  Polynomial& operator*=(const Polynomial& other) ;
  Polynomial& operator*=(const Monomial& m) ;
  Polynomial& operator+=(const Polynomial& other) ;
  Polynomial& operator+=(const Monomial& m) ;
  Polynomial& operator-=(const Polynomial& other) ;
  Polynomial& operator-=(const Monomial& m) ;

  
  Polynomial operator-() const;
  /* only considers monomial ordering, coefficients are irrelevant*/
  bool operator<(const Polynomial& other) const;
  bool operator<=(const Polynomial& other) const;
  bool operator>(const Polynomial& other) const;
  bool operator>=(const Polynomial& other) const;
  bool operator==(const Polynomial& other) const;
  
  mpz_class lc() const;
  Monomial lm() const;
  Term lt() const;

  mpz_class num_monomials() const;
  
protected:
private:
  std::map<Term, Monomial, std::greater<>> monomials;
};


#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_ */

