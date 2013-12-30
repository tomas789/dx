#ifndef FUNCTION_BASE_HPP
#define FUNCTION_BASE_HPP

#include "globals.hpp"

namespace ex { namespace base {
    /**
     * Every function type must be derived from this class
     */
    struct function_base { };

    struct variable : public function_base { 
        static const std::size_t arity = 0;
        typedef globals::size_type value_type;
        value_type value;
    };

    struct constant : public function_base { 
        static const std::size_t arity = 0; 
        typedef globals::eval_type value_type;
        value_type value;
    };

    struct sin : public function_base { static const std::size_t arity = 1; };
    struct cos : public function_base { static const std::size_t arity = 1; };
    struct tan : public function_base { static const std::size_t arity = 1; };
    struct log : public function_base { static const std::size_t arity = 1; };

    struct add : public function_base { static const std::size_t arity = 2; };
    struct sub : public function_base { static const std::size_t arity = 2; };
    struct mul : public function_base { static const std::size_t arity = 2; };
    struct div : public function_base { static const std::size_t arity = 2; };
    struct pow : public function_base { static const std::size_t arity = 2; };

    template <typename ... TList>
    struct typelist { };

    typedef typelist<
            variable, constant,
            sin, cos, tan, log,
            add, sub, mul, div, pow
        > symbols;



} }


#endif
