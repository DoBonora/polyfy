#include "../include/Term.hpp"
#include <algorithm>
#include <functional>
#include <iterator>

// Term::Term(const std::set<int32_t, std::greater<int32_t>> variables)
//     : variables(variables) {}

Term::Term(int32_t variable) : variables() { variables.push_back(variable); }

Term::Term(const std::vector<int32_t> vars) : variables(vars) { sort_vars(); }

Term::Term(const std::initializer_list<int32_t> vars) : variables(vars) {
  sort_vars();
}

Term &Term::operator*=(const Term &other) {
  size_t i = 0;
  size_t j = 0;
  size_t n = variables.size();
  size_t m = other.variables.size();
  
  while (i < n && j < m) {
    while (i < n && variables[i] > other.variables[j]) {
      ++i;
    }
    if (i >= n)
      break;
    
    while (j < m && variables[i] < other.variables[j]) {
      variables.push_back(other.variables[j++]);
    }

    if (j >= m)
      break;

    if (variables[i] == other.variables[j]) {
      ++i;
      ++j;
    }
  }

  for (; j < m; j++)
    variables.push_back(other.variables[j]);
  sort_vars();
  return *this;
}

Term operator*(const Term &t1, const Term &t2) {
  Term temp(t1);
  return temp *= t2;
}

bool Term::operator>(const Term &other) const {
  return std::lexicographical_compare(other.variables.begin(), other.variables.end(), variables.begin(), variables.end());
}

bool Term::operator>=(const Term &other) const {
  return *this > other || *this == other;
}

bool Term::operator>=(const Term &other) const {
  return (*this)> other || *this == other;
  // size_t i = 0;
  // size_t j = 0;

  // for (; i < variables.size() && j < other.variables.size() &&
  //        variables[i] == other.variables[j];
  //      ++i, ++j)
  //   ;
  // return (i >= variables.size() && j >= other.variables.size()) ||
  //        (i < variables.size() && j >= other.variables.size()) ||
  //        (i < variables.size() && j < other.variables.size() &&
  //         variables[i] >= other.variables[j]);
>>>>>>> tuning
}

bool Term::operator<(const Term &other) const { return other > *this; }

bool Term::operator<=(const Term &other) const { return other >= *this; }

bool Term::operator==(const Term &other) const {
  return variables == other.variables;
}

void Term::sort_vars() {
  std::sort(variables.begin(), variables.end(), std::greater<>());
}
