#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_

#include <set>
#include <cstdint>

/*
 * A term is just a set of variables
 * To avoid recomputing hashes we define terms to be immutable
*/
class Term
{
public:
  //! Default constructor
  Term(const std::set<uint32_t> variables);
  Term(uint32_t variable);

  //! Copy constructor
  Term(const Term &other);

  // //! Move constructor
  // Term(Term &&other) noexcept;

  // //! Destructor
  // virtual ~Term() noexcept;

  // //! Copy assignment operator
  // Term& operator=(const Term &other);

  //! Move assignment operator
  Term& operator=(Term &&other) noexcept;

  Term operator*(const Term& other) const;

  bool operator<(const Term& other) const;
  
  bool operator<=(const Term& other) const;
  
  bool operator>(const Term& other) const;
  
  bool operator>=(const Term& other) const;

  bool operator==(const Term& other) const;
  
protected:
private:
  const std::set<uint32_t> variables;
};


#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_TERM_HPP_ */
