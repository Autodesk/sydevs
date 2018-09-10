#pragma once
#ifndef SYDEVS_IDENTITY_H_
#define SYDEVS_IDENTITY_H_

#include <sydevs/core/number_types.h>
#include <ostream>

namespace sydevs {


/**
 * @brief A data type which identifies an item by combining an encapsulated
 *        integer-valued index with a dimension supplied by a template
 *        parameter.
 *
 * @details
 * An `identity` value is essentially a dimensioned integer, where the units
 * are provided by the template parameter `U`. An simple example of how to
 * instantiate an `identity` value is below.
 *
 * ~~~
 * class apple_unit;
 * using apple_id = identity<apple_unit>;
 * apple_id my_gala_apple(79);
 * ~~~
 *
 * A default-constructed `identity` value is invalid. For these values, the 
 * `valid` member function returns `false` and the `index` member function
 * returns the most negative representable 64-bit signed integer. For valid
 * `identity` values, the class supports increment and decrement operations, as
 * well as standard plus and minus operators that offset the encapsulated index
 * by an integer.
 *
 * ~~~
 * apple_id(555) + 10    // apple_id(565)
 * apple_id(555) - 1000  // apple_id(-445)
 * 215 + apple_id(555)   // apple_id(770)
 * ~~~
 */
template<typename U>
class identity 
{
public:
    /**
     * @brief Constructs an invalid `identity` value.
     */
    constexpr identity();

    /**
     * @brief Constructs an `identity` value with the specified `index`.
     */
    constexpr explicit identity(int64 index);

    constexpr identity(const identity&)  = default;  ///< Copy constructor
    identity& operator=(const identity&) = default;  ///< Copy assignment
    identity(identity&&)                 = default;  ///< Move constructor
    identity& operator=(identity&&)      = default;  ///< Move assignment
    ~identity()                          = default;  ///< Destructor

    constexpr bool valid() const;   ///< Returns `true` if the `identity` value is valid.
    constexpr int64 index() const;  ///< Returns the internal index.

    identity& operator++();              ///< Increments (prefix) the internal index.
    identity  operator++(int);           ///< Increments (postfix) the internal index.
    identity& operator--();              ///< Decrements (prefix) the internal index.
    identity  operator--(int);           ///< Decrements (postfix) the internal index.

    identity& operator+=(int64 rhs);  ///< Adds `rhs` to the `identity` value.
    identity& operator-=(int64 rhs);  ///< Subtracts `rhs` from the `identity` value.

    constexpr const identity operator+() const;  ///< Returns a copy of the `identity` value.
    
    constexpr const identity operator+(int64 rhs) const;  ///< Returns a new `identity` value with `rhs` added.
    constexpr const identity operator-(int64 rhs) const;  ///< Returns a new `identity` value with `rhs` subtracted.

    constexpr bool operator==(identity rhs) const;  ///< Returns `true` if the `identity` value equals `rhs`.
    constexpr bool operator!=(identity rhs) const;  ///< Returns `true` if the `identity` value does not equal `rhs`.
    constexpr bool operator<(identity rhs) const;   ///< Returns `true` if the `identity` value is less than `rhs`.
    constexpr bool operator>(identity rhs) const;   ///< Returns `true` if the `identity` value is greater than `rhs`.
    constexpr bool operator<=(identity rhs) const;  ///< Returns `true` if the `identity` value is at most `rhs`.
    constexpr bool operator>=(identity rhs) const;  ///< Returns `true` if the `identity` value is at least `rhs`.

protected:
    static constexpr int64 nan_int64 = std::numeric_limits<int64>::min();

private:
    int64 index_;
};


template<typename U>
constexpr identity<U>::identity()
    : index_(nan_int64)
{
}

    
template<typename U>
constexpr identity<U>::identity(int64 index)
    : index_(index)
{
}

    
template<typename U>
constexpr bool identity<U>::valid() const
{
    return index_ != nan_int64;
}


template<typename U>
constexpr int64 identity<U>::index() const
{
    return index_;
}


template<typename U>
identity<U>& identity<U>::operator++()
{
    if (valid()) {
        ++index_;
    }
    return *this;
}


template<typename U>
identity<U> identity<U>::operator++(int)
{
    auto old = *this;
    ++(*this);
    return old;
}


template<typename U>
identity<U>& identity<U>::operator--()
{
    if (valid()) {
        --index_;
    }
    return *this;
}


template<typename U>
identity<U> identity<U>::operator--(int)
{
    auto old = *this;
    --(*this);
    return old;
}


template<typename U>
identity<U>& identity<U>::operator+=(int64 rhs)
{
    *this = *this + rhs;
    return *this;
}


template<typename U>
identity<U>& identity<U>::operator-=(int64 rhs)
{
    *this = *this - rhs;
    return *this;
}


template<typename U>
constexpr const identity<U> identity<U>::operator+() const
{
    return *this;
}


template<typename U>
constexpr const identity<U> identity<U>::operator+(int64 rhs) const
{
    return valid() ? identity<U>(index_ + rhs) : identity<U>();
}


template<typename U>
constexpr const identity<U> identity<U>::operator-(int64 rhs) const
{
    return valid() ? identity<U>(index_ - rhs) : identity<U>();
}


template<typename U>
constexpr bool identity<U>::operator==(identity rhs) const
{
    return index_ == rhs.index_;
}


template<typename U>
constexpr bool identity<U>::operator!=(identity rhs) const
{
    return index_ != rhs.index_;
}


template<typename U>
constexpr bool identity<U>::operator<(identity rhs) const
{
    return index_ < rhs.index_;
}


template<typename U>
constexpr bool identity<U>::operator>(identity rhs) const
{
    return index_ > rhs.index_;
}


template<typename U>
constexpr bool identity<U>::operator<=(identity rhs) const
{
    return index_ <= rhs.index_;
}


template<typename U>
constexpr bool identity<U>::operator>=(identity rhs) const
{
    return index_ >= rhs.index_;
}

    
template<typename U>
constexpr const identity<U> operator+(int64 lhs, identity<U> rhs)
{
    return rhs + lhs;
}


template<typename U>
inline std::ostream& operator<<(std::ostream& os, const identity<U>& rhs)
{
    if (!rhs.valid()) {
        os << "{}";
    }
    else {
        os << "{" << rhs.index() << "}";
    }
    return os;
}


}  // namespace

#endif
