DX - Genetic Programming Toolbox
================================

DX is toolbox for simple genetic programming tasks.

It is written in C++ and leverages from use of C++11. Main goal is to provide simple yet powerful tool for genetic programming representing each individual as interpreted computer program.

Contributors
------------

* [Tomas Krejci](mailto:tomas789@gmail.com) , [Charles University in Prague, Czech Republic](http://www.cuni.cz/UKEN-1.html)

How To Install
--------------

It is shipped with `Makefile` which is tested with GNU Make 3.81. To compile
program simply call

    make

It will compile and produce executable `dx` which can be then used without
further restrictions and no dynamic dependencies.

C++11
-----

* Smart Pointers
* Move Semantics
* std::array

Supported Compilers
-------------------

List of supported compilers is restricted especially by it's support of C++11.
Some tested compilers are:

* GCC 4.9.0 20130915 (experimental)
* GCC 4.8.1
* GCC 4.7.3
* GCC 4.6.4
* Apple LLVM version 5.0 (clang-500.2.76) (based on LLVM 3.3svn)

Microsoft Visual C++ is currently NOT supported.

