DX - Symbolic Derivation Toolbox
================================

DX is small program used for symbolic derivation of given math expression.

It is written in C++ and leverages from use of C++11. Main goal is to provide simple
yet powerful tool for symbolic derivation. It is proof-of-concept and will in the 
future transform to Genetic Programming library.

Comtributors
------------

 o Tomas Krejci
   - Charles University in Prague, Czech Republic
   - tomas789 -at- gmail -dot- com

How To Install
--------------

It is shipped with `Makefile` which is tested with GNU Make 3.81. To compile
program simply call

```bash
make
```

It will compile and produce executable `dx` which can be then used without
further restrictions and no dynamic dependencies.

C++11
-----

 o Smart Pointers
 o Move Semantics
 o std::array

Supported Compilers
-------------------

List of supported compilers is restricted especially by it's support of C++11.
Some tested compilers are:

 o GCC 4.9.0 20130915 (experimental)
 o GCC 4.8.1
 o GCC 4.7.3
 o GCC 4.6.4
 o Apple LLVM version 5.0 (clang-500.2.76) (based on LLVM 3.3svn)

Microsoft Visual C++ is currently NOT supported.

Workflow
--------

 1/ Read expression from standard input (stdin).
 
 2/ Pass expression as an argument to the function 
        `expression parse(const std::string &)`
 
 3/ Result of `parse` function is AST of given expression

 4/ Call function expression 
        `expression::derive(const std::string &)`

 5/ Print result to standard output (stdout)


Internals
---------

### Parser

As a parser there is used Recursice Descent parsing (http://en.wikipedia.org/wiki/Recursive_descent_parser). Whole parser code is defined in class exparser (defined in exparser.hpp and implemented exparser.cpp).

It loops through set of possible variant and try to match given input to parser definition. It returns tuple of bool (was parsing successful) and value of parsed input - object of expression type.

 o Expression Representation
 o Expression class functionality
 o How to extend library

TODO
----

 o Support for GP (genetic programming)
 o Extendability
 o Symbol traits
