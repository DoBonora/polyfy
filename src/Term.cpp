#include "../include/Term.hpp"
#include <algorithm>
#include <functional>
#include <iterator>

Term::Term(int32_t variable) : variables() { variables.push_back(variable); }

Term::Term(const std::vector<int32_t> vars) : variables(vars) { sort_vars(); }

Term::Term(const std::initializer_list<int32_t> vars) : variables(vars) {
  sort_vars();
}

Term &Term::operator*=(const Term &rhs) {
  size_t i = 0;
  size_t j = 0;
  size_t n = variables.size();
  size_t m = rhs.variables.size();

  while (i < n && j < m) {
    while (i < n && variables[i] > rhs.variables[j]) {
      ++i;
    }
    if (i >= n)
      break;

    while (j < m && variables[i] < rhs.variables[j]) {
      variables.push_back(rhs.variables[j++]);
    }

    if (j >= m)
      break;

    if (variables[i] == rhs.variables[j]) {
      ++i;
      ++j;
    }
  }

  for (; j < m; j++)
    variables.push_back(rhs.variables[j]);
  sort_vars();
  return *this;
}

Term operator*(const Term &t1, const Term &t2) {
  Term temp(t1);
  return temp *= t2;
}

bool Term::operator>(const Term &rhs) const {
  return std::lexicographical_compare(rhs.variables.begin(),
                                      rhs.variables.end(), variables.begin(),
                                      variables.end());
}

bool Term::operator>=(const Term &rhs) const {
  return *this > rhs || *this == rhs;
}

bool Term::operator<(const Term &rhs) const { return rhs > *this; }

bool Term::operator<=(const Term &rhs) const { return rhs >= *this; }

bool Term::operator==(const Term &rhs) const {
  return variables == rhs.variables;
}

void Term::sort_vars() {
  std::sort(variables.begin(), variables.end(), std::greater<>());
}
