#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_UTILS_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_UTILS_HPP_

#include <string>
#include <exception>

class UndefinedVarException : public std::exception {
  std::string var;

public:
  UndefinedVarException(const std::string &var): var(var) {}

  virtual const char *what() const noexcept override {
    return ("Variable is not defined" + var).c_str();
  }
};


#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_UTILS_HPP_ */


