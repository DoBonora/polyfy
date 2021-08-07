#include <iostream>
#include "../include/Parse.hpp"

auto helper_string = "usage: aig_to_poly [path_to_aig]";

int main(int argc, char** argv) {
  if(argc != 2) {
    std::cout << "Incorrect number of arguments" << "\n";
    std::cout << helper_string << "\n";
    return -1;
  }
  Circuit c = parse_aig(argv[1]);

  for(auto& p: c.get_polys())
    std::cout << p << "\n";
  return 0;
}
