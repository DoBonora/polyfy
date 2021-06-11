#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_MONOMIAL_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_MONOMIAL_HPP_

#include <gmpxx.h>
#include <initializer_list>

#include "../include/Term.hpp"


class Monomial
{
public:
  //! Default constructor
  Monomial();
  Monomial(int32_t coeff, const Term& t);
  Monomial(const mpz_class& coeff, const Term& t);
  Monomial(const Term& t);

  //! Copy constructor
  Monomial(const Monomial &other);

  // //! Move constructor
  // Monomial(Monomial &&other) noexcept;

  //! Copy assignment operator
  Monomial& operator=(const Monomial &other);

  // //! Move assignment operator
  // Monomial& operator=(Monomial &&other) noexcept;

  Monomial operator*(const Monomial& other) const;
  Monomial operator*(int32_t factor) const;

  /* these operations assume t == other.t */
  Monomial operator+(const Monomial& other) const;
  Monomial operator-(const Monomial& other) const;
  
  Monomial& operator*=(const Monomial& other) ;
  Monomial& operator*=(int32_t factor) ;
            
  /* these  operations assume t == other.t */
  Monomial& operator+=(const Monomial& other) ;
  Monomial& operator-=(const Monomial& other) ;

  Monomial operator-() const;
  /* only considers monomial ordering, coefficients are irrelevant*/
  bool operator<(const Monomial& other) const;
  bool operator<=(const Monomial& other) const;
  bool operator>(const Monomial& other) const;
  bool operator>=(const Monomial& other) const;
  bool operator==(const Monomial& other) const;
  bool operator!=(const Monomial& other) const;

  bool is_zero() const;
  bool is_one() const;

  bool factor(const Monomial& other) const;
  bool multiple(const Monomial& other) const; 
 
protected:
private:
  mpz_class coeff;
  Term t;

  Monomial& operator*=(const mpz_class& i);
  Monomial operator*(const mpz_class& i) const; 
  
  friend class Polynomial;
};

Monomial operator*(int32_t factor, const Monomial& m);

#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_MONOMIAL_HPP_ */
