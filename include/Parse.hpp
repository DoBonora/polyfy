#ifndef POLYNOMIAL_VERIFICATION_INCLUDE_PARSE_HPP_
#define POLYNOMIAL_VERIFICATION_INCLUDE_PARSE_HPP_

#include <string>
#include <unordered_set>
#include <vector>

class Circuit {
public:
  inline void add_input(const std::string &i) {
    input_vars.emplace_back(i);
    var_lookup.insert(i);
  }
  inline void add_output(const std::string &o) {
    output_vars.emplace_back(o);
    var_lookup.insert(o);
  }
  /* gate_vars have to be added in topologically descending order */
  inline void add_gate(const std::string &g) {
    gate_vars.emplace_back(g);
    var_lookup.insert(g);
  }
  void add_reg(const std::string &out, const std::string &in);

  void add_and_pos_pos(const std::string &out, const std::string &in1,
                       const std::string &in2);
  void add_and_pos_neg(const std::string &out, const std::string &in1,
                       const std::string &in2);
  void add_and_neg_pos(const std::string &out, const std::string &in1,
                       const std::string &in2);
  void add_and_neg_neg(const std::string &out, const std::string &in1,
                       const std::string &in2);
  void add_not(const std::string &out, const std::string &in1);
  void add_eq(const std::string &out, const std::string &in1);

  std::vector<std::string> get_polys() const { return polys; }
    std::vector<std::string> get_reg_polys() const { return reg_polys; }
  std::string get_unsigned_mult_spec() const;
  std::vector<std::string> get_vars() const;
  int32_t get_input_length() const { return input_vars.size() / 2; }

private:
  std::vector<std::string> polys;
    std::vector<std::string> reg_polys;

  std::vector<std::string> gate_vars;
  std::vector<std::string> input_vars;
  std::vector<std::string> output_vars;

  std::unordered_set<std::string> var_lookup;

  void check_var(const std::string &s);
  template <typename... Args>
  void check_vars(const std::string &s, Args... ss) {
    check_var(s);
    if constexpr (sizeof...(ss) > 0)
      check_vars(ss...);
  }
};

Circuit parse_aig(const std::string &file);

#endif /* POLYNOMIAL_VERIFICATION_INCLUDE_PARSE_HPP_ */
