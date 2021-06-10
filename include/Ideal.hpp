#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_IDEAL_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_IDEAL_HPP_

#include <unordered_map>
#include <stdint.h>
#include <string>
#include <set>

#include "./Polynomial.hpp"
#include "./Monomial.hpp"

class Ideal
{
public:
  // //! Default constructor
  // Ideal();

  // //! Copy constructor
  // Ideal(const Ideal &other);

  // //! Move constructor
  // Ideal(Ideal &&other) noexcept;

  // //! Destructor
  // virtual ~Ideal() noexcept;

  // //! Copy assignment operator
  // Ideal& operator=(const Ideal &other);

  // //! Move assignment operator
  // Ideal& operator=(Ideal &&other) noexcept;

  /* variable is assumed to be smaller then all others inserted before */
  bool add_variable(const std::string& var);

  Polynomial from_string(const std::string& s) const ;

  void add_generator(const Polynomial& p);

protected:
private:
  /* variables ordered from smallest to largest*/
  std::unordered_map<std::string, uint32_t> variables;
  std::set<Polynomial> generators;

  Monomial monom_from_string(const std::string& s) const;

  class UndefinedVarException : public std::exception {
    std::string var;

  public:
    UndefinedVarException(const std::string& var);

    virtual const char* what() const noexcept override;
  };
};


#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_IDEAL_HPP_ */
