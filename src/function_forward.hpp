#ifndef FUNCTION_FORWARD_HPP
#define FUNCTION_FORWARD_HPP

namespace ex {

/** \brief Implementation of basic tree functionality
 *
 *  Forward declaration of ex::function class. It should be used
 *  whenever you need to forward-declare this class. It will guarantee
 *  there is only one forward declaration because of default template
 *  parameter value.
 *
 *  \tparam T class derived from \ref ex::base::function_base
 */
template <typename T, class Enable = void>
class function;

};

#endif
