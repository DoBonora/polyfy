#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_

#include "../include/Term.hpp"
#include "../include/Monomial.hpp"
#include <set>
#include <gmpxx.h>

class Polynomial
{
public:
  //! Default constructor
  Polynomial();

  Polynomial(const std::set<Monomial>& ms);
  Polynomial(const Monomial& m);

  Polynomial(const Polynomial& other);
  // //! Copy constructor
  // Polynomial(const Polynomial &other);

  // //! Move constructor
  // Polynomial(Polynomial &&other) noexcept;

  // //! Destructor
  // virtual ~Polynomial() noexcept;

  // //! Copy assignment operator
  // Polynomial& operator=(const Polynomial &other);

  // //! Move assignment operator
  // Polynomial& operator=(Polynomial &&other) noexcept;

  Polynomial operator*(const Polynomial& other) const;
  Polynomial operator*(const Monomial& m) const;
  Polynomial operator+(const Polynomial& other) const;
  Polynomial operator+(const Monomial& m) const;
  Polynomial operator-(const Polynomial& other) const;
  Polynomial operator-(const Monomial& m) const;

  
  Polynomial operator*=(const Polynomial& other) ;
  Polynomial operator*=(const Monomial& m) ;
  Polynomial operator+=(const Polynomial& other) ;
  Polynomial operator+=(const Monomial& m) ;
  Polynomial operator-=(const Polynomial& other) ;
  Polynomial operator-=(const Monomial& m) ;

  mpz_class lc() const;
  Monomial lm() const;
  Term lt() const;
  
  mpz_class num_monomials() const;
  
  
protected:
private:
  std::set<Monomial> monomials;
};


#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_POLYNOMIAL_HPP_ */
