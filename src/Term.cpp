#include "../include/Term.hpp"
#include <algorithm>

Term::Term(const std::set<int32_t, std::greater<int32_t>> variables)
    : variables(variables) {}

Term::Term(int32_t variable)
    : variables(std::set<int32_t, std::greater<int32_t>>({variable})) {}

Term::Term(const std::vector<int32_t> vars) {
  variables.insert(vars.begin(), vars.end());
}

Term::Term(const std::initializer_list<int32_t> vars) {
  variables.insert(vars.begin(), vars.end());
}

Term &Term::operator*=(const Term &other) {
  for(auto& v: other.variables)
    variables.insert(v);
  return *this;
}

Term operator*(const Term &t1, const Term &t2) {
  Term temp(t1);
  return temp *= t2;
}

bool Term::operator>(const Term &other) const {
  return std::lexicographical_compare(other.variables.begin(), other.variables.end(), variables.begin(), variables.end());
  // auto it1 = variables.begin(), it2 = other.variables.begin();
  // for (; it1 != variables.end() && it2 != other.variables.end() && *it1 == *it2;
  //      ++it1, ++it2)
  //   ;
  // return (it1 != variables.end() && it2 == other.variables.end()) ||
  //        (it1 != variables.end() && it2 != other.variables.end() &&
  //         *it1 > *it2);
}

bool Term::operator>=(const Term &other) const {
  return *this > other || *this == other;
  // auto it1 = variables.begin(), it2 = other.variables.begin();
  // for (; it1 != variables.end() && it2 != other.variables.end() && *it1 == *it2;
  //      ++it1, ++it2)
  //   ;
  // return (it2 == other.variables.end()) ||
  //        (it1 != variables.end() && it2 != other.variables.end() &&
  //         *it1 >= *it2);
}

bool Term::operator<(const Term &other) const { return other > *this; }

bool Term::operator<=(const Term &other) const { return other >= *this; }

bool Term::operator==(const Term &other) const {
  return variables == other.variables;
}
