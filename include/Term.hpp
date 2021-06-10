#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_

#include <set>
#include <cstdint>
#include <vector>
#include <initializer_list>

/*
 * A term is just a set of variables
 * To avoid recomputing hashes we define terms to be immutable
*/
class Term
{
public:
  //! Default constructor
  Term() = default;
  Term(const std::set<uint32_t, std::greater<uint32_t>> variables);
  Term(const std::vector<uint32_t> vars);
  Term(const std::initializer_list<uint32_t> vars);
  Term(uint32_t variable);

  //! Copy constructor
  Term(const Term &other);

  // //! Move constructor
  // Term(Term &&other) noexcept;

  // //! Destructor
  // virtual ~Term() noexcept;

  //! Copy assignment operator
  Term& operator=(const Term &other) = default;

  //! Move assignment operator
  Term& operator=(Term &&other) noexcept = default;

  Term operator*(const Term& other) const;

  bool operator<(const Term& other) const;
  
  bool operator<=(const Term& other) const;
  
  bool operator>(const Term& other) const;
  
  bool operator>=(const Term& other) const;

  bool operator==(const Term& other) const;
  
protected:
private:
  std::set<uint32_t, std::greater<uint32_t>> variables;
};


#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_ */
