#pragma once
#ifndef SYDEVS_QUALIFIED_TYPE_H_
#define SYDEVS_QUALIFIED_TYPE_H_

#include <sydevs/core/pointer.h>
#include <sydevs/core/quantity.h>
#include <sydevs/core/identity.h>
#include <sydevs/core/arraynd.h>
#include <sydevs/core/string_builder.h>
#include <string>
#include <tuple>
#include <set>
#include <map>
#include <functional>

namespace sydevs {


using tostring_function = std::function<std::string(const pointer&)>;

// qualified type trait declaration

template<typename T>
struct qualified_type {
    static constexpr bool valid = false;               ///< Indicates whether type `T` is a qualified type.
    static constexpr bool valid_and_sortable = false;  ///< Indicates whether type `T` is both a qualified type and a sortable type.
    static std::string tostring(const T& X);           ///< If `T` is a qualified type, converts `X` to a `std::string`.
    static pointer copy(const T& X);                   ///< If `T` is a qualified type, returns a deep copy of `X`.
};


// qualified type trait dependent function

template<typename T>
std::string tostring(const T& val)
{
    return qualified_type<T>::tostring(val);
}

template<typename T>
inline tostring_function tostring_converter()
{
    return [](const pointer& val) -> std::string {
        return qualified_type<T>::tostring(val.dereference<T>());
    };
}


// qualified type trait specialization declarations

template<>
struct qualified_type<bool> {
    static constexpr bool valid = true;
    static constexpr bool valid_and_sortable = true;
    static std::string tostring(const bool& X);
    static pointer copy(const bool& X);
};

template<>
struct qualified_type<int64> {
    static constexpr bool valid = true;
    static constexpr bool valid_and_sortable = true;
    static std::string tostring(const int64& X);
    static pointer copy(const int64& X);
};

template<>
struct qualified_type<float64> {
    static constexpr bool valid = true;
    static constexpr bool valid_and_sortable = true;
    static std::string tostring(const float64& X);
    static pointer copy(const float64& X);
};

template<>
struct qualified_type<std::string> {
    static constexpr bool valid = true;
    static constexpr bool valid_and_sortable = true;
    static std::string tostring(const std::string& X);
    static pointer copy(const std::string& X);
};

template<typename U>
struct qualified_type<quantity<U>> {
    static constexpr bool valid = true;
    static constexpr bool valid_and_sortable = true;
    static std::string tostring(const quantity<U>& X);
    static pointer copy(const quantity<U>& X);
};

template<typename U>
struct qualified_type<identity<U>> {
    static constexpr bool valid = true;
    static constexpr bool valid_and_sortable = true;
    static std::string tostring(const identity<U>& X);
    static pointer copy(const identity<U>& X);
};

template<typename T, int64 ndims>
struct qualified_type<arraynd<T, ndims>> {
    static constexpr bool valid = qualified_type<T>::valid;
    static constexpr bool valid_and_sortable = qualified_type<T>::valid_and_sortable && ndims == 1;
    static std::string tostring(const arraynd<T, ndims>& X);
    static pointer copy(const arraynd<T, ndims>& X);
};

template<typename T1, typename T2>
struct qualified_type<std::pair<T1, T2>> {
    static constexpr bool valid = (qualified_type<T1>::valid && qualified_type<T2>::valid);
    static constexpr bool valid_and_sortable = (qualified_type<T1>::valid_and_sortable && qualified_type<T2>::valid_and_sortable);
    static std::string tostring(const std::pair<T1, T2>& X);
    static pointer copy(const std::pair<T1, T2>& X);
};

template<typename T>
struct qualified_type<std::tuple<T>> {
    static constexpr bool valid = qualified_type<T>::valid;
    static constexpr bool valid_and_sortable = qualified_type<T>::valid_and_sortable;
    static std::string tostring(const std::tuple<T>& X);
    static pointer copy(const std::tuple<T>& X);
};

template<typename T, typename... Ts>
struct qualified_type<std::tuple<T, Ts...>> {
    static constexpr bool valid = (qualified_type<T>::valid && qualified_type<std::tuple<Ts...>>::valid);
    static constexpr bool valid_and_sortable = (qualified_type<T>::valid_and_sortable && qualified_type<std::tuple<Ts...>>::valid_and_sortable);
    static std::string tostring(const std::tuple<T, Ts...>& X);
    static pointer copy(const std::tuple<T, Ts...>& X);
};

template<typename T>
struct qualified_type<std::vector<T>> {
    static constexpr bool valid = qualified_type<T>::valid;
    static constexpr bool valid_and_sortable = qualified_type<T>::valid_and_sortable;
    static std::string tostring(const std::vector<T>& X);
    static pointer copy(const std::vector<T>& X);
};

template<typename T>
struct qualified_type<std::set<T>> {
    static constexpr bool valid = qualified_type<T>::valid;
    static constexpr bool valid_and_sortable = qualified_type<T>::valid_and_sortable;
    static std::string tostring(const std::set<T>& X);
    static pointer copy(const std::set<T>& X);
};

template<typename Key, typename T>
struct qualified_type<std::map<Key, T>> {
    static constexpr bool valid = qualified_type<Key>::valid_and_sortable && qualified_type<T>::valid;
    static constexpr bool valid_and_sortable = valid && qualified_type<T>::valid_and_sortable;
    static std::string tostring(const std::map<Key, T>& X);
    static pointer copy(const std::map<Key, T>& X);
};

template<typename T>
struct qualified_type<std::shared_ptr<T>> {
    static constexpr bool valid = true;
    static constexpr bool valid_and_sortable = false;
    static std::string tostring(const std::shared_ptr<T>& X);
    static pointer copy(const std::shared_ptr<T>& X);
};


// non qualified type trait definitions

template<typename T>
inline std::string qualified_type<T>::tostring(const T& X)
{
    throw std::logic_error("Conversion to string unavailable for a type that is not a qualified type");
    return "";
}

template<typename T>
inline pointer qualified_type<T>::copy(const T& X)
{
    throw std::logic_error("Copying unavailable for a type that is not a qualified type");
    return pointer();
}


// boolean trait definitions

inline std::string qualified_type<bool>::tostring(const bool& X)
{
    return X ? std::string("true") : std::string("false");
}

inline pointer qualified_type<bool>::copy(const bool& X)
{
    return pointer(new bool(X));
}


// int64 trait definitions

inline std::string qualified_type<int64>::tostring(const int64& X)
{
    return (string_builder() << X).str();
}

inline pointer qualified_type<int64>::copy(const int64& X)
{
    return pointer(new int64(X));
}


// float64 trait definitions

inline std::string qualified_type<float64>::tostring(const float64& X)
{
    return (string_builder() << X).str();
}

inline pointer qualified_type<float64>::copy(const float64& X)
{
    return pointer(new float64(X));
}


// std::string trait definitions

inline std::string qualified_type<std::string>::tostring(const std::string& X)
{
    return "\"" + X + "\"";
}

inline pointer qualified_type<std::string>::copy(const std::string& X)
{
    return pointer(new std::string(X));
}


// quantity<U> trait definitions

template<typename U>
inline std::string qualified_type<quantity<U>>::tostring(const quantity<U>& X)
{
    return (string_builder() << X).str();
}

template<typename U>
inline pointer qualified_type<quantity<U>>::copy(const quantity<U>& X)
{
    return pointer(new quantity<U>(X));
}


// identity<U> trait definitions

template<typename U>
inline std::string qualified_type<identity<U>>::tostring(const identity<U>& X)
{
    return (string_builder() << X).str();
}

template<typename U>
inline pointer qualified_type<identity<U>>::copy(const identity<U>& X)
{
    return pointer(new identity<U>(X));
}


// arraynd<T, ndims> trait definitions

template<typename T, int64 ndims>
inline std::string qualified_type<arraynd<T, ndims>>::tostring(const arraynd<T, ndims>& X)
{
    auto indices = std::array<int64, ndims>();
    for (int64 idim = 0; idim < ndims; ++idim) {
        indices[idim] = 0;
    }
    int64 offset = X.offset();
    int64 idim = 0;
    auto s = std::string("{");
    while (idim >= 0) {
        if (indices[idim] < X.dims()[idim]) {
            if (indices[idim] > 0) {
                s += ", ";
            }
            if (idim == ndims - 1) {
                s += qualified_type<T>::tostring(X.data()[offset]);
                ++indices[idim];
                offset += X.strides()[idim];
            }
            else {
                ++idim;
                s += "{";
            }
        }
        else {
            indices[idim] = 0;
            offset -= X.dims()[idim]*X.strides()[idim];
            --idim;
            if (idim >= 0) {
                ++indices[idim];
                offset += X.strides()[idim];
                s += "}";
            }
        }
    }
    s += "}";
    return s;
}

template<typename T, int64 ndims>
inline pointer qualified_type<arraynd<T, ndims>>::copy(const arraynd<T, ndims>& X)
{    
    return pointer(new arraynd<T, ndims>(X.copy()));
}


// std::pair<T1, T2> trait definitions

template<typename T1, typename T2>
inline std::string qualified_type<std::pair<T1, T2>>::tostring(const std::pair<T1, T2>& X)
{
    return "{" + qualified_type<T1>::tostring(X.first) + ", " + qualified_type<T2>::tostring(X.second) + "}";
}

template<typename T1, typename T2>
inline pointer qualified_type<std::pair<T1, T2>>::copy(const std::pair<T1, T2>& X)
{
    return pointer(new std::pair<T1, T2>(X));
}


// std::tuple<T> trait definitions

template<typename T>
inline std::string qualified_type<std::tuple<T>>::tostring(const std::tuple<T>& X)
{
    return "{" + qualified_type<T>::tostring(std::get<0>(X)) + "}";
}

template<typename T>
inline pointer qualified_type<std::tuple<T>>::copy(const std::tuple<T>& X)
{
    return pointer(new std::tuple<T>(X));
}


// std::tuple<T, Ts...> trait definitions

template<typename Tuple, std::size_t N>
struct tuple_tostring_helper {
    static std::string tostring(const Tuple& X) {
        return tuple_tostring_helper<Tuple, N - 1>::tostring(X) + ", " + qualified_type<typename std::tuple_element<N - 1, Tuple>::type>::tostring(std::get<N - 1>(X));
    }
};

template<typename Tuple>
struct tuple_tostring_helper<Tuple, 1> {
    static std::string tostring(const Tuple& X) {
        return qualified_type<typename std::tuple_element<0, Tuple>::type>::tostring(std::get<0>(X));
    }
};

template<typename T, typename... Ts>
inline std::string qualified_type<std::tuple<T, Ts...>>::tostring(const std::tuple<T, Ts...>& X)
{
    return "{" + tuple_tostring_helper<std::tuple<T, Ts...>, std::tuple_size<std::tuple<T, Ts...>>::value>::tostring(X) + "}";
}

template<typename T, typename... Ts>
inline pointer qualified_type<std::tuple<T, Ts...>>::copy(const std::tuple<T, Ts...>& X)
{
    return pointer(new std::tuple<T, Ts...>(X));
}


// std::vector<T> trait definitions

template<typename T>
inline std::string qualified_type<std::vector<T>>::tostring(const std::vector<T>& X)
{
    auto s = std::string("{");
    for (const auto& item : X) {
        if (s.back() != '{') {
            s += ", ";
        }
        s += qualified_type<T>::tostring(item);
    }
    s += "}";
    return s;
}

template<typename T>
inline pointer qualified_type<std::vector<T>>::copy(const std::vector<T>& X)
{
    return pointer(new std::vector<T>(X));
}


// std::set<T> trait definitions

template<typename T>
inline std::string qualified_type<std::set<T>>::tostring(const std::set<T>& X)
{
    auto s = std::string("{");
    for (const auto& item : X) {
        if (s.back() != '{') {
            s += ", ";
        }
        s += qualified_type<T>::tostring(item);
    }
    s += "}";
    return s;
}

template<typename T>
inline pointer qualified_type<std::set<T>>::copy(const std::set<T>& X)
{
    return pointer(new std::set<T>(X));
}


// std::map<Key, T> trait definitions

template<typename Key, typename T>
inline std::string qualified_type<std::map<Key, T>>::tostring(const std::map<Key, T>& X)
{
    auto s = std::string("{");
    for (const auto& entry : X) {
        if (s.back() != '{') {
            s += ", ";
        }
        s += ("{" + qualified_type<Key>::tostring(entry.first) + ", " + qualified_type<T>::tostring(entry.second) + "}");
    }
    s += "}";
    return s;
}

template<typename Key, typename T>
inline pointer qualified_type<std::map<Key, T>>::copy(const std::map<Key, T>& X)
{
    return pointer(new std::map<Key, T>(X));
}


// shared pointer trait definitions

template<typename T>
inline std::string qualified_type<std::shared_ptr<T>>::tostring(const std::shared_ptr<T>& X)
{
    return "(shared_ptr:" + 
        (X ? (qualified_type<T>::valid ? qualified_type<T>::tostring(*X) : std::string("...")) :
             std::string("nullptr"))
        + ")";
}

template<typename T>
inline pointer qualified_type<std::shared_ptr<T>>::copy(const std::shared_ptr<T>& X)
{
    return pointer(new std::shared_ptr<T>(X));
}


}  // namespace

#endif
