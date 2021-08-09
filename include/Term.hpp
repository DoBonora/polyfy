#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_

#include <cstdint>
#include <initializer_list>
#include <set>
#include <vector>

class Term {
public:
  Term() = default;
  Term(const std::vector<int32_t> vars);
  Term(const std::initializer_list<int32_t> vars);
  Term(int32_t variable);

  Term &operator*=(const Term &other);

  bool operator<(const Term &other) const;

  bool operator<=(const Term &other) const;

  bool operator>(const Term &other) const;

  bool operator>=(const Term &other) const;

  bool operator==(const Term &other) const;

protected:
private:
  std::vector<int32_t> variables;  
  void sort_vars();
  friend class Ideal;
  friend class Polynomial;
  friend class Monomial;
};

Term operator*(const Term &t1, const Term &t2);
#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_ */
