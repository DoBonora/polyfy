#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_IDEAL_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_IDEAL_HPP_

#include <set>
#include <stdint.h>
#include <string>
#include <unordered_map>

#include "./Monomial.hpp"
#include "./Polynomial.hpp"

/* only supports lexicographical order for now */
class Ideal {
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

  /* variable is assumed to be greater then all others inserted before */
  bool add_variable(const std::string &var);

  Polynomial from_string(const std::string &s) const;

  void add_generator(const Polynomial &p);
  void add_generator(const std::string &s);

  Polynomial reduce(const Polynomial &p);

  Polynomial zero() const;

protected:
private:
  /* variables ordered from smallest to largest*/
  std::unordered_map<std::string, uint32_t> variables;
  std::set<Polynomial, std::greater<>> generators;

  Monomial monom_from_string(const std::string &s) const;

  class UndefinedVarException : public std::exception {
    std::string var;

  public:
    UndefinedVarException(const std::string &var);

    virtual const char *what() const noexcept override;
  };
};

#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_IDEAL_HPP_ */
