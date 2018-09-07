#pragma once
#ifndef SYDEVS_IDENTITY_H_
#define SYDEVS_IDENTITY_H_

#include <sydevs/core/number_types.h>
#include <ostream>

namespace sydevs {


template<typename U>
class identity 
{
public:
    constexpr identity();
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
    identity& operator+=(identity rhs);  ///< Adds `rhs` to the `identity` value.
    identity& operator-=(identity rhs);  ///< Subtracts `rhs` from the `identity` value.

    constexpr const identity operator+() const;  ///< Returns a copy of the `identity` value.
    constexpr const identity operator-() const;  ///< Returns the negation of the `identity` value.

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
    ++index_;
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
    --index_;
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
identity<U>& identity<U>::operator+=(identity rhs)
{
    *this = *this + rhs;
    return *this;
}


template<typename U>
identity<U>& identity<U>::operator-=(identity rhs)
{
    *this = *this - rhs;
    return *this;
}


template<typename U>
constexpr const identity<U> identity<U>::operator+() const
{
    return identity<U>(index_);
}


template<typename U>
constexpr const identity<U> identity<U>::operator-() const
{
    return valid ? identity<U>(-index_) : identity<U>();
}

    
template<typename U>
constexpr const identity<U> identity<U>::operator+(int64 rhs) const
{
    return valid && rhs.valid ? identity<U>(index_ + rhs.index_) : identity<U>();
}


template<typename U>
constexpr const identity<U> identity<U>::operator-(int64 rhs) const
{
    return valid && rhs.valid ? identity<U>(index_ - rhs.index_) : identity<U>();
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
