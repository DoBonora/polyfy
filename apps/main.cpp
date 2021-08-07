#include "../include/Ideal.hpp"
#include "../include/Parse.hpp"
#include "../include/Utils.hpp"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>

auto helper_string = "usage: polyfy [path_to_aig]";

Ideal from_circuit(const Circuit &c) {
  Ideal I;
  for (auto &var : c.get_vars()) {
    std::cout << var << "\n";

    I.add_variable(var);
  }

  for (auto &g : c.get_polys()) {
    std::cout << g << "\n";

    try {
      I.add_generator(g);
    } catch (UndefinedVarException &e) {
      std::cout << e.what() << "\n";
      exit(-1);
    }
  }
  return I;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Incorrect number of arguments"
              << "\n";
    std::cout << helper_string << "\n";
    return -1;
  }

  Circuit c = parse_aig(argv[1]);
  Ideal I = from_circuit(c);
  I.print_generators();
  std::cout << c.get_unsigned_mult_spec() << "\n";
  Polynomial spec = I.from_string(c.get_unsigned_mult_spec());
  Polynomial rem = I.reduce(spec);
  std::cout << I.to_string(rem) << std::endl;
  return 0;
}
