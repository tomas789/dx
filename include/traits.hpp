#ifndef TRAITS_HPP
#define TRAITS_HPP

#include <array>

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
 * Are same type trait
 */
template <typename... TList>
struct are_same { static const bool value = false; };

template <typename T, typename... TList>
struct are_same<T, T, TList...> {
    static const bool value = are_same<T, TList...>::value;
};

template <typename T>
struct are_same<T> { static const bool value = true; };

template <>
struct are_same<> { static const bool value = true; };

/**
 * Generic array inserter
 */
template <class Array, class T>
void move_to(Array& a, T && t) {
    a[a.size() - 1] = std::move(t);
}

template <class Array, class T, class... TList>
void move_to(Array& a, T && t, TList && ... plist) {
    a[a.size() - sizeof...(plist) - 1] = std::move(t);
    move_to(a, std::move(plist)...);
}

#endif
