#ifndef EXPR_HPP
#define EXPR_HPP

#include <iostream>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <type_traits>

/**
 * Virtual super-class for expression tree
 *
 * All important types and functions for outside users are 
 * defined here.
 */
class expr {
public:

	/* It might be beter to use policy class instead */
	using ptr_type = std::unique_ptr<expr>;
	using string_type = std::string;
	using eval_type = double;
	using valuation_type = std::vector<eval_type>;

	virtual ptr_type derive(const valuation_type::size_type& var) = 0;
	virtual ptr_type clone() = 0;
	virtual string_type to_string() = 0;
	virtual eval_type eval(const valuation_type& values) = 0;
};

/**
 * Type of expression tree. 
 *
 * Tree is basically pointer. This helps to keep track of tree
 * ownership. When you have tree_type - you own pointer otherwise
 * when you have tree_type::element_type you do NOT own this tree.
 * So you shouldn't (musn't) explicitly delete it.
 *
 * The class satisfies the requirements of MoveConstructible and 
 * MoveAssignable, but not the requirements of either 
 * CopyConstructible or CopyAssignable.
 */
using tree_type = std::unique_ptr<expr>;

/**
 * Child (subtree) container.
 *
 * TODO : What is more appropriete polymorphic hierarchy?
 * +------+    +-------+    +----------+
 * | expr | <- | n_ary | <- | function |
 * +------+    +-------+    +----------+
 *
 *               OR
 *
 * +------+    +----------+
 * | expr | <- | function |
 * +------+    +----------+
 *              /
 * +-------+   /
 * | n_ary | <-
 * +-------+
 */
template <std::size_t Arity>
class n_ary : public expr {
protected:
	std::array<expr::ptr_type, Arity> childs;

    template <class T, class U> friend class function;
};

/**
 * Every function type must be derived from this class
 */
struct function_base { };

struct variable : public function_base { 
	constexpr static std::size_t arity = 0;
    typedef expr::valuation_type::size_type value_type;
	value_type var_num;
};

struct constant : public function_base { 
	constexpr static std::size_t arity = 0; 
    typedef expr::eval_type value_type;
	value_type c;
};

struct sin : public function_base { constexpr static std::size_t arity = 1; };
struct cos : public function_base { constexpr static std::size_t arity = 1; };
struct tan : public function_base { constexpr static std::size_t arity = 1; };
struct log : public function_base { constexpr static std::size_t arity = 1; };

struct add : public function_base { constexpr static std::size_t arity = 2; };
struct sub : public function_base { constexpr static std::size_t arity = 2; };
struct mul : public function_base { constexpr static std::size_t arity = 2; };
struct div : public function_base { constexpr static std::size_t arity = 2; };

/**
 * Compile time type property deduction
 * Specialisation for specific templates should be sufficient but this is
 * more elegant and general
 */
template <typename T>
class has_value {
    typedef char yes[1];
    typedef char no[2];

    template <typename U>
    static yes& test(typename U::value_type*);
    template <typename U>
    static no& test(...);

public:
    enum { value = (sizeof(yes) == sizeof(test<T>(0))) };
};

/**
 * Function class with all functionality
 * 
 * Template parameter must be derived from function_base class - this makes
 * more human-readable compiler errors when invalid template parameter is
 * supplied.
 *
 * TODO : Conditionally derive from n_ary only when T::arity != 0
 */
template <class T, class Enable = void>
class function;

template <class T>
class function<
    T, 
    typename std::enable_if<
        std::is_base_of<
            function_base, 
            T
        >::value 
    >::type 
>
  : public T, public n_ary<T::arity> {
public:
	/**
	 * Constructor for functions with internal value
	 * Defined only for functions with value_type type
	 *
	 * TODO : Is possible to simply pass more then one parameter
	 *        without additional encapsulation (pair, tuple, vector, ...)?
	 * TODO : define template <class T> function<T>::operator std::string();
	 */
	template <
            typename U = T, 
            typename std::enable_if<
                    has_value<U>::value
                >::type ...
        >
	function(const typename U::value_type& value);

    function() {};	
    explicit function(const function<T>&) { };

	virtual expr::ptr_type derive(const expr::valuation_type::size_type& var);
	
	virtual expr::ptr_type clone() {
		return expr::ptr_type(new function<T>(*this));
	};

	virtual expr::string_type to_string();
	virtual expr::eval_type eval(const expr::valuation_type& values);

};

template <
        class T, 
        class = typename std::enable_if<
                std::is_base_of<
                        function_base, 
                        T
                    >::value 
            >::type 
    >
expr::ptr_type construct(typename T::value_type v) {
	return expr::ptr_type(nullptr);
}

/**
 * Required prototypes
 */
//template <> template <> 
//inline function<constant>::function(const constant::value_type& val);

//template <> template <>
//inline function<variable>::function(const variable::value_type& val);

/*******************************************************
 ***** function<variable> template specialisations *****
 *******************************************************/

template <> template <> 
inline function<variable>::function(const variable::value_type& vnum)
{ variable::var_num = vnum; }

template <> 
inline expr::ptr_type function<variable>::derive(
    const expr::valuation_type::size_type& n)
{ return ptr_type(new function<constant>(n == var_num ? 1 : 0)); }

template <> inline expr::string_type function<variable>::to_string()
{ return "x_" + std::to_string(variable::var_num); }

template <> 
inline expr::eval_type function<variable>::eval(
    const expr::valuation_type& values)
{ return values[variable::var_num]; }

/*******************************************************
 ***** function<constant> template specialisations *****
 *******************************************************/

template <> template <> 
inline function<constant>::function(const constant::value_type& val)
{ constant::c = val; }

template <> 
inline expr::ptr_type function<constant>::derive(
    const expr::valuation_type::size_type&) 
{ return ptr_type(new function<constant>(0)); }

template <> inline expr::string_type function<constant>::to_string() 
{ return std::to_string(constant::c); }

template <> 
inline expr::eval_type function<constant>::eval(const expr::valuation_type&) 
{ return constant::c; }

/*******************************************************
 *****   function<sin> template specialisations    *****
 *******************************************************/

/*******************************************************
 *****   function<cos> template specialisations    *****
 *******************************************************/

/*******************************************************
 *****   function<tan> template specialisations    *****
 *******************************************************/

/*******************************************************
 *****   function<log> template specialisations    *****
 *******************************************************/

/*******************************************************
 *****   function<add> template specialisations    *****
 *******************************************************/

template <> 
inline expr::ptr_type function<add>::derive(
        const expr::valuation_type::size_type& var) { 
	auto *p = new function<add>;
	p->childs[0] = std::move(n_ary::childs[0]->derive(var));
	p->childs[1] = std::move(n_ary::childs[1]->derive(var));
	return ptr_type(p);
}

template <> inline expr::string_type function<add>::to_string()
{ return n_ary::childs[0]->to_string() + "+" + n_ary::childs[1]->to_string(); }

template <> 
inline expr::eval_type function<add>::eval(const expr::valuation_type& val) 
{ return n_ary::childs[0]->eval(val) + n_ary::childs[1]->eval(val); }

/*******************************************************
 *****   function<sub> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<sub>::derive(
        const expr::valuation_type::size_type& var) {
    auto *p = new function<sub>;
    p->childs[0] = std::move(n_ary::childs[0]->derive(var));
    p->childs[1] = std::move(n_ary::childs[1]->derive(var));
    return ptr_type(p);
}

template <> inline expr::string_type function<sub>::to_string()
{ return n_ary::childs[0]->to_string() + "-" + n_ary::childs[1]->to_string(); }

template <>
inline expr::eval_type function<sub>::eval(const expr::valuation_type& val)
{ return n_ary::childs[0]->eval(val) - n_ary::childs[1]->eval(val); }

/*******************************************************
 *****   function<mul> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<mul>::derive(
        const expr::valuation_type::size_type& var) {
    auto *add_lhs = new function<mul>;
    add_lhs->childs[0] = std::move(n_ary::childs[0]->derive(var));
    add_lhs->childs[1] = std::move(n_ary::childs[1]->clone());
    auto *add_rhs = new function<mul>;
    add_rhs->childs[0] = std::move(n_ary::childs[0]->clone());
    add_rhs->childs[1] = std::move(n_ary::childs[1]->derive(var));
    auto *res = new function<add>;
    res->childs[0] = std::move(ptr_type(add_lhs));
    res->childs[1] = std::move(ptr_type(add_rhs));
    return ptr_type(res);
}

template <> inline expr::string_type function<mul>::to_string()
{ return n_ary::childs[0]->to_string() + "*" + n_ary::childs[1]->to_string(); }

template <>
inline expr::eval_type function<mul>::eval(const expr::valuation_type& val)
{ return n_ary::childs[0]->eval(val) * n_ary::childs[1]->eval(val); }

/*******************************************************
 *****   function<div> template specialisations    *****
 *******************************************************/

#endif
