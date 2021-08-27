#include "../include/Ideal.hpp"
#include "../include/Parse.hpp"
#include "../include/Utils.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>

auto helper_string = "usage: polyfy [path_to_aig]";

Ideal from_circuit(const Circuit &c) {
  Ideal I;
  for (auto &var : c.get_vars()) {
    I.add_variable(var);
  }

  for (auto &g : c.get_polys()) {
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

  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  Circuit c = parse_aig(argv[1]);
  Ideal I = from_circuit(c);
  //   I.print_generators();

  std::cout << "Verifying " << std::to_string(c.get_input_length())
            << "-bit unsigned multiplier." << std::endl;
  std::cout << "Number of variables: " << c.get_vars().size() << "\n";
  std::cout << "Number of gate polynomials: " << c.get_polys().size() << std::endl;

  Polynomial spec = I.from_string(c.get_unsigned_mult_spec());
  //  std::cout << I.to_string(spec) << "\n";
  auto start = high_resolution_clock::now();
  Polynomial rem = I.reduce(spec);
  auto end = high_resolution_clock::now();

  std::cout << "\nRemainder after reduction:"
            << "\n";
  std::cout << I.to_string(rem) << std::endl;

  auto time = duration_cast<milliseconds>(end - start);
  std::cout << "\nReduction took " << std::to_string(time.count()) << "ms"
            << "\n";

  return 0;
}
