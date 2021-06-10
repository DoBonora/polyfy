#include <gtest/gtest.h>

#include <cstdint>
#include <gmpxx.h>

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
}

TEST_F(MonomialTest, ZeroTest) {
  Monomial zero(0, Term());
  ASSERT_TRUE(zero.is_zero());
  ASSERT_FALSE(x.is_zero());
  ASSERT_FALSE(unit.is_zero());
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


    // f = Polynomial({xyz, xy, xz, yz, x, y, z, unit});
    // g = Polynomial({yz, y});
    // h = Polynomial({x, unit});
    // u = Polynomial({xyz, xy, xz, yz, 2 * x, y, z, unit});
    // v = Polynomial({5 * xyz});
    // zero = Polynomial();

TEST_F(PolynomialTest, MulTest) {
  ASSERT_TRUE(f*zero == zero);
  Polynomial unit_poly({unit});
  ASSERT_TRUE(f*unit == f);
  ASSERT_TRUE(f*unit_poly == f);
  
  Polynomial a({z, unit});
  ASSERT_TRUE(a*y == g);

  Polynomial large_prod({6*xyz, 6*yz, 2*xy, 2*y});
  ASSERT_TRUE(large_prod == f*g);

  ASSERT_TRUE(f*(g*h) == (f*g)*h);
  ASSERT_TRUE(u*(v*h) == (u*v)*h);
  ASSERT_TRUE(f*g == g*f);
  ASSERT_TRUE(u*v == v*u);
}
/*----------------------------------------------------------*/
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
