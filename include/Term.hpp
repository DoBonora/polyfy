#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_

#include <cstdint>
#include <initializer_list>
#include <set>
#include <vector>

/*
 * A term is just a set of variables
 * To avoid recomputing hashes we define terms to be immutable
 */
class Term {
public:
  friend class Ideal;

  Term() = default;
  Term(const std::set<int32_t, std::greater<int32_t>> vars);
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
  std::set<int32_t, std::greater<int32_t>> variables;
  friend class Polynomial;
  friend class Monomial;
};

Term operator*(const Term &t1, const Term &t2);
#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_ */
