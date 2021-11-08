#include "../include/Parse.hpp"
#include "../include/Utils.hpp"
#include <lorina/aiger.hpp>
#include <mockturtle/mockturtle.hpp>

#include <algorithm>
#include <gmp.h>
#include <gmpxx.h>
#include <string>
#include <unordered_map>

void Circuit::add_reg(const std::string &out, const std::string &in) {
    check_vars(out, in);
    reg_polys.emplace_back("-" + out + "+" + in);
}

void Circuit::add_and_pos_pos(const std::string &out, const std::string &in1,
                              const std::string &in2) {
  check_vars(out, in1, in2);
  polys.emplace_back("-" + out + "+" + in1 + "*" + in2);
}

void Circuit::add_and_pos_neg(const std::string &out, const std::string &in1,
                              const std::string &in2) {
  check_vars(out, in1, in2);
  polys.emplace_back("-" + out + "-" + in1 + "*" + in2 + "+" + in1);
}

void Circuit::add_and_neg_pos(const std::string &out, const std::string &in1,
                              const std::string &in2) {
  add_and_pos_neg(out, in2, in1);
}

void Circuit::add_and_neg_neg(const std::string &out, const std::string &in1,
                              const std::string &in2) {
  check_vars(out, in1, in2);
  polys.emplace_back("-" + out + "+" + in1 + "*" + in2 + "-" + in1 + "-" + in2 +
                     "+1");
}

void Circuit::add_not(const std::string &out, const std::string &in1) {
  check_vars(out, in1);
  polys.emplace_back("-" + out + "-" + in1 + "+1");
}
void Circuit::add_eq(const std::string &out, const std::string &in1) {
  check_vars(out, in1);
  polys.emplace_back("-" + out + "+" + in1);
}

std::string Circuit::get_unsigned_mult_spec() const {
  std::string spec;
  auto n = output_vars.size();
  for (size_t i = 0; i < n; i++) {
    mpz_class coeff(2);
    mpz_pow_ui(coeff.get_mpz_t(), coeff.get_mpz_t(), n - i - 1);
    spec += coeff.get_str() + "*" + output_vars[n-i - 1] + "+";
  }
  spec.resize(spec.size() - 1);

  for (size_t i = 0; i < n / 2; i++) {
    for (size_t j = 0; j < n / 2; j++) {
      mpz_class coeff(2);
      mpz_pow_ui(coeff.get_mpz_t(), coeff.get_mpz_t(),
                 (n /2- i - 1) + (n/2 - j - 1));
      spec += "-" + coeff.get_str() + "*" + input_vars[n / 2 - i - 1] + "*" +
              input_vars[n - j - 1];
    }
  }
  return spec;
}

std::vector<std::string> Circuit::get_vars() const {
  std::vector<std::string> vars;
  vars.insert(vars.end(), input_vars.begin(), input_vars.end());
  vars.insert(vars.end(), gate_vars.begin(), gate_vars.end());
  vars.insert(vars.end(), output_vars.begin(), output_vars.end());
  return vars;
}

void Circuit::check_var(const std::string &s) {
  if(var_lookup.find(s) != var_lookup.end())
    return;

  throw UndefinedVarException(s);
}

Circuit parse_aig(const std::string &file) {
  Circuit circuit;
  mockturtle::aig_network aig;
  auto const result = lorina::read_aiger(file, mockturtle::aiger_reader(aig));
  assert(result == lorina::return_code::success);

  uint64_t shift = aig.num_pos() + aig.num_pis() + aig.num_gates() + aig.num_registers() * 2; //add the registers
  std::unordered_map<uint64_t, std::string> to_string;
  for (size_t output = 0; output < aig.num_pos(); output++) {
    std::string output_name = "o" + std::to_string(output);
    to_string[shift + output] = output_name;
    circuit.add_output(output_name);
  }
  aig.foreach_node([&](auto node) {
    std::string varname;
    if (aig.is_pi(node)) {
      if(node == 0) {
          return;
      }

      varname = "i" + std::to_string(node-1);
      to_string[node] = varname;
      circuit.add_input(varname);
    }
    else if(aig.is_ro(node)){ //added case for register node
        varname = "r" + std::to_string(node);
        to_string[node] = varname;
        circuit.add_gate(varname);
    }
    else {
      varname = "l" + std::to_string(node);
      to_string[node] = varname;
      circuit.add_gate(varname);
    }
  });
  //aig.is_combinational()?std::cout<<"Is combinational"<<'\n':std::cout<<"Is not combinational"<<'\n';
  /* Create generator polynomials */
  aig.foreach_gate([&](auto gate) {
    mockturtle::aig_network::signal fanin[2];

    aig.foreach_fanin(
        gate, [&fanin, i = 0](auto signal) mutable { fanin[i++] = signal; });

    std::string out = to_string[gate];
    std::string in0 = to_string[aig.get_node(fanin[0])];
    std::string in1 = to_string[aig.get_node(fanin[1])];

    bool not_in0 = aig.is_complemented(fanin[0]);
    bool not_in1 = aig.is_complemented(fanin[1]);

    if (not_in0 && not_in1) {
      circuit.add_and_neg_neg(out, in0, in1);
    } else if (not_in0) {
      circuit.add_and_neg_pos(out, in0, in1);
    } else if (not_in1) {
      circuit.add_and_pos_neg(out, in0, in1);
    } else {
      circuit.add_and_pos_pos(out, in0, in1);
    }
  });
    aig.foreach_register([&](auto reg) {
        mockturtle::aig_network::node nri = aig.get_node(reg.first);
        mockturtle::aig_network::node nro = reg.second;
        std::string snri = to_string[nri];
        std::string snro = to_string[nro];

        circuit.add_reg(snro, snri);


    });

  uint32_t o = 0;
  aig.foreach_po([&](auto po) {
    auto out = to_string[shift + o++];
    auto in = to_string[aig.get_node(po)];

    if (aig.is_complemented(po)) {
      circuit.add_not(out, in);
    } else {
      circuit.add_eq(out, in);
    }
  });

  return circuit;
  ;
}
