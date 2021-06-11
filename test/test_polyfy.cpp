#include <gtest/gtest.h>

#include <cstdint>
#include <gmpxx.h>

#include "../include/Ideal.hpp"
#include "../include/Monomial.hpp"
#include "../include/Polynomial.hpp"
#include "../include/Term.hpp"

/*----------------------------------------------------------*/

class TermTest : public ::testing::Test {

public:
  /* x > y > z */
  const uint32_t x_var = 2;
  const uint32_t y_var = 1;
  const uint32_t z_var = 0;

  Term xyz, xy, xz, yz, x, y, z, unit;

protected:
  virtual void SetUp() {
    xyz = Term({x_var, y_var, z_var});
    xy = Term({x_var, y_var});
    xz = Term({x_var, z_var});
    yz = Term({y_var, z_var});
    x = Term(x_var);
    y = Term(y_var);
    z = Term(z_var);
    unit = Term();
  }
};

TEST_F(TermTest, EqTest) {
  Term xyz_test({x_var, y_var, z_var});
  ASSERT_TRUE(xyz_test == xyz);
}

TEST_F(TermTest, GTTest) {

  ASSERT_TRUE(xyz >= xyz);
  ASSERT_TRUE(xyz >= xy);
  ASSERT_TRUE(xyz >= xz);
  ASSERT_TRUE(xyz >= yz);
  ASSERT_TRUE(xyz >= x);
  ASSERT_TRUE(xyz >= y);
  ASSERT_TRUE(xyz >= z);
  ASSERT_TRUE(xyz >= unit);

  ASSERT_TRUE(xy >= xy);
  ASSERT_TRUE(xy >= xz);
  ASSERT_TRUE(xy >= yz);
  ASSERT_TRUE(xy >= x);
  ASSERT_TRUE(xy >= y);
  ASSERT_TRUE(xy >= z);
  ASSERT_TRUE(xy >= unit);

  ASSERT_TRUE(xz >= xz);
  ASSERT_TRUE(xz >= yz);
  ASSERT_TRUE(xz >= x);
  ASSERT_TRUE(xz >= y);
  ASSERT_TRUE(xz >= z);
  ASSERT_TRUE(xz >= unit);

  ASSERT_TRUE(yz >= yz);
  ASSERT_TRUE(yz >= y);
  ASSERT_TRUE(yz >= z);
  ASSERT_TRUE(yz >= unit);

  ASSERT_TRUE(x >= x);
  ASSERT_TRUE(x >= yz);
  ASSERT_TRUE(x >= y);
  ASSERT_TRUE(x >= z);
  ASSERT_TRUE(x >= unit);

  ASSERT_TRUE(y >= y);
  ASSERT_TRUE(y >= z);
  ASSERT_TRUE(y >= unit);

  ASSERT_TRUE(z >= z);
  ASSERT_TRUE(z >= unit);

  ASSERT_TRUE(unit >= unit);
}

TEST_F(TermTest, LTTest) {

  ASSERT_TRUE(xyz <= xyz);
  ASSERT_TRUE(xy <= xyz);
  ASSERT_TRUE(xz <= xyz);
  ASSERT_TRUE(yz <= xyz);
  ASSERT_TRUE(x <= xyz);
  ASSERT_TRUE(y <= xyz);
  ASSERT_TRUE(z <= xyz);
  ASSERT_TRUE(unit <= xyz);

  ASSERT_TRUE(xy <= xy);
  ASSERT_TRUE(xz <= xy);
  ASSERT_TRUE(yz <= xy);
  ASSERT_TRUE(x <= xy);
  ASSERT_TRUE(y <= xy);
  ASSERT_TRUE(z <= xy);
  ASSERT_TRUE(unit <= xy);

  ASSERT_TRUE(xz <= xz);
  ASSERT_TRUE(yz <= xz);
  ASSERT_TRUE(x <= xz);
  ASSERT_TRUE(y <= xz);
  ASSERT_TRUE(z <= xz);
  ASSERT_TRUE(unit <= xz);

  ASSERT_TRUE(yz <= yz);
  ASSERT_TRUE(y <= yz);
  ASSERT_TRUE(z <= yz);
  ASSERT_TRUE(unit <= yz);

  ASSERT_TRUE(x <= x);
  ASSERT_TRUE(yz <= x);
  ASSERT_TRUE(y <= x);
  ASSERT_TRUE(z <= x);
  ASSERT_TRUE(unit <= x);

  ASSERT_TRUE(y <= y);
  ASSERT_TRUE(z <= y);
  ASSERT_TRUE(unit <= y);

  ASSERT_TRUE(z <= z);
  ASSERT_TRUE(unit <= z);

  ASSERT_TRUE(unit <= unit);
}

TEST_F(TermTest, GreaterTest) {

  ASSERT_TRUE(xyz > xy);
  ASSERT_TRUE(xyz > xz);
  ASSERT_TRUE(xyz > yz);
  ASSERT_TRUE(xyz > x);
  ASSERT_TRUE(xyz > y);
  ASSERT_TRUE(xyz > z);
  ASSERT_TRUE(xyz > unit);

  ASSERT_TRUE(xy > xz);
  ASSERT_TRUE(xy > yz);
  ASSERT_TRUE(xy > x);
  ASSERT_TRUE(xy > y);
  ASSERT_TRUE(xy > z);
  ASSERT_TRUE(xy > unit);

  ASSERT_TRUE(xz > yz);
  ASSERT_TRUE(xz > x);
  ASSERT_TRUE(xz > y);
  ASSERT_TRUE(xz > z);
  ASSERT_TRUE(xz > unit);

  ASSERT_TRUE(yz > y);
  ASSERT_TRUE(yz > z);
  ASSERT_TRUE(yz > unit);

  ASSERT_TRUE(x > y);
  ASSERT_TRUE(x > yz);
  ASSERT_TRUE(x > z);
  ASSERT_TRUE(x > unit);

  ASSERT_TRUE(y > z);
  ASSERT_TRUE(y > unit);

  ASSERT_TRUE(z > unit);

  ASSERT_FALSE(unit > unit);
}

TEST_F(TermTest, LessTest) {

  ASSERT_TRUE(xy < xyz);
  ASSERT_TRUE(xz < xyz);
  ASSERT_TRUE(yz < xyz);
  ASSERT_TRUE(x < xyz);
  ASSERT_TRUE(y < xyz);
  ASSERT_TRUE(z < xyz);
  ASSERT_TRUE(unit < xyz);

  ASSERT_TRUE(xz < xy);
  ASSERT_TRUE(yz < xy);
  ASSERT_TRUE(x < xy);
  ASSERT_TRUE(y < xy);
  ASSERT_TRUE(z < xy);
  ASSERT_TRUE(unit < xy);

  ASSERT_TRUE(yz < xz);
  ASSERT_TRUE(x < xz);
  ASSERT_TRUE(y < xz);
  ASSERT_TRUE(z < xz);
  ASSERT_TRUE(unit < xz);

  ASSERT_TRUE(y < yz);
  ASSERT_TRUE(z < yz);
  ASSERT_TRUE(unit < yz);

  ASSERT_TRUE(y < x);
  ASSERT_TRUE(yz < x);
  ASSERT_TRUE(z < x);
  ASSERT_TRUE(unit < x);

  ASSERT_TRUE(z < y);
  ASSERT_TRUE(unit < y);

  ASSERT_TRUE(unit < z);

  ASSERT_FALSE(unit < unit);
}

TEST_F(TermTest, MulTest) {
  ASSERT_TRUE(xy * z == xyz);
  ASSERT_TRUE(z * xy == xy * z);
  ASSERT_TRUE(x * (y * z) == (x * y) * z);
  ASSERT_TRUE(x * x == x);
  ASSERT_TRUE(unit * x == x);
  ASSERT_FALSE(x * y == y * z);
}

/*----------------------------------------------------------*/

class MonomialTest : public ::testing::Test {

public:
  /* x > y > z */
  const uint32_t x_var = 2;
  const uint32_t y_var = 1;
  const uint32_t z_var = 0;

  Monomial xyz, xy, xz, yz, x, y, z, unit;

protected:
  virtual void SetUp() {
    xyz = Monomial(Term({x_var, y_var, z_var}));
    xy = Monomial(Term({x_var, y_var}));
    xz = Monomial(Term({x_var, z_var}));
    yz = Monomial(Term({y_var, z_var}));
    x = Monomial(Term(x_var));
    y = Monomial(Term(y_var));
    z = Monomial(Term(z_var));
    unit = Monomial(Term());
  }
};

TEST_F(MonomialTest, GTTest) {
  ASSERT_TRUE(xyz >= xyz);
  ASSERT_TRUE(Monomial(2, Term({x_var, y_var, z_var})) >= xyz);
  ASSERT_TRUE(xyz >= Monomial(-1, Term({x_var, y_var, z_var})));
  ASSERT_TRUE(xyz >= yz);
  ASSERT_TRUE(xyz >= unit);
  ASSERT_TRUE(x >= yz);
}

TEST_F(MonomialTest, GreaterTest) {
  ASSERT_TRUE(Monomial(2, Term({x_var, y_var, z_var})) > xyz);
  ASSERT_TRUE(xyz > Monomial(-1, Term({x_var, y_var, z_var})));
  ASSERT_TRUE(xyz > yz);
  ASSERT_TRUE(xyz > unit);
  ASSERT_TRUE(x > yz);
}

TEST_F(MonomialTest, LTTest) {
  ASSERT_TRUE(xyz <= xyz);
  ASSERT_TRUE(xyz <= Monomial(2, Term({x_var, y_var, z_var})));
  ASSERT_TRUE(Monomial(-1, Term({x_var, y_var, z_var})) <= xyz);
  ASSERT_TRUE(yz <= xyz);
  ASSERT_TRUE(unit <= xyz);
  ASSERT_TRUE(yz <= x);
}

TEST_F(MonomialTest, LesserTest) {
  ASSERT_TRUE(xyz < Monomial(2, Term({x_var, y_var, z_var})));
  ASSERT_TRUE(Monomial(-1, Term({x_var, y_var, z_var})) < xyz);
  ASSERT_TRUE(yz < xyz);
  ASSERT_TRUE(unit < xyz);
  ASSERT_TRUE(yz < x);
}

TEST_F(MonomialTest, MulTest) {
  ASSERT_TRUE(2 * x == Monomial(2, Term(x_var)));
  ASSERT_TRUE(x * y == y * x);
  ASSERT_TRUE(2 * (x * y) == (2 * x) * y);
  ASSERT_TRUE((-1) * x == -x);
  ASSERT_TRUE(x * (y * z) == (x * y) * z);
  ASSERT_TRUE(2 * x * y * z == 2 * xyz);
  ASSERT_TRUE(x * unit == x);
  ASSERT_TRUE(x * x == x);
}

TEST_F(MonomialTest, AddTest) {
  ASSERT_TRUE(x + x == Monomial(2, Term(x_var)));
  ASSERT_TRUE(2 * (x + x) == (2 * x) + (2 * x));

  Monomial a = x;
  a += x;
  ASSERT_TRUE(a == 2 * x);
  a -= x;
  ASSERT_TRUE(a == x);
  a -= 2 * x;
  ASSERT_TRUE(a == -x);
}

TEST_F(MonomialTest, ZeroTest) {
  Monomial zero(0, Term());
  ASSERT_TRUE(zero.is_zero());
  ASSERT_FALSE(x.is_zero());
  ASSERT_FALSE(unit.is_zero());
}

TEST_F(MonomialTest, DivisibleTest) {
  ASSERT_TRUE(x.factor(xyz));
  ASSERT_TRUE(xyz.multiple(x));
  ASSERT_TRUE(y.factor(xyz));
  ASSERT_FALSE(x.factor(y));
  ASSERT_TRUE(unit.factor(x));
  ASSERT_TRUE(unit.factor(unit));
  ASSERT_TRUE(xyz.factor(xyz));
  ASSERT_FALSE(xy.factor(yz));
}

/*----------------------------------------------------------*/

class PolynomialTest : public ::testing::Test {

public:
  /* x > y > z */
  const uint32_t x_var = 2;
  const uint32_t y_var = 1;
  const uint32_t z_var = 0;

  Polynomial f, g, h, u, v, zero;
  Monomial xyz, xy, xz, yz, x, y, z, unit;

protected:
  virtual void SetUp() {
    xyz = Monomial(Term({x_var, y_var, z_var}));
    xy = Monomial(Term({x_var, y_var}));
    xz = Monomial(Term({x_var, z_var}));
    yz = Monomial(Term({y_var, z_var}));
    x = Monomial(Term(x_var));
    y = Monomial(Term(y_var));
    z = Monomial(Term(z_var));
    unit = Monomial(Term());

    f = Polynomial({xyz, xy, xz, yz, x, y, z, unit});
    g = Polynomial({yz, y});
    h = Polynomial({x, unit});
    u = Polynomial({xyz, xy, xz, yz, 2 * x, y, z, unit});
    v = Polynomial({5 * xyz});
    zero = Polynomial();
  };
};

TEST_F(PolynomialTest, GTTest) {
  ASSERT_TRUE(v >= v);
  ASSERT_TRUE(v >= f);
  ASSERT_TRUE(v >= g);
  ASSERT_TRUE(v >= h);
  ASSERT_TRUE(v >= u);
  ASSERT_TRUE(v >= zero);

  ASSERT_TRUE(u >= u);
  ASSERT_TRUE(u >= f);
  ASSERT_TRUE(u >= g);
  ASSERT_TRUE(u >= h);
  ASSERT_TRUE(u >= zero);

  ASSERT_TRUE(f >= f);
  ASSERT_TRUE(f >= g);
  ASSERT_TRUE(f >= h);
  ASSERT_TRUE(f >= zero);

  ASSERT_TRUE(g >= g);
  ASSERT_TRUE(g >= zero);

  ASSERT_TRUE(h >= h);
  ASSERT_TRUE(h >= g);
  ASSERT_TRUE(h >= zero);

  ASSERT_TRUE(zero >= zero);
}

TEST_F(PolynomialTest, GreaterTest) {
  ASSERT_TRUE(v > f);
  ASSERT_TRUE(v > g);
  ASSERT_TRUE(v > h);
  ASSERT_TRUE(v > u);
  ASSERT_TRUE(v > zero);

  ASSERT_TRUE(u > f);
  ASSERT_TRUE(u > g);
  ASSERT_TRUE(u > h);
  ASSERT_TRUE(u > zero);

  ASSERT_TRUE(f > g);
  ASSERT_TRUE(f > h);
  ASSERT_TRUE(f > zero);

  ASSERT_TRUE(g > zero);

  ASSERT_TRUE(h > zero);
  ASSERT_TRUE(h > g);

  ASSERT_FALSE(zero > zero);
  ASSERT_FALSE(f > f);
  ASSERT_FALSE(g > g);
  ASSERT_FALSE(h > h);
  ASSERT_FALSE(u > u);
  ASSERT_FALSE(v > v);
}

TEST_F(PolynomialTest, LTTest) {
  ASSERT_TRUE(v <= v);
  ASSERT_TRUE(f <= v);
  ASSERT_TRUE(g <= v);
  ASSERT_TRUE(h <= v);
  ASSERT_TRUE(u <= v);
  ASSERT_TRUE(zero <= v);

  ASSERT_TRUE(u <= u);
  ASSERT_TRUE(f <= u);
  ASSERT_TRUE(g <= u);
  ASSERT_TRUE(h <= u);
  ASSERT_TRUE(zero <= u);

  ASSERT_TRUE(f <= f);
  ASSERT_TRUE(g <= f);
  ASSERT_TRUE(h <= f);
  ASSERT_TRUE(zero <= f);

  ASSERT_TRUE(g <= g);
  ASSERT_TRUE(zero <= g);

  ASSERT_TRUE(h <= h);
  ASSERT_TRUE(g <= h);
  ASSERT_TRUE(zero <= h);

  ASSERT_TRUE(zero <= zero);
}

TEST_F(PolynomialTest, LessTest) {
  ASSERT_TRUE(f < v);
  ASSERT_TRUE(g < v);
  ASSERT_TRUE(h < v);
  ASSERT_TRUE(u < v);
  ASSERT_TRUE(zero < v);

  ASSERT_TRUE(f < u);
  ASSERT_TRUE(g < u);
  ASSERT_TRUE(h < u);
  ASSERT_TRUE(zero < u);

  ASSERT_TRUE(g < f);
  ASSERT_TRUE(h < f);
  ASSERT_TRUE(zero < f);

  ASSERT_TRUE(zero < g);

  ASSERT_TRUE(g < h);
  ASSERT_TRUE(zero < h);

  ASSERT_FALSE(zero < zero);
  ASSERT_FALSE(f < f);
  ASSERT_FALSE(g < g);
  ASSERT_FALSE(h < h);
  ASSERT_FALSE(u < u);
  ASSERT_FALSE(v < v);
}

TEST_F(PolynomialTest, MulTest) {
  ASSERT_TRUE(f * zero == zero);
  Polynomial unit_poly({unit});
  ASSERT_TRUE(f * unit == f);
  ASSERT_TRUE(f * unit_poly == f);

  Polynomial a({z, unit});
  ASSERT_TRUE(a * y == g);

  Polynomial large_prod({6 * xyz, 6 * yz, 2 * xy, 2 * y});
  ASSERT_TRUE(large_prod == f * g);

  ASSERT_TRUE(f * (g * h) == (f * g) * h);
  ASSERT_TRUE(u * (v * h) == (u * v) * h);
  ASSERT_TRUE(f * g == g * f);
  ASSERT_TRUE(u * v == v * u);

  ASSERT_TRUE(2 * h == Polynomial({2 * x, 2 * unit}));

  a *= y;
  ASSERT_TRUE(a == g);
  a *= y;
  ASSERT_TRUE(a == g);
}

TEST_F(PolynomialTest, AddTest) {
  ASSERT_TRUE(f + f == 2 * f);
  ASSERT_TRUE(f + zero == f);
  ASSERT_TRUE(f + g == g + f);
  ASSERT_TRUE(u + v == v + u);
  ASSERT_TRUE(f + (g + h) == (f + g) + h);
  ASSERT_TRUE(u + (v + h) == (u + v) + h);

  ASSERT_TRUE(g + h == Polynomial({x, yz, y, unit}));
  ASSERT_TRUE(f + u == Polynomial({2 * xyz, 2 * xy, 2 * xz, 2 * yz, 3 * x,
                                   2 * y, 2 * z, 2 * unit}));

  ASSERT_TRUE(f + x == u);
  ASSERT_TRUE(v + xyz == Polynomial({6 * xyz}));

  ASSERT_TRUE(f - f == zero);
  ASSERT_TRUE(u - f == Polynomial({x}));
  ASSERT_TRUE(f - zero == f);
  ASSERT_TRUE(2 * g - g == g);
  ASSERT_TRUE(f + (-g) == f - g);
  ASSERT_TRUE(zero - f == -f);

  Polynomial a = f;
  a += x;
  ASSERT_TRUE(a == u);
  a -= x;
  ASSERT_TRUE(a == f);
  a -= Polynomial({xy, xz, yz, x, y, z, unit});
  ASSERT_TRUE(a == Polynomial({xyz}));
  a += 4 * xyz;
  ASSERT_TRUE(a == v);
}
TEST_F(PolynomialTest, DistTest) {
  ASSERT_TRUE(f * (v + g) == (f * v) + (f * g));
  ASSERT_TRUE((v + g) * f == (v * f) + (g * f));
  ASSERT_TRUE((v - v) * f == zero);
}

// f = Polynomial({xyz, xy, xz, yz, x, y, z, unit});
// g = Polynomial({yz, y});
// h = Polynomial({x, unit});
// u = Polynomial({xyz, xy, xz, yz, 2 * x, y, z, unit});
// v = Polynomial({5 * xyz});
// zero = Polynomial();

TEST_F(PolynomialTest, ReduceTest) {
  ASSERT_TRUE(g.can_lead_reduce(f));
  ASSERT_TRUE(f.lead_reducible_by(g));
  ASSERT_TRUE(g.can_reduce(f));
  ASSERT_TRUE(f.reducible_by(g));
  Polynomial a({x, yz});
  Polynomial b({y, unit});
  ASSERT_FALSE(a.lead_reducible_by(b));
  ASSERT_TRUE(a.reducible_by(b));
  ASSERT_DEATH(f.reducible_by(zero), "");

  a = f;
  g.lead_reduce(a);
  ASSERT_TRUE(a == Polynomial({xz, yz, x, y, z, unit}));

  a = f;
  f.lead_reduce(a);
  ASSERT_TRUE(a == zero);

  a = 2 * f;
  f.lead_reduce(a);
  ASSERT_TRUE(a == zero);

  a = 5 * f;
  b = 3 * f;
  b.lead_reduce(a);
  ASSERT_TRUE(a == zero);
}

/*----------------------------------------------------------*/

class IdealTest : public ::testing::Test {

public:
  Ideal I, C;
  /* x > y > z */
  const uint32_t x_var = 2;
  const uint32_t y_var = 1;
  const uint32_t z_var = 0;
  Monomial xyz, xy, xz, yz, x, y, z, unit;
  Polynomial spec;

protected:
  virtual void SetUp() {
    xyz = Monomial(Term({x_var, y_var, z_var}));
    xy = Monomial(Term({x_var, y_var}));
    xz = Monomial(Term({x_var, z_var}));
    yz = Monomial(Term({y_var, z_var}));
    x = Monomial(Term(x_var));
    y = Monomial(Term(y_var));
    z = Monomial(Term(z_var));
    unit = Monomial(Term());

    I.add_variable("z");
    I.add_variable("y");
    I.add_variable("x");

    C.add_variable("a0");
    C.add_variable("a1");
    C.add_variable("b0");
    C.add_variable("b1");
    C.add_variable("l1");
    C.add_variable("l2");
    C.add_variable("l3");
    C.add_variable("l5");
    C.add_variable("s0");
    C.add_variable("s1");
    C.add_variable("s2");
    C.add_variable("s3");

    spec = C.from_string("8*s3+4*s2+2*s1+s0-4*a1*b1-2*a0*b1-2*a1*b0-a0*b0");
  };
};

TEST_F(IdealTest, ParseTest) {
  Polynomial f = I.from_string("x");
  ASSERT_TRUE(f == Polynomial({x}));
  f = I.from_string("x + y + z");
  ASSERT_TRUE(f == Polynomial({x, y, z}));
  f = I.from_string("5*x*y*z + 14 * y - 3*x*y");
  ASSERT_TRUE(f == Polynomial({5 * xyz, 14 * y, (-3) * xy}));
  f = I.from_string("0");
  ASSERT_TRUE(f == Polynomial());
  f = I.from_string("-3x + 15y");
  ASSERT_TRUE(f == Polynomial({-3 * x, 15 * y}));
  f = I.from_string("5");
  ASSERT_TRUE(f == Polynomial({5 * unit}));
  f = I.from_string("-1");
  ASSERT_TRUE(f == Polynomial({-unit}));

  EXPECT_ANY_THROW(I.from_string("x*y*z+w"));
}

TEST_F(IdealTest, 2BitMultTest) {
  C.add_generator("-s0+a0*b0");
  C.add_generator("-l1+a1*b0");
  C.add_generator("-l2+a0*b1");
  C.add_generator("-l3+a1*b1");
  C.add_generator("-s1+l2+l1-2*l2*l1");
  C.add_generator("-l5+l1*l2");
  C.add_generator("-s2+l3+l5-2*l3*l5");
  C.add_generator("-s3+l5*l3");

  Polynomial rem = C.reduce(spec);
  ASSERT_TRUE(rem == C.zero());
}

TEST_F(IdealTest, 2BitMultTestBug) {
  C.add_generator("-s0+a0*b0");
  C.add_generator("-l1+a1*b0");
  C.add_generator("-l2+a0*b1");
  C.add_generator("-l3+a1*b1");
  C.add_generator("-s1+l2+l1-l2*l1"); // BUG! OR instead of XOR
  C.add_generator("-l5+l1*l2");
  C.add_generator("-s2+l3+l5-2*l3*l5");
  C.add_generator("-s3+l5*l3");

  Polynomial rem = C.reduce(spec);
  ASSERT_TRUE(rem != C.zero());
}
/*----------------------------------------------------------*/
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
