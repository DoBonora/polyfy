#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_TEST_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_TEST_HPP_

class Name
{
public:
  //! Default constructor
  Name();

  //! Copy constructor
  Name(const Name &other);

  //! Move constructor
  Name(Name &&other) noexcept;

  //! Destructor
  virtual ~Name() noexcept;

  //! Copy assignment operator
  Name& operator=(const Name &other);

  //! Move assignment operator
  Name& operator=(Name &&other) noexcept;



protected:
private:
};


#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_TEST_HPP_ */
