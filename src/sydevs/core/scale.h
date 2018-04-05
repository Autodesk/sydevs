#pragma once
#ifndef SYDEVS_SCALE_H_
#define SYDEVS_SCALE_H_

#include <sydevs/core/number_types.h>
#include <iosfwd>

namespace sydevs {


/** 
 * @brief A data type which represents the general concept of scale as a
 *        dimensionless power of 1000.
 *
 * @details
 * A `scale` value can be used to characterize the resolution or approximate
 * magnitude of a meausure of time (i.e. a duration), space (i.e. a distance),
 * or potentially any other physical quantity.
 * 
 * Every `scale` value encapsulates a level integer, obtained using `level()`,
 * which serves as the exponent of the dimensionless power of 1000. If the
 * level integer is known at compile time, one of the predefined scale
 * constants below may be used instead of invoking the constructor. The
 * constants are named based on metric prefixes.
 * 
 * ~~~
 * yocto  // scale(-8);
 * zepto  // scale(-7);
 * atto   // scale(-6);
 * femto  // scale(-5);
 * pico   // scale(-4);
 * nano   // scale(-3);
 * micro  // scale(-2);
 * milli  // scale(-1);
 * unit   // scale(0);
 * kilo   // scale(1);
 * mega   // scale(2);
 * giga   // scale(3);
 * tera   // scale(4);
 * peta   // scale(5);
 * exa    // scale(6);
 * zetta  // scale(7);
 * yotta  // scale(8);
 * ~~~
 *
 * The prefix name may be obtained from the `scale` value using the 
 * `operator<<` overload.
 * 
 * The represented power (i.e. 1000 to the power `level()`) may be approximated
 * using `approx()`.
 * 
 * The following operators adjust the `scale` value's level integer: `++`,
 * `--`, `+=`, `-=`, `+`, `-`, `==`, `*`, `!=`, `<`, `>`, `<=`, `>=`. Examples
 * are below.
 * 
 * ~~~
 * nano + 4 == kilo
 * micro - 3 == femto
 * tera - mega == 2
 * 1 + milli == unit
 * ~~~
 * 
 * The `/` operator yields the factor that separates two scales. The result is
 * approximated if the denominator is a larger `scale` than the numerator.
 * 
 * ~~~
 * milli/pico == 1000000000
 * ~~~
 */
class scale
{
public:
    using level_type = int8;  ///< The type used to store the "level" integer internally.

    /**
     * @brief Constructs a `scale` value with a level integer of zero.
     */
    constexpr scale();

    /**
     * @brief Constructs a `scale` value with the specified level integer.
     * 
     * @details
     * The dimensionless power represented by the constructed `scale` value is
     * 1000 to the power of `level`.
     *
     * @param level The level integer of the `scale` value. 
     */
    explicit constexpr scale(int64 level);

    constexpr scale(const scale&)  = default;  ///< Copy constructor
    scale& operator=(const scale&) = default;  ///< Copy assignment
    scale(scale&&)                 = default;  ///< Move constructor
    scale& operator=(scale&&)      = default;  ///< Move assignment
    ~scale()                       = default;  ///< Destructor

    constexpr int64 level() const;     ///< Returns the level integer.
    constexpr float64 approx() const;  ///< Returns 1000 to the power of `level()`, rounded if necessary.

    scale& operator++();           ///< Increments (prefix) the level integer.
    scale  operator++(int);        ///< Increments (postfix) the level integer.
    scale& operator--();           ///< Decrements (prefix) the level integer.
    scale  operator--(int);        ///< Decrements (postfix) the level integer.
    scale& operator+=(int64 rhs);  ///< Adds `rhs` to the level integer.
    scale& operator-=(int64 rhs);  ///< Subtracts `rhs` from the level integer.

    constexpr const scale operator+() const;  ///< Returns a copy of the `scale` value.
    constexpr const scale operator-() const;  ///< Returns the negation of the `scale` value.

    constexpr const scale operator+(int64 rhs) const;  ///< Returns a `scale` value with `level()` increased by `rhs`.
    constexpr const scale operator-(int64 rhs) const;  ///< Returns a `scale` value with `level()` decreased by `rhs`.
    constexpr int64 operator-(scale rhs) const;        ///< Returns `level()` minus `rhs.level()`.
    constexpr float64 operator/(scale rhs) const;      ///< Returns 1000 to the power of `level()` divided by 1000 to the power of `rhs.level()`.

    constexpr bool operator==(scale rhs) const;  ///< Returns `true` if the `scale` value equals `rhs`.
    constexpr bool operator!=(scale rhs) const;  ///< Returns `true` if the `scale` value does not equal `rhs`.
    constexpr bool operator< (scale rhs) const;  ///< Returns `true` if the `scale` value is less than `rhs`.
    constexpr bool operator> (scale rhs) const;  ///< Returns `true` if the `scale` value is greater than `rhs`.
    constexpr bool operator<=(scale rhs) const;  ///< Returns `true` if the `scale` value is at most `rhs`.
    constexpr bool operator>=(scale rhs) const;  ///< Returns `true` if the `scale` value is at least `rhs`.

    /**
     * @brief Provdes the metric prefix symbol.
     * 
     * @details
     * The metric prefix symbol is the single-character as indicated at
     * http://en.wikipedia.org/wiki/Metric_prefix for `level() >= -8` and
     * `level() <= 8`. If the level integer is outside this range, or if it is
     * zero, then integer `0` is returned.
     *
     * @return The single-character metric prefix symbol or `0`.
     */
    char symbol() const;

private:
    level_type level_;

};


constexpr scale::scale()
    : level_(0)
{
}


constexpr scale::scale(int64 level)
    : level_(level_type(level))
{
}

    
constexpr scale no_scale = scale(std::numeric_limits<scale::level_type>::max());

constexpr scale yocto = scale(-8);
constexpr scale zepto = scale(-7);
constexpr scale atto  = scale(-6);
constexpr scale femto = scale(-5);
constexpr scale pico  = scale(-4);
constexpr scale nano  = scale(-3);
constexpr scale micro = scale(-2);
constexpr scale milli = scale(-1);
constexpr scale unit  = scale(0);
constexpr scale kilo  = scale(1);
constexpr scale mega  = scale(2);
constexpr scale giga  = scale(3);
constexpr scale tera  = scale(4);
constexpr scale peta  = scale(5);
constexpr scale exa   = scale(6);
constexpr scale zetta = scale(7);
constexpr scale yotta = scale(8);


constexpr int64 scale::level() const
{
    return level_;
}


constexpr float64 scale::approx() const
{ 
    return level_ == 0 ? 1.0 : 
           level_ < 0  ? 0.001*scale(level_ + 1).approx() : 
                         1000.0*scale(level_ - 1).approx();
}


constexpr const scale scale::operator+() const
{
    return scale(level_);
}


constexpr const scale scale::operator-() const
{
    return scale(-level_);
}


constexpr const scale scale::operator+(int64 rhs) const
{
    return scale(level_ + rhs);
}


constexpr const scale scale::operator-(int64 rhs) const
{
    return scale(level_ - rhs);
}


constexpr int64 scale::operator-(scale rhs) const
{
    return level_ - rhs.level_;
}


constexpr float64 scale::operator/(scale rhs) const
{ 
    return level_ == rhs.level_ ? 1.0 : 
           level_ < rhs.level_  ? 0.001*(scale(level_ + 1)/rhs) : 
                                  1000.0*(scale(level_ - 1)/rhs);
}


constexpr bool scale::operator==(scale rhs) const
{
    return level_ == rhs.level_;
}


constexpr bool scale::operator!=(scale rhs) const
{
    return level_ != rhs.level_;
}


constexpr bool scale::operator<(scale rhs) const
{
    return level_ < rhs.level_;
}


constexpr bool scale::operator>(scale rhs) const
{
    return level_ > rhs.level_;
}


constexpr bool scale::operator<=(scale rhs) const
{
    return level_ <= rhs.level_;
}


constexpr bool scale::operator>=(scale rhs) const
{
    return level_ >= rhs.level_;
}


constexpr const scale operator+(int64 lhs, scale rhs)
{
    return rhs + lhs;
}


std::ostream& operator<<(std::ostream& os, const scale& rhs);
  

}  // namespace

#endif
