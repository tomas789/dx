#ifndef EXPR_HPP
#define EXPR_HPP

/*
 * BIG TODO : Check exception safety and add exception specifications
 */

#include <iostream>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <type_traits>
#include <cmath>

#include "traits.hpp"

namespace ex {

class tree_type;

/**
 * Virtual super-class for expression tree
 *
 * All important types and functions for outside users are 
 * defined here.
 */
class expr {
public:

	/* It might be better to use policy class instead */
	//using ptr_type = std::unique_ptr<expr>;
    typedef tree_type ptr_type;
    typedef std::string string_type;
    typedef double eval_type;
    typedef std::vector<eval_type> valuation_type;
    typedef std::size_t size_type;

	virtual ptr_type derive(const valuation_type::size_type& var) = 0;
	virtual ptr_type clone() = 0;
	virtual string_type to_string() = 0;
	virtual eval_type eval(const valuation_type& values) = 0;
    
    virtual size_type arity() = 0;
    virtual ptr_type & operator[] (size_type n) = 0;
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
 *
 * TODO : Instead of inheriting unique_ptr consider using it as 
 *        member variable
 */
class tree_type {
    std::unique_ptr<expr> e_;
public:
	constexpr tree_type() = default;
    explicit tree_type(expr * p);
    tree_type(tree_type && t);
    tree_type(const tree_type & t);

    tree_type & operator= (const tree_type & t);
    tree_type & operator= (tree_type && t);

    bool empty() const;
    expr::size_type arity() const;
    tree_type & operator[] (expr::size_type n);
    std::unique_ptr<expr> & operator-> ();
    const std::unique_ptr<expr> & operator-> () const;
    
	/* no destructor required */
};

/**
 * Child (subtree) container.
 *
 * TODO : What is more appropriete polymorphic hierarchy?
 *           +------+    +-------+    +----------+
 *           | expr | <- | n_ary | <- | function |
 *           +------+    +-------+    +----------+
 *
 *                          OR
 *
 *           +------+    +----------+
 *           | expr | <- | function |
 *           +------+    +----------+
 *                        /
 *           +-------+   /
 *           | n_ary | <-
 *           +-------+
 */
template <std::size_t Arity>
class n_ary : public expr {
protected:
	std::array<expr::ptr_type, Arity> childs;
    template <class T, class U> friend class function;
public:
    expr::size_type arity() {
        return Arity;
    }

    expr::ptr_type & operator[] (expr::size_type n) {
        return childs[n];
    }

    template <class T, class ... TList>
    void move_to(T && t, TList && ... plist) {
        childs[childs.size() - sizeof...(plist) - 1] = std::move(t);
        move_to(std::move(plist)...);
    }

    template <class T>
    void move_to(T && t) {
        childs[childs.size() - 1] = std::move(t);
    }
};

/**
 * Every function type must be derived from this class
 */
struct function_base { };

struct variable : public function_base { 
	static const std::size_t arity = 0;
    typedef expr::valuation_type::size_type value_type;
	value_type value;
};

struct constant : public function_base { 
	static const std::size_t arity = 0; 
    typedef expr::eval_type value_type;
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

/**
 * Function class with all functionality
 * 
 * Template parameter must be derived from function_base class - this makes
 * more human-readable compiler errors when invalid template parameter is
 * supplied.
 *
 * TODO : Conditionally derive from n_ary only when T::arity != 0 (?)
 * TODO : Leverage generic design and make generic to_string and eval
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
    
    /**
     * Parameterless constructor 
     */
    function() {
    }

    /**
     * Construct using value
     * Fro nullary functions only (T::arity == 0)
     */
	template <
            typename U = T, 
            typename std::enable_if<
                    has_value<U>::value
                >::type ...
        >
	function(const typename U::value_type& v) { 
        U::value = v; 
    }

    /**
     * Initialize using childrens
     * For non-terminals onyl (T::arity != 0)
     */
	template<
			typename... Tree, 
			typename std::enable_if<
					sizeof...(Tree) == T::arity && 
					are_same<expr::ptr_type, Tree...>::value
				>::type...
		>
	explicit function(Tree && ... tlist) {
        n_ary<T::arity>::move_to(std::move(tlist)...);
    }

    /** 
     * Copy constructor 
     */
    explicit function(const function<T>& a) : T(static_cast<T>(a)) { 
		for (std::size_t i = 0; i < T::arity; ++i)
			n_ary<T::arity>::childs[i] = std::move(a.childs[i]->clone());
	};

	/**
     * Implicit type cast to tree_type
     */
    operator expr::ptr_type() const { 
        return ptr_type(this); 
    }

    expr::ptr_type derive(const expr::valuation_type::size_type& var);
	expr::string_type to_string();
	expr::eval_type eval(const expr::valuation_type& values);

    /**
     * Virtual copy constructor using actual copy constructor
     */
    expr::ptr_type clone() {
		return expr::ptr_type(new function<T>(*this));
	}
};

/**
 * Construct terminal functions by it's value
 * TODO : NOT TESTED
 */
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

//template <> template <> 
//inline function<variable>::function(const variable::value_type& vnum)
//{ variable::value = vnum; }

template <> 
inline expr::ptr_type function<variable>::derive(
    const expr::valuation_type::size_type& n)
{ return ptr_type(new function<constant>(n == variable::value ? 1 : 0)); }

template <> inline expr::string_type function<variable>::to_string()
{ return "x_" + std::to_string(variable::value); }

template <> 
inline expr::eval_type function<variable>::eval(
    const expr::valuation_type& values)
{ return values[variable::value]; }

/*******************************************************
 ***** function<constant> template specialisations *****
 *******************************************************/

//template <> template <> 
//inline function<constant>::function(const constant::value_type& val)
//{ constant::value = val; }

template <> 
inline expr::ptr_type function<constant>::derive(
    const expr::valuation_type::size_type&) 
{ return ptr_type(new function<constant>(0)); }

template <> inline expr::string_type function<constant>::to_string() 
{ return std::to_string(constant::value); }

template <> 
inline expr::eval_type function<constant>::eval(const expr::valuation_type&) 
{ return constant::value; }

/*******************************************************
 *****   function<sin> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<struct sin>::derive(
        const expr::valuation_type::size_type& var) {
    auto *p = new function<struct cos>;
    p->childs[0] = std::move(n_ary::childs[0]->clone());
    auto *m = new function<mul>;
    m->childs[0] = std::move(ptr_type(p));
    m->childs[1] = std::move(n_ary::childs[0]->derive(var));
    return ptr_type(m);
}

template <> inline expr::string_type function<struct sin>::to_string()
{ return "sin(" + n_ary::childs[0]->to_string() + ")"; }

template <>
inline expr::eval_type function<struct sin>::eval(
        const expr::valuation_type& val)
{ return std::sin(n_ary::childs[0]->eval(val)); }

/*******************************************************
 *****   function<cos> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<struct cos>::derive(
        const expr::valuation_type::size_type& var) {
    auto *s = new function<struct sin>;
    s->childs[0] = std::move(n_ary::childs[0]->clone());
    auto *p = new function<mul>;
    p->childs[0] = std::move(ptr_type(s));
    p->childs[1] = std::move(n_ary::childs[0]->derive(var));
    auto *m = new function<mul>;
    m->childs[0] = std::move(ptr_type(new function<constant>(-1)));
    m->childs[1] = std::move(ptr_type(p));
    return ptr_type(m);
}

template <> inline expr::string_type function<struct cos>::to_string()
{ return "cos(" + n_ary::childs[0]->to_string() + ")"; }

template <>
inline expr::eval_type function<struct cos>::eval(
        const expr::valuation_type& val)
{ return std::cos(n_ary::childs[0]->eval(val)); }

/*******************************************************
 *****   function<tan> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<struct tan>::derive(
		const expr::valuation_type::size_type& var) {
	auto n5 = new function<mul>;
	n5->childs[0] = std::move(ptr_type(new function<constant>(2)));
	n5->childs[1] = std::move(childs[0]->clone());
	auto n4 = new function<struct cos>;
	n4->childs[0] = std::move(ptr_type(n5));
	auto n3 = new function<add>;
	n3->childs[0] = std::move(ptr_type(n4));
	n3->childs[1] = std::move(ptr_type(new function<constant>(1)));
	auto n2 = new function<mul>;
	n2->childs[0] = std::move(ptr_type(new function<constant>(2)));
	n2->childs[1] = std::move(childs[0]->derive(var));
	auto n1 = new function<struct div>;
	n1->childs[0] = std::move(ptr_type(n2));
	n1->childs[1] = std::move(ptr_type(n3));
	return ptr_type(n1);
}

template <> inline expr::string_type function<struct tan>::to_string()
{ return "tan(" + childs[0]->to_string() + ")"; }

template <>
inline expr::eval_type function<struct tan>::eval(
		const expr::valuation_type& val)
{ return std::tan(childs[0]->eval(val)); }

/*******************************************************
 *****   function<log> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<struct log>::derive(
        const expr::valuation_type::size_type& var) {
    auto *p = new function<struct div>;
    p->childs[0] = std::move(n_ary::childs[0]->derive(var));
    p->childs[1] = std::move(n_ary::childs[0]->clone());
    return ptr_type(p);
}

template <> inline expr::string_type function<struct log>::to_string()
{ return "log(" + n_ary::childs[0]->to_string() + ")"; }

template <>
inline expr::eval_type function<struct log>::eval(const expr::valuation_type& val)
{ return std::log(n_ary::childs[0]->eval(val)); }

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
{ return "(" + n_ary::childs[0]->to_string() + "+" + n_ary::childs[1]->to_string() + ")"; }

template <> 
inline expr::eval_type function<add>::eval(const expr::valuation_type& val) 
{ return n_ary::childs[0]->eval(val) + n_ary::childs[1]->eval(val); }

/*******************************************************
 *****   function<sub> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<sub>::derive(
        const expr::valuation_type::size_type& var) {
    
	auto *p = new function<sub>(
		operator[](0)->derive(var),
		operator[](1)->derive(var));

    return ptr_type(p);
}

template <> inline expr::string_type function<sub>::to_string()
{ return "(" + operator[](0)->to_string() + "-" + operator[](1)->to_string() + ")"; }

template <>
inline expr::eval_type function<sub>::eval(const expr::valuation_type& val)
{ return n_ary::childs[0]->eval(val) - n_ary::childs[1]->eval(val); }

/*******************************************************
 *****   function<mul> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<mul>::derive(
        const expr::valuation_type::size_type& var) {

    auto *add_lhs = new function<mul>(
		operator[](0)->derive(var),
		operator[](1)->clone());

    auto *add_rhs = new function<mul>(
		ptr_type(operator[](0)->clone()),
		ptr_type(operator[](1)->derive(var)));

	auto res = new function<add>(ptr_type(add_lhs), ptr_type(add_rhs));

    return ptr_type(res);
}

template <> inline expr::string_type function<mul>::to_string()
{ return "(" + operator[](0)->to_string() + "*" + operator[](1)->to_string() + ")"; }

template <>
inline expr::eval_type function<mul>::eval(const expr::valuation_type& val)
{ return operator[](0)->eval(val) * operator[](1)->eval(val); }

/*******************************************************
 *****   function<div> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<struct div>::derive(
		const expr::valuation_type::size_type& var) {
	auto n5 = new function<struct mul>;
	n5->childs[0] = std::move(childs[0]->clone());
	n5->childs[1] = std::move(childs[1]->derive(var));
	auto n4 = new function<struct mul>;
	n4->childs[0] = std::move(childs[1]->clone());
	n4->childs[1] = std::move(childs[0]->derive(var));
	auto n3 = new function<struct pow>;
	n3->childs[0] = std::move(childs[1]->clone());
	n3->childs[1] = std::move(ptr_type(new function<constant>(2)));
	auto n2 = new function<struct sub>;
	n2->childs[0] = std::move(ptr_type(n4));
	n2->childs[1] = std::move(ptr_type(n5));
	auto n1 = new function<struct div>;
	n1->childs[0] = std::move(ptr_type(n2));
	n1->childs[1] = std::move(ptr_type(n3));
	return ptr_type(n1);
}

template <> inline expr::string_type function<struct div>::to_string()
{ return "(" + childs[0]->to_string() + "/" + childs[1]->to_string() + ")"; }

template <>
inline expr::eval_type function<struct div>::eval(const expr::valuation_type& val)
{ return childs[0]->eval(val) / childs[1]->eval(val); }

/*******************************************************
 *****   function<pow> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<struct pow>::derive(
		const expr::valuation_type::size_type& var) {
	auto n8 = new function<struct log>;
	n8->childs[0] = std::move(childs[0]->clone());
	auto n7 = new function<struct mul>;
	n7->childs[0] = std::move(ptr_type(n8));
	n7->childs[1] = std::move(childs[1]->derive(var));
	auto n6 = new function<struct mul>;
	n6->childs[0] = std::move(childs[0]->derive(var));
	n6->childs[1] = std::move(ptr_type(n7));
	auto n5 = new function<struct mul>;
	n5->childs[0] = std::move(childs[1]->clone());
	n5->childs[1] = std::move(childs[0]->derive(var));
	auto n4 = new function<struct sub>;
	n4->childs[0] = std::move(childs[1]->clone());
	n4->childs[1] = std::move(ptr_type(new function<constant>(1)));
	auto n3 = new function<struct add>;
	n3->childs[0] = std::move(ptr_type(n5));
	n3->childs[1] = std::move(ptr_type(n6));
	auto n2 = new function<struct pow>;
	n2->childs[0] = std::move(childs[0]->clone());
	n2->childs[1] = std::move(ptr_type(n4));
	auto n1 = new function<struct mul>;
	n1->childs[0] = std::move(ptr_type(n2));
	n1->childs[1] = std::move(ptr_type(n3));
	return ptr_type(n1);
}

template <> inline expr::string_type function<struct pow>::to_string()
{ return "(" + childs[0]->to_string() + "^" + childs[1]->to_string() + ")"; }

template <>
inline expr::eval_type function<struct pow>::eval(const expr::valuation_type& val)
{ return std::pow(childs[0]->eval(val), childs[1]->eval(val)); }

tree_type make_variable(variable::value_type vnum);
tree_type make_constant(constant::value_type c);

} // end namespace ex

#endif
