#include "../include/Term.hpp"
#include <algorithm>

Term::Term(const std::set<uint32_t, std::greater<uint32_t>> variables)
    : variables(variables) {}

Term::Term(uint32_t variable)
    : variables(std::set<uint32_t, std::greater<uint32_t>>({variable})) {}

Term::Term(const std::vector<uint32_t> vars) {
  variables.insert(vars.begin(), vars.end());
}

Term::Term(const std::initializer_list<uint32_t> vars) {
  variables.insert(vars.begin(), vars.end());
}

Term Term::operator*(const Term &other) const {
  std::set<uint32_t, std::greater<uint32_t>> new_variables;
  std::merge(variables.begin(), variables.end(), other.variables.begin(),
             other.variables.end(),
             std::inserter(new_variables, new_variables.begin()));
  return Term(new_variables);
}

Term::Term(const Term &other) : variables(other.variables) {}

bool Term::operator>(const Term &other) const {
  auto it1 = variables.begin(), it2 = other.variables.begin();
  for (; it1 != variables.end() && it2 != other.variables.end() && *it1 == *it2;
       ++it1, ++it2)
    ;
  return (it1 != variables.end() && it2 == other.variables.end()) ||
         (it1 != variables.end() && it2 != other.variables.end() &&
          *it1 > *it2);
}

bool Term::operator>=(const Term &other) const {
  auto it1 = variables.begin(), it2 = other.variables.begin();
  for (; it1 != variables.end() && it2 != other.variables.end() && *it1 == *it2;
       ++it1, ++it2)
    ;
  return (it2 == other.variables.end()) ||
         (it1 != variables.end() && it2 != other.variables.end() &&
          *it1 >= *it2);
}

bool Term::operator<(const Term &other) const { return other > *this; }

bool Term::operator<=(const Term &other) const { return other >= *this; }

bool Term::operator==(const Term &other) const {
  return variables == other.variables;
}
