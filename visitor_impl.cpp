#include "visitor_impl.hpp"

#include <boost/lexical_cast.hpp>

#include "function.hpp"

namespace ex {

std::string printer_visitor::recursive(ex::expr & e, std::size_t num) {
    ex::generic_visitor<printer_visitor> v;
    return boost::any_cast<std::string>(
            e[num]->accept(v)
        );
}

boost::any printer_visitor::visit(ex::expr & e) {
    return std::string("general_expression");
}

boost::any printer_visitor::visit(ex::function<ex::base::variable> & c) {
    return c.value;
}

boost::any printer_visitor::visit(ex::function<ex::base::constant> & c) {
    return boost::lexical_cast<std::string>(c.value);
}

boost::any printer_visitor::visit(ex::function<ex::base::sin> & c) {
    return std::string("sin(") + recursive(c, 0) + std::string(")");
}

boost::any printer_visitor::visit(ex::function<ex::base::cos> & c) {
    return std::string("cos(") + recursive(c, 0) + std::string(")");
}

boost::any printer_visitor::visit(ex::function<ex::base::tan> & c) {
    return std::string("tan(") + recursive(c, 0) + std::string(")");
}

boost::any printer_visitor::visit(ex::function<ex::base::log> & c) {
    return std::string("log(") + recursive(c, 0) + std::string(")");
}

boost::any printer_visitor::visit(ex::function<ex::base::add> & c) {
    return std::string("(" + recursive(c, 0) + "+" + recursive(c, 1) + ")");
}

boost::any printer_visitor::visit(ex::function<ex::base::sub> & c) {
    return std::string("(" + recursive(c, 0) + "-" + recursive(c, 1) + ")");
}

boost::any printer_visitor::visit(ex::function<ex::base::mul> & c) {
    return std::string("(" + recursive(c, 0) + "*" + recursive(c, 1) + ")");
}

boost::any printer_visitor::visit(ex::function<ex::base::div> & c) {
    return std::string("(" + recursive(c, 0) + "/" + recursive(c, 1) + ")");
}

boost::any printer_visitor::visit(ex::function<ex::base::pow> & c) {
    return std::string("(" + recursive(c, 0) + "^" + recursive(c, 1) + ")");
}

}


