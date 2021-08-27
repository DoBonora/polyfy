
# Table of Contents

1.  [General Information](#org5163f9b)
    1.  [Hardware Verification](#org1853940)
    2.  [SCA](#org52e402b)
2.  [Usage](#orgd3dab02)
    1.  [System Requirements](#org6db893f)
    2.  [Build and Run](#org5f4b5e8)
3.  [Implementation Details and Design Decisions](#orge778ccb)
    1.  [Term](#org401ca80)
    2.  [Monomial](#orgb83cd6c)
    3.  [Polynomial](#org91c1487)

Developer: Tom Peham


<a id="org5163f9b"></a>

# General Information

This tool was developed during the Masters course *Hardware Design* at the *Johannes Kepler University*. It implements
the basic method of circuit verification by means of *Symbolic Computer Algebra* (SCA). The interested reader is
encouraged to study the [relevant literature](http://www.informatik.uni-bremen.de/agra/sca-verification/index.html) for more details. This implementation doesn't apply any advanced
techniques necessary to verify complicated multiplier circuits.


<a id="org1853940"></a>

## Hardware Verification

Hardware verification is the task of proving that a given circuit correctly implements a given specification. As this is
equivalent to the task of identifying the equality of two boolean functions this problem NP-complete. There are types of
circuits (adder circuits for example) that modern SAT solvers can verify efficiently. But SAT solvers usually fail when
it comes to multiplier circuits.


<a id="org52e402b"></a>

## SCA

For multiplier circuits it turns out that verfication using SCA is often quite efficient. For this method the circuit
implementation and the specification are interpreted as polynomials and a monomial ordering is defined on the monomials
over the variables. Using Gröbner Basis theory one can prove that a
circuit is correct if and only if the reduction of the specification polynomial by the circuit polynomials yields a
remainder of 0.


<a id="orgd3dab02"></a>

# Usage


<a id="org6db893f"></a>

## System Requirements

The program has been tested under Linux (Ubuntu 18.04, 64-bit) and should be compatible with any current version of g++
supporting C++17 and a minimum CMake version of 3.14.
The project also requires the GNU Multiple Precision Arithmetic Library. It can be installed with `apt` using the
following command:

    sudo apt install libgmp3-dev


<a id="org5f4b5e8"></a>

## Build and Run

1.  Configure CMake:
    `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`

2.  Build the respective target:
    `cmake --build ./build --config Release`

`polyfy` accepts multiplier circuits given in the [aiger](http://fmv.jku.at/aiger/) format. The current version of `polyfy` expects
that the two input operands of the multiplier circuit have the same bit-width.

To use the verifier run

    ./polyfy <circtui>.aig


<a id="orge778ccb"></a>

# Implementation Details and Design Decisions

The main SCA routine is the [`reduce`](https://github.com/pehamTom/polyfy/blob/master/src/Ideal.cpp#L105) method. The reduction is performed by iterating over the
circuit polynomials and reducing the leading term of the current specification polynomial. More specifically if `f`
is the current specification polynomial and `g` is the generator with leading monomial `+/-x` for some variable `x` then we update `f` with
`f-lm(f)/x*g` (or `f+lm(f)/x*g` depending on the sign of the leading coefficient of `g`) where `lm(f)` is the leading
monomial of f. This simple reduction is possible because all circuit polynomials are guaranteed to be linear lead polynomials,
meaning that the the leading monomial of any circuit polynomials is of the form `+/-x` for some variable `x`. Updating
`f` this way ensures that the leading term cancels.

Profiling shows that most of the run time during the reduction is spent in the [`+=`](https://github.com/pehamTom/polyfy/blob/master/src/Polynomial.cpp#L73) method of the
`Polynomial` class.


<a id="org401ca80"></a>

## Term

A term is a power product of circuit variables. Since the variables can only take on values of **0** or **1**, the relation
`x^2=x` holds for every variable `x`. Therefore any variable in a term can only have exponent **0** or **1**. This gives a
term the same semantics as an ordered set since any variable can either be in the term or not. This suggests
implementing a term using a `std::set`. Indeed this was the initial implementation. Profiling showed however that a
significant portion of the run time of the reduction algorithm was spent in comparing terms. This is because `std::set`
is usually implemented as a Red-Black tree. Although Red-Black trees are nice for asymptotic guarantees, they do not
store values in a cache-friendly manner. The significant number of cache-misses was the reason why the comparison
operations were such a strong performance draw. Simply implementing a term as a `std::vector` and ensuring set-like
semantics turned out to be orders of magnitude faster.


<a id="orgb83cd6c"></a>

## Monomial

A monomial is a term together with a coefficient. Since coefficients can get very large during SCA the coefficients
where implemented using the well-known **GMP** library.


<a id="org91c1487"></a>

## Polynomial

A polynomial is an ordered collection of monomials. There are two obvious choices for implementing a polynomial: `std::vector`
or `std::list`. Adding a monomial to a polynomial requires traversal of the polynomial to find the position where the
monomial is either inserted or added to an existing monomial with the same term. Thus when adding two polynomials we
have to repeatedly perform these insertions (or also deletions when two monomials cancel) and additions.

The data structure used for the implementation of the polynomial should thefore be fast at insertions and deletions (a
perfect application for doubly linked list) and at traversals (a good application for `std::vector` because of
cache-locality).

Through profiling we found that `std::vector` again wins the performance race. Although deletions and
insertions in a `std::vector` require moving all elements beyond the insertion position, in SCA most insertions usually
happen at the beginning of the polynomial. This is because the variables are topologically ordered and gate polynomials
usually (but not always!) do not relate variables that are far apart in the topological ordering. Storing the monomials
in the vector such that large monomials (with respect to the monomial ordering) are at the end of the vector ensures
that (on average) not many monomials have to be copied when deleting or inserting.

Another benefit of having a sorted `std::vector` is the possibility of finding insertion/addition positions for
monomials via binary search. Since the gate polynomials are usually small (5 monomials max), an addition can be
performed with very few binary searches. Although this is not perfect for cache-locality the logarithmic insertion gives
a huge performance boost.

