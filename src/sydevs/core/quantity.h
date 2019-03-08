#pragma once
#ifndef SYDEVS_QUANTITY_H_
#define SYDEVS_QUANTITY_H_

#include <sydevs/core/units.h>
#include <cmath>
#include <ostream>

namespace sydevs {


constexpr int64 quantity_limit = int64(1)*1000*1000*1000*1000*1000;  ///< The maximum precision multiplier, plus 1.


/** 
 * @brief A base class for quantities, defining operations that do not depend
 *        on the dimension.
 */ 
class quantity_base
{
public:
    constexpr bool valid() const;        ///< Returns `true` if the `quantity` value is valid.
    constexpr bool finite() const;       ///< Returns `true` if the `quantity` value is finite.
    constexpr int64 multiplier() const;  ///< Returns the number that multiples the length precision.
    constexpr scale precision() const;   ///< Returns the length precision.
    constexpr bool fixed() const;        ///< Returns `true` if the `quantity` value has a fixed length precision.

protected:
    constexpr quantity_base();
    explicit constexpr quantity_base(int64 multiplier);
    constexpr quantity_base(int64 multiplier, scale precision);
    constexpr quantity_base(int64 multiplier, scale precision, bool fixed);
    constexpr quantity_base(scale precision, float64 multiplier, int8 fixed);

    constexpr quantity_base(const quantity_base&)  = default;  ///< Copy constructor
    quantity_base& operator=(const quantity_base&) = default;  ///< Copy assignment
    quantity_base(quantity_base&&)                 = default;  ///< Move constructor
    quantity_base& operator=(quantity_base&&)      = default;  ///< Move assignment
    ~quantity_base()                               = default;  ///< Destructor

    static constexpr float64 inf_float64 = std::numeric_limits<float64>::infinity();
    static constexpr float64 nan_float64 = std::numeric_limits<float64>::quiet_NaN();
    static constexpr int64 inf_int64     = std::numeric_limits<int64>::max();
    static constexpr int64 nan_int64     = std::numeric_limits<int64>::min();

    static constexpr int64 constexpr_abs(int64 n);
    static constexpr float64 constexpr_abs(float64 x);
    static constexpr float64 convert_multiplier(int64 multiplier);
    static constexpr int64 convert_level(int64 multiplier, int64 level);
    static constexpr float64 offset_multiplier(float64 multiplier);
    static constexpr int64 truncate_multiplier(float64 multiplier);
    static constexpr int64 round_multiplier(float64 multiplier);
    static constexpr int64 scale_multiplier(float64 multiplier, float64 factor);

    float64 multiplier_;
    scale precision_;
    int8 fixed_;
};


/**
 * @brief A data type template which represents a dimensioned quantity as a
 *        multiple of a precision level, where the dimensions are supplied by
 *        a [units](@ref sydevs::units) template parameter and the precision
 *        level is specified using a value of type [scale](@ref sydevs::scale).
 *
 * @details
 * A `quantity` value allows for the exact specification, addition, and 
 * subtraction of dimensioned quantities provided that the following conditions
 * are met:
 *
 * - Quantities can be represented as an integer multiple of a precision level.
 * - The "multiplier", the number which multiplies the precision, is less than
 *   1,000,000,000,000,000 in magnitude (negative quantities are permitted).
 *
 * A set of `constexpr` user-defined literals provide a means to construct
 * `quantity` values, as shown below.
 * 
 * ~~~
 * 5_m      // 5 meters
 * 5_fs     // 5 femtoseconds
 * 5_um     // 5 micrometers
 * 6_kg     // 6 kilograms
 * 6000_Mg  // 6000 Megagrams
 * ~~~
 *
 * These user-defined literals correspond with the 7 Standard International (SI)
 * base units as represented by [units](@ref sydevs::units) (`_g`, `_m`, `_s`,
 * `_A`, `_K`, `_mol`, `_cd`), and may also incorporate an SI prefix (`y`, `z`,
 * `a`, `f`, `n`, `p`, `u`, `m`, `k`, `M`, `G`, `T`, `P`, `E`, `Z`, `Y`). Note
 * one difference from the SI standard: the gram (`1_g`) is used as the base
 * unit for mass, instead of the kilogram (`1_kg`) as in the standard.
 *
 * There are also 4 additional user-defined literals that produce
 * duration-valued quantities with a precision of 1 second and a multiplier
 * scaled up by 60 (`?_min`), 60`*`60 (`?_hr`), 24`*`60`*`60 (`?_day`) or 
 * 365`*`24`*`60`*`60 (`?_yr`). Examples of these literals are below.
 *
 * ~~~
 * 5_min  // 300 seconds (5 minutes)
 * 7_day  // 604,800 seconds (1 week)
 * ~~~
 *
 * Quantity values can also be created by directly invoking one of the three
 * constructors: the default constructor producing an invalid quanitity, the
 * single-argument constructor producing a quantity with a precision of one base
 * unit, and the two-argument constructor where both the multiplier and
 * precision level are supplied.
 *
 * ~~~
 * quantity<grams>()                         // invalid mass quantity
 * quantity<grams>(3)                        // 3 grams
 * quantity<meters>(3)                       // 3 meters
 * quantity<seconds>(3, nano)                // 3 nanoseconds
 * quantity<decltype(_m/_s)>(3)              // 3 meters/second
 * quantity<decltype(_g*_m/_s/_s)>(3, kilo)  // 3 Newtons
 * ~~~
 *
 * Type aliases are provided for quantities of the 7 SI base units.
 *
 * ~~~
 * mass                       // quantity<grams>
 * distance                   // quantity<meters>
 * duration                   // quantity<seconds>
 * electric_current           // quantity<amperes>
 * thermodynamic_temperature  // quantity<kelvins>
 * amount_of_substance        // quantity<moles>
 * luminous_intensity         // quantity<candelas>
 * ~~~
 *
 * These can be used in place of `quantity<...>` for quantities based on SI
 * base units.
 *
 * ~~~
 * mass()             // invalid mass quantity
 * mass(3)            // 3 grams
 * distance(3)        // 3 meters
 * duration(3, nano)  // 3 nanoseconds
 * ~~~
 *
 * One way to construct quantities based on SI derived units is to multiply or
 * divide a `quantity` value on the left with a `units` value on the right.
 *
 * ~~~
 * -777_kg*_m/_s/_s  // -777 Newtons
 * 90_km/_hr         // 25 meters per second (90 kilometers per hour)
 * ~~~
 *
 * Also, multiplying or dividing quantities yields a new quantity with different
 * units.
 *
 * ~~~
 * 200_m/8_s  // 25 meters per second
 * ~~~
 *
 * To remove the units from a quantity, simply divide by the unit. The result is
 * a dimensionless quantity value (`quantity<no_units>`) that can be coerced
 * into a floating-point number.
 *
 * ~~~
 * 1500_mA/_A  // 1.5
 * ~~~
 *
 * One can construct infinite quantities, as well as the maximum representable
 * quantity at a given precision level.
 *
 * ~~~
 * duration::inf()   // Infinite duration
 * mass::max(yocto)  // Maximum mass at ys precision (999999999999999_yg)
 * ~~~
 *
 * Precision is a core aspect of `quantity` types. To illustrate, although 
 * `(7000_us == 7_ms)` evaluates to `true`, the two values are different. The
 * left-hand side is stored as a muliplier of 7000 and a time precision of
 * microseconds, whereas the right-hand side is stored as a multiplier of 7 and
 * a time precision of milliseconds.
 *
 * By default, `quantity` values resulting from arithmetic operations have
 * their precision levels automatically adjusted to minimize rounding error.
 * This default behaviour treats `quantity` values as base-1000 floating-point
 * numbers. The following examples all involve time values.
 *
 * ~~~
 * 3_s + 475_ms       // Result: 3475 milliseconds
 * 1_ks + 1_us        // Result: 1000000001 microseconds
 * 500_ps - 1_ns      // Result: -500 picoseconds
 * (1.0/3.0)*1_s      // Result: 333333333333333 femtoseconds
 * (1.0/3.0)*1000_ms  // Result: 333333333333333 femtoseconds
 * 1000_ms/3.0        // Result: 333333333333333 femtoseconds
 * 10_ms/250_us       // Result: 40.0
 * ~~~
 * 
 * However, using the `fixed_at` member function, one can produce quantities
 * with fixed precision levels that remain unchanged through operations, as
 * illustrated below. This is in contrast to the `rescaled` member function,
 * which neither fixes nor unfixes the precision level.
 * 
 * ~~~
 * (1_min + 40_s).fixed_at(micro)/8.0  // Result: 12500000 microseconds
 * (1_min + 40_s).fixed_at(milli)/8.0  // Result: 12500 milliseconds
 * (1_min + 40_s).fixed_at(unit)/8.0   // Result: 13 seconds
 * (1_min + 40_s).rescaled(micro)/8.0  // Result: 12500000 microseconds
 * (1_min + 40_s).rescaled(milli)/8.0  // Result: 12500 milliseconds
 * (1_min + 40_s).rescaled(unit)/8.0   // Result: 12500 milliseconds
 * ~~~
 * 
 * Fixing the precision level prevents round-off errors when adding or
 * subtracting quantities. On the other hand, precision may be lost as in the
 * above example where 12500 milliseconds is rounded to 13 seconds.
 *
 * Operations between `quantity` values fixed at different precision levels
 * result in invalid quantities.
 * 
 * ~~~
 * ((1_s).fixed_at(milli) + (1_s).fixed_at(micro)).valid()  // Result: false
 * ~~~
 *
 * However if two `quantity` values have different precision levels but only
 * one is fixed, then the fixed precision will be adopted by the result.
 *
 * ~~~
 * ((1_s).fixed_at(micro) + (1_s).rescaled(nano)).precision()  // Result: micro
 * ~~~
 *
 * A `quantity` value can be modified using the `+=`, `-=`, `*=`, `/=`
 * operators. One must be aware that the member functions `fixed_at`,
 * `rescaled`, `refined`, `coarsened`, and `unfixed` return new `quantity`
 * values but leave the original unchanged.
 *
 * A `quantity` value may be output or converted to a string using the
 * `operator<<` overload.
 */
template<typename U>
class quantity : public quantity_base
{
template<typename U_> friend class quantity;
public:
    /**
     * @brief Constructs an invalid `quantity` value.
     */
    constexpr quantity();

    /**
     * @brief Constructs a `quantity` value representing the specified number
     *        of units with no prefix (i.e. milli, kilo).
     * 
     * @details
     * The constructed `quantity` value has the specified `multiplier` and a
     * precision level of `unit`. The precision does not remain fixed through
     * operations.
     * 
     * @param multiplier The number that mulitplies the default precision level
     *                   to yield the represented quantity. 
     */
    explicit constexpr quantity(int64 multiplier);

    /**
     * @brief Constructs a `quantity` value representing the specified multiple
     *        of the specified precision level.
     * 
     * @details
     * The constructed `quantity` value has the specified `multiplier` and
     * `precision`. The precision does not remain fixed through operations.
     * 
     * @param multiplier The number that mulitplies the specified precision
     *                   level to yield the represented quantity. 
     * @param precision The precision level that is multiplied to yield the
     *                  represented quantity. 
     */
    constexpr quantity(int64 multiplier, scale precision);

    constexpr quantity(const quantity&)  = default;  ///< Copy constructor
    quantity& operator=(const quantity&) = default;  ///< Copy assignment
    quantity(quantity&&)                 = default;  ///< Move constructor
    quantity& operator=(quantity&&)      = default;  ///< Move assignment
    ~quantity()                          = default;  ///< Destructor

    static constexpr quantity inf();
    static constexpr quantity max(scale precision);

    constexpr const quantity fixed_at(scale precision) const;  ///< Returns a new `quantity` value with the length precision changed and fixed.
    constexpr const quantity rescaled(scale precision) const;  ///< Returns a new `quantity` value with the length precision changed but neither fixed nor unfixed.
    constexpr const quantity refined() const;                  ///< Returns a new `quantity` value with the length precision minimized without losing precision.
    constexpr const quantity coarsened() const;                ///< Returns a new `quantity` value with the length precision maximized without losing precision.
    constexpr const quantity unfixed() const;                  ///< Returns a new `quantity` value with the length precision unfixed.

    quantity& operator+=(quantity rhs);  ///< Adds `rhs` to the `quantity` value.
    quantity& operator-=(quantity rhs);  ///< Subtracts `rhs` from the `quantity` value.
    quantity& operator*=(float64 rhs);   ///< Multiplies the `quantity` value by `rhs`.
    quantity& operator/=(float64 rhs);   ///< Divides the `quantity` value by `rhs`.

    constexpr const quantity operator+() const;  ///< Returns a copy of the `quantity` value.
    constexpr const quantity operator-() const;  ///< Returns the negation of the `quantity` value.

    constexpr const quantity operator+(quantity rhs) const;  ///< Returns a new `quantity` value with `rhs` added.
    constexpr const quantity operator-(quantity rhs) const;  ///< Returns a new `quantity` value with `rhs` subtracted.

    constexpr const quantity operator*(float64 rhs) const;  ///< Returns a new `quantity` value multiplied by `rhs`.
    constexpr const quantity operator/(float64 rhs) const;  ///< Returns a new `quantity` value divided by `rhs`.

    template<typename U_>
    constexpr quantity<decltype(U()*U_())> operator*(quantity<U_> rhs) const;  ///< Returns the `quantity` value multiplied by `rhs`.

    template<typename U_>
    constexpr quantity<decltype(U()/U_())> operator/(quantity<U_> rhs) const;  ///< Returns the `quantity` value divided by `rhs`.

    template<typename U_>
    constexpr quantity<decltype(U()*U_())> operator*(U_ rhs) const;  ///< Returns the `quantity` value multiplied by `rhs`.

    template<typename U_>
    constexpr quantity<decltype(U()/U_())> operator/(U_ rhs) const;  ///< Returns the `quantity` value divided by `rhs`.

    constexpr bool operator==(quantity rhs) const;  ///< Returns `true` if the `quantity` value equals `rhs`.
    constexpr bool operator!=(quantity rhs) const;  ///< Returns `true` if the `quantity` value does not equal `rhs`.
    constexpr bool operator<(quantity rhs) const;   ///< Returns `true` if the `quantity` value is less than `rhs`.
    constexpr bool operator>(quantity rhs) const;   ///< Returns `true` if the `quantity` value is greater than `rhs`.
    constexpr bool operator<=(quantity rhs) const;  ///< Returns `true` if the `quantity` value is at most `rhs`.
    constexpr bool operator>=(quantity rhs) const;  ///< Returns `true` if the `quantity` value is at least `rhs`.

private:
    constexpr quantity(int64 multiplier, scale precision, bool fixed);
    constexpr quantity(scale precision, float64 multiplier, int8 fixed);

    constexpr quantity autoscaled() const;
    constexpr quantity autorounded() const;
};


/** 
 * @brief A speciaulization to facilitate the conversion of dimensionless
 *        quantities into floats.
 */ 
template<>
class quantity<no_units> : public quantity_base
{
template<typename U_> friend class quantity;
public:
    constexpr quantity();
    explicit constexpr quantity(int64 multiplier);
    constexpr quantity(int64 multiplier, scale precision);

    constexpr quantity(const quantity&)  = default;  ///< Copy constructor
    quantity& operator=(const quantity&) = default;  ///< Copy assignment
    quantity(quantity&&)                 = default;  ///< Move constructor
    quantity& operator=(quantity&&)      = default;  ///< Move assignment
    ~quantity()                          = default;  ///< Destructor

    static constexpr quantity inf();
    static constexpr quantity max(scale precision);

    constexpr const quantity fixed_at(scale precision) const;  ///< Returns a new `quantity` value with the length precision changed and fixed.
    constexpr const quantity rescaled(scale precision) const;  ///< Returns a new `quantity` value with the length precision changed but neither fixed nor unfixed.
    constexpr const quantity refined() const;                  ///< Returns a new `quantity` value with the length precision minimized without losing precision.
    constexpr const quantity coarsened() const;                ///< Returns a new `quantity` value with the length precision maximized without losing precision.
    constexpr const quantity unfixed() const;                  ///< Returns a new `quantity` value with the length precision unfixed.

    constexpr const quantity operator+() const;  ///< Returns a copy of the `quantity` value.
    constexpr const quantity operator-() const;  ///< Returns the negation of the `quantity` value.

    constexpr const quantity operator+(quantity rhs) const;  ///< Returns a new `quantity` value with `rhs` added.
    constexpr const quantity operator-(quantity rhs) const;  ///< Returns a new `quantity` value with `rhs` subtracted.

    constexpr const quantity operator*(float64 rhs) const;  ///< Returns a new `quantity` value multiplied by `rhs`.
    constexpr const quantity operator/(float64 rhs) const;  ///< Returns a new `quantity` value divided by `rhs`.

    template<typename U_>
    constexpr quantity<U_> operator*(quantity<U_> rhs) const;  ///< Returns the `quantity` value multiplied by `rhs`.

    template<typename U_>
    constexpr quantity<decltype(_1/U_())> operator/(quantity<U_> rhs) const;  ///< Returns the `quantity` value divided by `rhs`.

    template<typename U_>
    constexpr quantity<U_> operator*(U_ rhs) const;  ///< Returns the `quantity` value multiplied by `rhs`.

    template<typename U_>
    constexpr quantity<decltype(_1/U_())> operator/(U_ rhs) const;  ///< Returns the `quantity` value divided by `rhs`.

    constexpr operator float64() const;  ///< Returns a floating-point representation of the `quantity` value.

private:
    constexpr quantity(int64 multiplier, scale precision, bool fixed);
    constexpr quantity(scale precision, float64 multiplier, int8 fixed);

    constexpr quantity autoscaled() const;
    constexpr quantity autorounded() const;
};


constexpr bool quantity_base::valid() const
{
    return multiplier_ == multiplier_;
}


constexpr bool quantity_base::finite() const
{
    return multiplier_ < inf_float64 && multiplier_ > -inf_float64;
}


constexpr int64 quantity_base::multiplier() const
{
    return !valid()                    ? nan_int64 :
           multiplier_ == inf_float64  ? inf_int64 :
           multiplier_ == -inf_float64 ? -inf_int64 :
                                         int64(multiplier_);
}


constexpr scale quantity_base::precision() const
{
    return precision_;
}


constexpr bool quantity_base::fixed() const
{
    return fixed_ == 1;
}


constexpr quantity_base::quantity_base()
    : multiplier_(nan_float64)
    , precision_(unit)
    , fixed_(0)
{
}

    
constexpr quantity_base::quantity_base(int64 multiplier)
    : multiplier_(convert_multiplier(multiplier))
    , precision_(unit)
    , fixed_(0)
{
}


constexpr quantity_base::quantity_base(int64 multiplier, scale precision)
    : multiplier_(convert_multiplier(multiplier))
    , precision_(convert_level(multiplier, precision.level()))
    , fixed_(0)
{
}


constexpr quantity_base::quantity_base(int64 multiplier, scale precision, bool fixed)
    : multiplier_(convert_multiplier(multiplier))
    , precision_(convert_level(multiplier, precision.level()))
    , fixed_(fixed)
{
}


constexpr quantity_base::quantity_base(scale precision, float64 multiplier, int8 fixed)
    : multiplier_(multiplier)
    , precision_(precision)
    , fixed_(fixed)
{
}


constexpr int64 quantity_base::constexpr_abs(int64 n)
{
    return n < 0 ? -n : n;
}


constexpr float64 quantity_base::constexpr_abs(float64 x)
{
    return x < 0 ? -x : x;
}


constexpr float64 quantity_base::convert_multiplier(int64 multiplier)
{
    return multiplier == nan_int64       ? nan_float64 : 
           multiplier >= quantity_limit  ? inf_float64 : 
           multiplier <= -quantity_limit ? -inf_float64 :
                                           float64(multiplier);
}


constexpr int64 quantity_base::convert_level(int64 multiplier, int64 level)
{
    return multiplier == nan_int64 || constexpr_abs(multiplier) >= quantity_limit ? 0 : 
                                                                                    level;
}


constexpr float64 quantity_base::offset_multiplier(float64 multiplier)
{
    return multiplier >= 0 ? multiplier + 0.5 :
                             multiplier - 0.5;
}


constexpr int64 quantity_base::truncate_multiplier(float64 multiplier)
{
    return multiplier >= quantity_limit  ? inf_int64 :
           multiplier <= -quantity_limit ? -inf_int64 :
                                           int64(multiplier);
}


constexpr int64 quantity_base::round_multiplier(float64 multiplier)
{
    return truncate_multiplier(offset_multiplier(multiplier));
}


constexpr int64 quantity_base::scale_multiplier(float64 multiplier, float64 factor)
{
    return factor >= 1 ? truncate_multiplier(offset_multiplier(factor*multiplier)) :
                         truncate_multiplier(factor*offset_multiplier(multiplier));
}


template<typename U>
constexpr quantity<U>::quantity()
    : quantity_base()
{
}

    
constexpr quantity<no_units>::quantity()
    : quantity_base()
{
}

    
template<typename U>
constexpr quantity<U>::quantity(int64 multiplier)
    : quantity_base(multiplier)
{
}


constexpr quantity<no_units>::quantity(int64 multiplier)
    : quantity_base(multiplier)
{
}


template<typename U>
constexpr quantity<U>::quantity(int64 multiplier, scale precision)
    : quantity_base(multiplier, precision)
{
}


constexpr quantity<no_units>::quantity(int64 multiplier, scale precision)
    : quantity_base(multiplier, precision)
{
}


template<typename U>
constexpr quantity<U>::quantity(int64 multiplier, scale precision, bool fixed)
    : quantity_base(multiplier, precision, fixed)
{
}


constexpr quantity<no_units>::quantity(int64 multiplier, scale precision, bool fixed)
    : quantity_base(multiplier, precision, fixed)
{
}


template<typename U>
constexpr quantity<U>::quantity(scale precision, float64 multiplier, int8 fixed)
    : quantity_base(precision, multiplier, fixed)
{
}


constexpr quantity<no_units>::quantity(scale precision, float64 multiplier, int8 fixed)
    : quantity_base(precision, multiplier, fixed)
{
}


template<typename U>
constexpr quantity<U> quantity<U>::inf()
{
    return quantity<U>(quantity_limit);
}


constexpr quantity<no_units> quantity<no_units>::inf()
{
    return quantity<no_units>(quantity_limit);
}


template<typename U>
constexpr quantity<U> quantity<U>::max(scale precision)
{
    return quantity<U>(quantity_limit - 1, precision);
}


constexpr quantity<no_units> quantity<no_units>::max(scale precision)
{
    return quantity<no_units>(quantity_limit - 1, precision);
}


template<typename U>
constexpr const quantity<U> quantity<U>::fixed_at(scale precision) const
{
    return !valid() ? quantity<U>() :
                      quantity<U>(scale_multiplier(multiplier_, precision_/precision), precision, int8(1));
}


constexpr const quantity<no_units> quantity<no_units>::fixed_at(scale precision) const
{
    return !valid() ? quantity<no_units>() :
                      quantity<no_units>(scale_multiplier(multiplier_, precision_/precision), precision, int8(1));
}


template<typename U>
constexpr const quantity<U> quantity<U>::rescaled(scale precision) const
{
    return !valid() ? quantity<U>() :
                      quantity<U>(scale_multiplier(multiplier_, precision_/precision), precision, fixed_ == 1);
}


constexpr const quantity<no_units> quantity<no_units>::rescaled(scale precision) const
{
    return !valid() ? quantity<no_units>() :
                      quantity<no_units>(scale_multiplier(multiplier_, precision_/precision), precision, fixed_ == 1);
}


template<typename U>
constexpr const quantity<U> quantity<U>::refined() const
{
    return !valid()                                                              ? quantity<U>() :
           !finite()                                                             ? *this :
           multiplier_ == 0                                                      ? quantity<U>(0) :
           constexpr_abs(round_multiplier(1000.0*multiplier_)) >= quantity_limit ? *this :
                                                                                   quantity<U>(precision_ - 1, 1000.0*multiplier_, int8(0)).autorounded().refined();
}


constexpr const quantity<no_units> quantity<no_units>::refined() const
{
    return !valid()                                                              ? quantity<no_units>() :
           !finite()                                                             ? *this :
           multiplier_ == 0                                                      ? quantity<no_units>(0) :
           constexpr_abs(round_multiplier(1000.0*multiplier_)) >= quantity_limit ? *this :
                                                                                   quantity<no_units>(precision_ - 1, 1000.0*multiplier_, int8(0)).autorounded().refined();
}


template<typename U>
constexpr const quantity<U> quantity<U>::coarsened() const
{
    return !valid()                                                  ? quantity<U>() :
           !finite()                                                 ? *this :
           multiplier_ == 0                                          ? quantity<U>(0) :
           multiplier_ != 1000.0*round_multiplier(0.001*multiplier_) ? *this :
                                                                       quantity<U>(precision_ + 1, float64(round_multiplier(0.001*multiplier_)), int8(0)).coarsened();
}


constexpr const quantity<no_units> quantity<no_units>::coarsened() const
{
    return !valid()                                                  ? quantity<no_units>() :
           !finite()                                                 ? *this :
           multiplier_ == 0                                          ? quantity<no_units>(0) :
           multiplier_ != 1000.0*round_multiplier(0.001*multiplier_) ? *this :
                                                                       quantity<no_units>(precision_ + 1, float64(round_multiplier(0.001*multiplier_)), int8(0)).coarsened();
}


template<typename U>
constexpr const quantity<U> quantity<U>::unfixed() const
{
    return quantity<U>(precision_, multiplier_, int8(0));
}


constexpr const quantity<no_units> quantity<no_units>::unfixed() const
{
    return quantity<no_units>(precision_, multiplier_, int8(0));
}


template<typename U>
quantity<U>& quantity<U>::operator+=(quantity<U> rhs)
{
    *this = *this + rhs;
    return *this;
}


template<typename U>
quantity<U>& quantity<U>::operator-=(quantity<U> rhs)
{
    *this = *this - rhs;
    return *this;
}


template<typename U>
quantity<U>& quantity<U>::operator*=(float64 rhs)
{
    *this = *this*rhs;
    return *this;
}


template<typename U>
quantity<U>& quantity<U>::operator/=(float64 rhs)
{
    *this = *this/rhs;
    return *this;
}


template<typename U>
constexpr const quantity<U> quantity<U>::operator+() const
{
    return quantity<U>(precision_, multiplier_, fixed_);
}


constexpr const quantity<no_units> quantity<no_units>::operator+() const
{
    return quantity<no_units>(precision_, multiplier_, fixed_);
}


template<typename U>
constexpr const quantity<U> quantity<U>::operator-() const
{
    return quantity<U>(precision_, -multiplier_, fixed_);
}

    
constexpr const quantity<no_units> quantity<no_units>::operator-() const
{
    return quantity<no_units>(precision_, -multiplier_, fixed_);
}

    
template<typename U>
constexpr const quantity<U> quantity<U>::operator+(quantity rhs) const
{
    return fixed_ && rhs.fixed_ ? precision_ == rhs.precision_ ? quantity<U>(precision_, multiplier_ + rhs.multiplier_, int8(1)).autorounded() :
                                                                 quantity<U>() :
           fixed_               ? quantity<U>(precision_, multiplier_ + (rhs.precision_/precision_)*rhs.multiplier_, int8(1)).autorounded() :
           rhs.fixed_           ? quantity<U>(rhs.precision_, (precision_/rhs.precision_)*multiplier_ + rhs.multiplier_, int8(1)).autorounded() :
                                  precision_ <= rhs.precision_ ? quantity<U>(precision_, multiplier_ + (rhs.precision_/precision_)*rhs.multiplier_, int8(0)).autoscaled() :
                                                                 quantity<U>(rhs.precision_, (precision_/rhs.precision_)*multiplier_ + rhs.multiplier_, int8(0)).autoscaled();
}


constexpr const quantity<no_units> quantity<no_units>::operator+(quantity rhs) const
{
    return fixed_ && rhs.fixed_ ? precision_ == rhs.precision_ ? quantity<no_units>(precision_, multiplier_ + rhs.multiplier_, int8(1)).autorounded() :
                                                                 quantity<no_units>() :
           fixed_               ? quantity<no_units>(precision_, multiplier_ + (rhs.precision_/precision_)*rhs.multiplier_, int8(1)).autorounded() :
           rhs.fixed_           ? quantity<no_units>(rhs.precision_, (precision_/rhs.precision_)*multiplier_ + rhs.multiplier_, int8(1)).autorounded() :
                                  precision_ <= rhs.precision_ ? quantity<no_units>(precision_, multiplier_ + (rhs.precision_/precision_)*rhs.multiplier_, int8(0)).autoscaled() :
                                                                 quantity<no_units>(rhs.precision_, (precision_/rhs.precision_)*multiplier_ + rhs.multiplier_, int8(0)).autoscaled();
}


template<typename U>
constexpr const quantity<U> quantity<U>::operator-(quantity rhs) const
{
    return fixed_ && rhs.fixed_ ? precision_ == rhs.precision_ ? quantity<U>(precision_, multiplier_ - rhs.multiplier_, int8(1)).autorounded() :
                                                                 quantity<U>() :
           fixed_               ? quantity<U>(precision_, multiplier_ - (rhs.precision_/precision_)*rhs.multiplier_, int8(1)).autorounded() :
           rhs.fixed_           ? quantity<U>(rhs.precision_, (precision_/rhs.precision_)*multiplier_ - rhs.multiplier_, int8(1)).autorounded() :
                                  precision_ <= rhs.precision_ ? quantity<U>(precision_, multiplier_ - (rhs.precision_/precision_)*rhs.multiplier_, int8(0)).autoscaled() :
                                                                 quantity<U>(rhs.precision_, (precision_/rhs.precision_)*multiplier_ - rhs.multiplier_, int8(0)).autoscaled();
}


constexpr const quantity<no_units> quantity<no_units>::operator-(quantity rhs) const
{
    return fixed_ && rhs.fixed_ ? precision_ == rhs.precision_ ? quantity<no_units>(precision_, multiplier_ - rhs.multiplier_, int8(1)).autorounded() :
                                                                 quantity<no_units>() :
           fixed_               ? quantity<no_units>(precision_, multiplier_ - (rhs.precision_/precision_)*rhs.multiplier_, int8(1)).autorounded() :
           rhs.fixed_           ? quantity<no_units>(rhs.precision_, (precision_/rhs.precision_)*multiplier_ - rhs.multiplier_, int8(1)).autorounded() :
                                  precision_ <= rhs.precision_ ? quantity<no_units>(precision_, multiplier_ - (rhs.precision_/precision_)*rhs.multiplier_, int8(0)).autoscaled() :
                                                                 quantity<no_units>(rhs.precision_, (precision_/rhs.precision_)*multiplier_ - rhs.multiplier_, int8(0)).autoscaled();
}


template<typename U>
constexpr const quantity<U> quantity<U>::operator*(float64 rhs) const
{
    return fixed_ ? quantity<U>(precision_, multiplier_*rhs, int8(1)).autorounded() :
                    quantity<U>(precision_, multiplier_*rhs, int8(0)).autoscaled();
}


constexpr const quantity<no_units> quantity<no_units>::operator*(float64 rhs) const
{
    return fixed_ ? quantity<no_units>(precision_, multiplier_*rhs, int8(1)).autorounded() :
                    quantity<no_units>(precision_, multiplier_*rhs, int8(0)).autoscaled();
}


template<typename U>
constexpr const quantity<U> quantity<U>::operator/(float64 rhs) const
{
    return fixed_ ? quantity<U>(precision_, multiplier_/rhs, int8(1)).autorounded() :
                    quantity<U>(precision_, multiplier_/rhs, int8(0)).autoscaled();
}


constexpr const quantity<no_units> quantity<no_units>::operator/(float64 rhs) const
{
    return fixed_ ? quantity<no_units>(precision_, multiplier_/rhs, int8(1)).autorounded() :
                    quantity<no_units>(precision_, multiplier_/rhs, int8(0)).autoscaled();
}


template<typename U>
template<typename U_>
constexpr quantity<decltype(U()*U_())> quantity<U>::operator*(quantity<U_> rhs) const
{
    return quantity<decltype(U()*U_())>(precision_ + rhs.precision().level(), multiplier_*rhs.multiplier(), int8(0)).autoscaled();
}


template<typename U_>
constexpr quantity<U_> quantity<no_units>::operator*(quantity<U_> rhs) const
{
    return quantity<U_>(precision_ + rhs.precision().level(), multiplier_*rhs.multiplier(), int8(0)).autoscaled();
}


template<typename U>
template<typename U_>
constexpr quantity<decltype(U()/U_())> quantity<U>::operator/(quantity<U_> rhs) const
{
    return !rhs.valid() ? quantity<decltype(U()/U_())>() :
                          quantity<decltype(U()/U_())>(precision_ - rhs.precision().level(), multiplier_/rhs.multiplier(), int8(0)).autoscaled();
}


template<typename U_>
constexpr quantity<decltype(_1/U_())> quantity<no_units>::operator/(quantity<U_> rhs) const
{
    return !rhs.valid() ? quantity<decltype(_1/U_())>() :
                          quantity<decltype(_1/U_())>(precision_ - rhs.precision().level(), multiplier_/rhs.multiplier(), int8(0)).autoscaled();
}


template<typename U>
template<typename U_>
constexpr quantity<decltype(U()*U_())> quantity<U>::operator*(U_ rhs) const
{
    return quantity<decltype(U()*U_())>(precision_, multiplier_, int8(0));
}


template<typename U_>
constexpr quantity<U_> quantity<no_units>::operator*(U_ rhs) const
{
    return quantity<U_>(precision_, multiplier_, int8(0));
}


template<typename U>
template<typename U_>
constexpr quantity<decltype(U()/U_())> quantity<U>::operator/(U_ rhs) const
{
    return quantity<decltype(U()/U_())>(precision_, multiplier_, int8(0));
}


template<typename U_>
constexpr quantity<decltype(_1/U_())> quantity<no_units>::operator/(U_ rhs) const
{
    return quantity<decltype(_1/U_())>(precision_, multiplier_, int8(0));
}


template<typename U>
constexpr bool quantity<U>::operator==(quantity<U> rhs) const
{
    return precision_ <= rhs.precision_ ? multiplier_ == (rhs.precision_/precision_)*rhs.multiplier_ :
                                          (precision_/rhs.precision_)*multiplier_ == rhs.multiplier_;
}


template<typename U>
constexpr bool quantity<U>::operator!=(quantity<U> rhs) const
{
    return precision_ <= rhs.precision_ ? multiplier_ != (rhs.precision_/precision_)*rhs.multiplier_ :
                                          (precision_/rhs.precision_)*multiplier_ != rhs.multiplier_;
}


template<typename U>
constexpr bool quantity<U>::operator<(quantity<U> rhs) const
{
    return precision_ <= rhs.precision_ ? multiplier_ < (rhs.precision_/precision_)*rhs.multiplier_ :
                                          (precision_/rhs.precision_)*multiplier_ < rhs.multiplier_;
}


template<typename U>
constexpr bool quantity<U>::operator>(quantity<U> rhs) const
{
    return precision_ <= rhs.precision_ ? multiplier_ > (rhs.precision_/precision_)*rhs.multiplier_ :
                                          (precision_/rhs.precision_)*multiplier_ > rhs.multiplier_;
}


template<typename U>
constexpr bool quantity<U>::operator<=(quantity<U> rhs) const
{
    return precision_ <= rhs.precision_ ? multiplier_ <= (rhs.precision_/precision_)*rhs.multiplier_ :
                                          (precision_/rhs.precision_)*multiplier_ <= rhs.multiplier_;
}


template<typename U>
constexpr bool quantity<U>::operator>=(quantity<U> rhs) const
{
    return precision_ <= rhs.precision_ ? multiplier_ >= (rhs.precision_/precision_)*rhs.multiplier_ :
                                          (precision_/rhs.precision_)*multiplier_ >= rhs.multiplier_;
}


template<typename U>
constexpr quantity<U> quantity<U>::autoscaled() const
{
    return !valid()                                                  ? quantity<U>() :
           multiplier_ == inf_float64                                ? quantity<U>::inf() :
           multiplier_ == -inf_float64                               ? -quantity<U>::inf() :
           constexpr_abs(multiplier_) + 0.5 >= quantity_limit        ? quantity<U>(precision_ + 1, 0.001*multiplier_, int8(0)).autoscaled() :
           1000.0*constexpr_abs(multiplier_) + 0.5 >= quantity_limit ? autorounded() :
           multiplier_ == int64(multiplier_)                         ? autorounded() :
                                                                       quantity<U>(precision_ - 1, 1000.0*multiplier_, int8(0)).autoscaled();
}


constexpr quantity<no_units> quantity<no_units>::autoscaled() const
{
    return !valid()                                                  ? quantity<no_units>() :
           multiplier_ == inf_float64                                ? quantity<no_units>::inf() :
           multiplier_ == -inf_float64                               ? -quantity<no_units>::inf() :
           constexpr_abs(multiplier_) + 0.5 >= quantity_limit        ? quantity<no_units>(precision_ + 1, 0.001*multiplier_, int8(0)).autoscaled() :
           1000.0*constexpr_abs(multiplier_) + 0.5 >= quantity_limit ? autorounded() :
           multiplier_ == int64(multiplier_)                         ? autorounded() :
                                                                       quantity<no_units>(precision_ - 1, 1000.0*multiplier_, int8(0)).autoscaled();
}


template<typename U>
constexpr quantity<U> quantity<U>::autorounded() const
{
    return !valid()                             ? quantity<U>() :
           multiplier_ + 0.5 >= quantity_limit  ? quantity<U>::inf() :
           multiplier_ - 0.5 <= -quantity_limit ? -quantity<U>::inf() :
                                                  quantity(int64(offset_multiplier(multiplier_)), precision_, fixed_ == 1);
}


constexpr quantity<no_units> quantity<no_units>::autorounded() const
{
    return !valid()                             ? quantity<no_units>() :
           multiplier_ + 0.5 >= quantity_limit  ? quantity<no_units>::inf() :
           multiplier_ - 0.5 <= -quantity_limit ? -quantity<no_units>::inf() :
                                                  quantity(int64(offset_multiplier(multiplier_)), precision_, fixed_ == 1);
}


constexpr quantity<no_units>::operator float64() const
{
    return precision_ == unit ? multiplier_ :
           precision_ < unit  ? 0.001*float64(quantity<no_units>(precision_ + 1, multiplier_, int8(0))) :
                                1000.0*float64(quantity<no_units>(precision_ - 1, multiplier_, int8(0)));
}


template<typename U>
constexpr const quantity<U> operator*(float64 lhs, quantity<U> rhs)
{
    return rhs*lhs;
}


template<typename U>
constexpr const quantity<decltype(_1/U())> operator/(float64 lhs, quantity<U> rhs)
{
    return (quantity<no_units>(1)*lhs)/rhs;
}


using mass = quantity<grams>;
using distance = quantity<meters>;
using duration = quantity<seconds>;
using electric_current = quantity<amperes>;
using thermodynamic_temperature = quantity<kelvins>;
using amount_of_substance = quantity<moles>;
using luminous_intensity = quantity<candelas>;


constexpr mass operator"" _yg(unsigned long long multiplier) { return mass(int64(multiplier), yocto); }
constexpr mass operator"" _zg(unsigned long long multiplier) { return mass(int64(multiplier), zepto); }
constexpr mass operator"" _ag(unsigned long long multiplier) { return mass(int64(multiplier), atto); }
constexpr mass operator"" _fg(unsigned long long multiplier) { return mass(int64(multiplier), femto); }
constexpr mass operator"" _pg(unsigned long long multiplier) { return mass(int64(multiplier), pico); }
constexpr mass operator"" _ng(unsigned long long multiplier) { return mass(int64(multiplier), nano); }
constexpr mass operator"" _ug(unsigned long long multiplier) { return mass(int64(multiplier), micro); }
constexpr mass operator"" _mg(unsigned long long multiplier) { return mass(int64(multiplier), milli); }
constexpr mass operator"" _g(unsigned long long multiplier)  { return mass(int64(multiplier)); }
constexpr mass operator"" _kg(unsigned long long multiplier) { return mass(int64(multiplier), kilo); }
constexpr mass operator"" _Mg(unsigned long long multiplier) { return mass(int64(multiplier), mega); }
constexpr mass operator"" _Gg(unsigned long long multiplier) { return mass(int64(multiplier), giga); }
constexpr mass operator"" _Tg(unsigned long long multiplier) { return mass(int64(multiplier), tera); }
constexpr mass operator"" _Pg(unsigned long long multiplier) { return mass(int64(multiplier), peta); }
constexpr mass operator"" _Eg(unsigned long long multiplier) { return mass(int64(multiplier), exa); }
constexpr mass operator"" _Zg(unsigned long long multiplier) { return mass(int64(multiplier), zetta); }
constexpr mass operator"" _Yg(unsigned long long multiplier) { return mass(int64(multiplier), yotta); }


constexpr distance operator"" _ym(unsigned long long multiplier) { return distance(int64(multiplier), yocto); }
constexpr distance operator"" _zm(unsigned long long multiplier) { return distance(int64(multiplier), zepto); }
constexpr distance operator"" _am(unsigned long long multiplier) { return distance(int64(multiplier), atto); }
constexpr distance operator"" _fm(unsigned long long multiplier) { return distance(int64(multiplier), femto); }
constexpr distance operator"" _pm(unsigned long long multiplier) { return distance(int64(multiplier), pico); }
constexpr distance operator"" _nm(unsigned long long multiplier) { return distance(int64(multiplier), nano); }
constexpr distance operator"" _um(unsigned long long multiplier) { return distance(int64(multiplier), micro); }
constexpr distance operator"" _mm(unsigned long long multiplier) { return distance(int64(multiplier), milli); }
constexpr distance operator"" _m(unsigned long long multiplier)  { return distance(int64(multiplier)); }
constexpr distance operator"" _km(unsigned long long multiplier) { return distance(int64(multiplier), kilo); }
constexpr distance operator"" _Mm(unsigned long long multiplier) { return distance(int64(multiplier), mega); }
constexpr distance operator"" _Gm(unsigned long long multiplier) { return distance(int64(multiplier), giga); }
constexpr distance operator"" _Tm(unsigned long long multiplier) { return distance(int64(multiplier), tera); }
constexpr distance operator"" _Pm(unsigned long long multiplier) { return distance(int64(multiplier), peta); }
constexpr distance operator"" _Em(unsigned long long multiplier) { return distance(int64(multiplier), exa); }
constexpr distance operator"" _Zm(unsigned long long multiplier) { return distance(int64(multiplier), zetta); }
constexpr distance operator"" _Ym(unsigned long long multiplier) { return distance(int64(multiplier), yotta); }


constexpr duration operator"" _ys(unsigned long long multiplier) { return duration(int64(multiplier), yocto); }
constexpr duration operator"" _zs(unsigned long long multiplier) { return duration(int64(multiplier), zepto); }
constexpr duration operator"" _as(unsigned long long multiplier) { return duration(int64(multiplier), atto); }
constexpr duration operator"" _fs(unsigned long long multiplier) { return duration(int64(multiplier), femto); }
constexpr duration operator"" _ps(unsigned long long multiplier) { return duration(int64(multiplier), pico); }
constexpr duration operator"" _ns(unsigned long long multiplier) { return duration(int64(multiplier), nano); }
constexpr duration operator"" _us(unsigned long long multiplier) { return duration(int64(multiplier), micro); }
constexpr duration operator"" _ms(unsigned long long multiplier) { return duration(int64(multiplier), milli); }
constexpr duration operator"" _s(unsigned long long multiplier)  { return duration(int64(multiplier)); }
constexpr duration operator"" _ks(unsigned long long multiplier) { return duration(int64(multiplier), kilo); }
constexpr duration operator"" _Ms(unsigned long long multiplier) { return duration(int64(multiplier), mega); }
constexpr duration operator"" _Gs(unsigned long long multiplier) { return duration(int64(multiplier), giga); }
constexpr duration operator"" _Ts(unsigned long long multiplier) { return duration(int64(multiplier), tera); }
constexpr duration operator"" _Ps(unsigned long long multiplier) { return duration(int64(multiplier), peta); }
constexpr duration operator"" _Es(unsigned long long multiplier) { return duration(int64(multiplier), exa); }
constexpr duration operator"" _Zs(unsigned long long multiplier) { return duration(int64(multiplier), zetta); }
constexpr duration operator"" _Ys(unsigned long long multiplier) { return duration(int64(multiplier), yotta); }

constexpr duration operator"" _min(unsigned long long multiplier) { return duration(int64(60*multiplier)); }
constexpr duration operator"" _hr(unsigned long long multiplier)  { return duration(int64((60*60)*multiplier)); }
constexpr duration operator"" _day(unsigned long long multiplier) { return duration(int64((60*60*24)*multiplier)); }
constexpr duration operator"" _yr(unsigned long long multiplier)  { return duration(int64((60*60*24*365)*multiplier)); }


constexpr electric_current operator"" _yA(unsigned long long multiplier) { return electric_current(int64(multiplier), yocto); }
constexpr electric_current operator"" _zA(unsigned long long multiplier) { return electric_current(int64(multiplier), zepto); }
constexpr electric_current operator"" _aA(unsigned long long multiplier) { return electric_current(int64(multiplier), atto); }
constexpr electric_current operator"" _fA(unsigned long long multiplier) { return electric_current(int64(multiplier), femto); }
constexpr electric_current operator"" _pA(unsigned long long multiplier) { return electric_current(int64(multiplier), pico); }
constexpr electric_current operator"" _nA(unsigned long long multiplier) { return electric_current(int64(multiplier), nano); }
constexpr electric_current operator"" _uA(unsigned long long multiplier) { return electric_current(int64(multiplier), micro); }
constexpr electric_current operator"" _mA(unsigned long long multiplier) { return electric_current(int64(multiplier), milli); }
constexpr electric_current operator"" _A(unsigned long long multiplier)  { return electric_current(int64(multiplier)); }
constexpr electric_current operator"" _kA(unsigned long long multiplier) { return electric_current(int64(multiplier), kilo); }
constexpr electric_current operator"" _MA(unsigned long long multiplier) { return electric_current(int64(multiplier), mega); }
constexpr electric_current operator"" _GA(unsigned long long multiplier) { return electric_current(int64(multiplier), giga); }
constexpr electric_current operator"" _TA(unsigned long long multiplier) { return electric_current(int64(multiplier), tera); }
constexpr electric_current operator"" _PA(unsigned long long multiplier) { return electric_current(int64(multiplier), peta); }
constexpr electric_current operator"" _EA(unsigned long long multiplier) { return electric_current(int64(multiplier), exa); }
constexpr electric_current operator"" _ZA(unsigned long long multiplier) { return electric_current(int64(multiplier), zetta); }
constexpr electric_current operator"" _YA(unsigned long long multiplier) { return electric_current(int64(multiplier), yotta); }


constexpr thermodynamic_temperature operator"" _yK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), yocto); }
constexpr thermodynamic_temperature operator"" _zK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), zepto); }
constexpr thermodynamic_temperature operator"" _aK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), atto); }
constexpr thermodynamic_temperature operator"" _fK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), femto); }
constexpr thermodynamic_temperature operator"" _pK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), pico); }
constexpr thermodynamic_temperature operator"" _nK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), nano); }
constexpr thermodynamic_temperature operator"" _uK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), micro); }
constexpr thermodynamic_temperature operator"" _mK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), milli); }
constexpr thermodynamic_temperature operator"" _K(unsigned long long multiplier)  { return thermodynamic_temperature(int64(multiplier)); }
constexpr thermodynamic_temperature operator"" _kK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), kilo); }
constexpr thermodynamic_temperature operator"" _MK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), mega); }
constexpr thermodynamic_temperature operator"" _GK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), giga); }
constexpr thermodynamic_temperature operator"" _TK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), tera); }
constexpr thermodynamic_temperature operator"" _PK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), peta); }
constexpr thermodynamic_temperature operator"" _EK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), exa); }
constexpr thermodynamic_temperature operator"" _ZK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), zetta); }
constexpr thermodynamic_temperature operator"" _YK(unsigned long long multiplier) { return thermodynamic_temperature(int64(multiplier), yotta); }


constexpr amount_of_substance operator"" _ymol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), yocto); }
constexpr amount_of_substance operator"" _zmol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), zepto); }
constexpr amount_of_substance operator"" _amol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), atto); }
constexpr amount_of_substance operator"" _fmol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), femto); }
constexpr amount_of_substance operator"" _pmol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), pico); }
constexpr amount_of_substance operator"" _nmol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), nano); }
constexpr amount_of_substance operator"" _umol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), micro); }
constexpr amount_of_substance operator"" _mmol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), milli); }
constexpr amount_of_substance operator"" _mol(unsigned long long multiplier)  { return amount_of_substance(int64(multiplier)); }
constexpr amount_of_substance operator"" _kmol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), kilo); }
constexpr amount_of_substance operator"" _Mmol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), mega); }
constexpr amount_of_substance operator"" _Gmol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), giga); }
constexpr amount_of_substance operator"" _Tmol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), tera); }
constexpr amount_of_substance operator"" _Pmol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), peta); }
constexpr amount_of_substance operator"" _Emol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), exa); }
constexpr amount_of_substance operator"" _Zmol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), zetta); }
constexpr amount_of_substance operator"" _Ymol(unsigned long long multiplier) { return amount_of_substance(int64(multiplier), yotta); }


constexpr luminous_intensity operator"" _ycd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), yocto); }
constexpr luminous_intensity operator"" _zcd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), zepto); }
constexpr luminous_intensity operator"" _acd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), atto); }
constexpr luminous_intensity operator"" _fcd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), femto); }
constexpr luminous_intensity operator"" _pcd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), pico); }
constexpr luminous_intensity operator"" _ncd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), nano); }
constexpr luminous_intensity operator"" _ucd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), micro); }
constexpr luminous_intensity operator"" _mcd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), milli); }
constexpr luminous_intensity operator"" _cd(unsigned long long multiplier)  { return luminous_intensity(int64(multiplier)); }
constexpr luminous_intensity operator"" _kcd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), kilo); }
constexpr luminous_intensity operator"" _Mcd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), mega); }
constexpr luminous_intensity operator"" _Gcd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), giga); }
constexpr luminous_intensity operator"" _Tcd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), tera); }
constexpr luminous_intensity operator"" _Pcd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), peta); }
constexpr luminous_intensity operator"" _Ecd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), exa); }
constexpr luminous_intensity operator"" _Zcd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), zetta); }
constexpr luminous_intensity operator"" _Ycd(unsigned long long multiplier) { return luminous_intensity(int64(multiplier), yotta); }


inline std::ostream& operator<<(std::ostream& os, const mass& rhs)
{
    if (!rhs.valid()) {
        os << "mass()";
    }
    else {
        if (!rhs.finite()) {
            os << (rhs < 0_g ? "-" : "") << "mass::inf()";
        }
        else {
            if (rhs.precision() == unit) {
                os << rhs.multiplier() << "_g";
            }
            else if (rhs.precision() >= yocto && rhs.precision() <= yotta) {
                os << rhs.multiplier() << "_" << rhs.precision().symbol() << "g";
            }
            else {
                os << "mass(" << rhs.multiplier() << ", " << rhs.precision() << ")";
            }
        }
    }
    return os;
}


inline std::ostream& operator<<(std::ostream& os, const distance& rhs)
{
    if (!rhs.valid()) {
        os << "distance()";
    }
    else {
        if (!rhs.finite()) {
            os << (rhs < 0_m ? "-" : "") << "distance::inf()";
        }
        else {
            if (rhs.precision() == unit) {
                os << rhs.multiplier() << "_m";
            }
            else if (rhs.precision() >= yocto && rhs.precision() <= yotta) {
                os << rhs.multiplier() << "_" << rhs.precision().symbol() << "m";
            }
            else {
                os << "distance(" << rhs.multiplier() << ", " << rhs.precision() << ")";
            }
        }
    }
    return os;
}


inline std::ostream& operator<<(std::ostream& os, const duration& rhs)
{
    if (!rhs.valid()) {
        os << "duration()";
    }
    else {
        if (!rhs.finite()) {
            os << (rhs < 0_s ? "-" : "") << "duration::inf()";
        }
        else {
            if (rhs.precision() == unit) {
                os << rhs.multiplier() << "_s";
            }
            else if (rhs.precision() >= yocto && rhs.precision() <= yotta) {
                os << rhs.multiplier() << "_" << rhs.precision().symbol() << "s";
            }
            else {
                os << "duration(" << rhs.multiplier() << ", " << rhs.precision() << ")";
            }
        }
    }
    return os;
}


inline std::ostream& operator<<(std::ostream& os, const electric_current& rhs)
{
    if (!rhs.valid()) {
        os << "electric_current()";
    }
    else {
        if (!rhs.finite()) {
            os << (rhs < 0_A ? "-" : "") << "electric_current::inf()";
        }
        else {
            if (rhs.precision() == unit) {
                os << rhs.multiplier() << "_A";
            }
            else if (rhs.precision() >= yocto && rhs.precision() <= yotta) {
                os << rhs.multiplier() << "_" << rhs.precision().symbol() << "A";
            }
            else {
                os << "electric_current(" << rhs.multiplier() << ", " << rhs.precision() << ")";
            }
        }
    }
    return os;
}


inline std::ostream& operator<<(std::ostream& os, const thermodynamic_temperature& rhs)
{
    if (!rhs.valid()) {
        os << "thermodynamic_temperature()";
    }
    else {
        if (!rhs.finite()) {
            os << (rhs < 0_K ? "-" : "") << "thermodynamic_temperature::inf()";
        }
        else {
            if (rhs.precision() == unit) {
                os << rhs.multiplier() << "_K";
            }
            else if (rhs.precision() >= yocto && rhs.precision() <= yotta) {
                os << rhs.multiplier() << "_" << rhs.precision().symbol() << "K";
            }
            else {
                os << "thermodynamic_temperature(" << rhs.multiplier() << ", " << rhs.precision() << ")";
            }
        }
    }
    return os;
}


inline std::ostream& operator<<(std::ostream& os, const amount_of_substance& rhs)
{
    if (!rhs.valid()) {
        os << "amount_of_substance()";
    }
    else {
        if (!rhs.finite()) {
            os << (rhs < 0_mol ? "-" : "") << "amount_of_substance::inf()";
        }
        else {
            if (rhs.precision() == unit) {
                os << rhs.multiplier() << "_mol";
            }
            else if (rhs.precision() >= yocto && rhs.precision() <= yotta) {
                os << rhs.multiplier() << "_" << rhs.precision().symbol() << "mol";
            }
            else {
                os << "amount_of_substance(" << rhs.multiplier() << ", " << rhs.precision() << ")";
            }
        }
    }
    return os;
}


inline std::ostream& operator<<(std::ostream& os, const luminous_intensity& rhs)
{
    if (!rhs.valid()) {
        os << "luminous_intensity()";
    }
    else {
        if (!rhs.finite()) {
            os << (rhs < 0_cd ? "-" : "") << "luminous_intensity::inf()";
        }
        else {
            if (rhs.precision() == unit) {
                os << rhs.multiplier() << "_cd";
            }
            else if (rhs.precision() >= yocto && rhs.precision() <= yotta) {
                os << rhs.multiplier() << "_" << rhs.precision().symbol() << "cd";
            }
            else {
                os << "luminous_intensity(" << rhs.multiplier() << ", " << rhs.precision() << ")";
            }
        }
    }
    return os;
}


inline std::ostream& operator<<(std::ostream& os, const quantity<no_units>& rhs)
{
    if (rhs.valid() && rhs < quantity<no_units>(0)) {
        os << "-";
    }
    os << "quantity<no_units>";
    if (!rhs.valid()) {
        os << "()";
    }
    else if (!rhs.finite()) {
        os << "::inf()";
    }
    else {
        os << "(" << (rhs.multiplier() < 0 ? -rhs.multiplier() : rhs.multiplier()) << ", " << rhs.precision() << ")";
    }
    return os;
}


template<typename U>
inline std::ostream& operator<<(std::ostream& os, const quantity<U>& rhs)
{
    if (rhs.valid() && rhs < quantity<U>(0)) {
        os << "-";
    }
    os << "quantity<decltype(";
    bool starting = true;
    for (int64 i = 0; i != U::g_; i += (U::g_ > 0 ? 1 : -1)) {
        if (U::g_ != 0) {
            os << (starting ? (U::g_ > 0 ? "" : "_1/") :
                              (U::g_ > 0 ? "*" : "/"));
        }
        os << "_g";
        starting = false;
    }
    for (int64 i = 0; i != U::m_; i += (U::m_ > 0 ? 1 : -1)) {
        if (U::m_ != 0) {
            os << (starting ? (U::m_ > 0 ? "" : "_1/") :
                              (U::m_ > 0 ? "*" : "/"));
        }
        os << "_m";
        starting = false;
    }
    for (int64 i = 0; i != U::s_; i += (U::s_ > 0 ? 1 : -1)) {
        if (U::s_ != 0) {
            os << (starting ? (U::s_ > 0 ? "" : "_1/") :
                              (U::s_ > 0 ? "*" : "/"));
        }
        os << "_s";
        starting = false;
    }
    for (int64 i = 0; i != U::A_; i += (U::A_ > 0 ? 1 : -1)) {
        if (U::A_ != 0) {
            os << (starting ? (U::A_ > 0 ? "" : "_1/") :
                              (U::A_ > 0 ? "*" : "/"));
        }
        os << "_A";
        starting = false;
    }
    for (int64 i = 0; i != U::K_; i += (U::K_ > 0 ? 1 : -1)) {
        if (U::K_ != 0) {
            os << (starting ? (U::K_ > 0 ? "" : "_1/") :
                              (U::K_ > 0 ? "*" : "/"));
        }
        os << "_K";
        starting = false;
    }
    for (int64 i = 0; i != U::mol_; i += (U::mol_ > 0 ? 1 : -1)) {
        if (U::mol_ != 0) {
            os << (starting ? (U::mol_ > 0 ? "" : "_1/") :
                              (U::mol_ > 0 ? "*" : "/"));
        }
        os << "_mol";
        starting = false;
    }
    for (int64 i = 0; i != U::cd_; i += (U::cd_ > 0 ? 1 : -1)) {
        if (U::cd_ != 0) {
            os << (starting ? (U::cd_ > 0 ? "" : "_1/") :
                              (U::cd_ > 0 ? "*" : "/"));
        }
        os << "_cd";
        starting = false;
    }
    os << ")>";
    if (!rhs.valid()) {
        os << "()";
    }
    else if (!rhs.finite()) {
        os << "::inf()";
    }
    else {
        os << "(" << (rhs.multiplier() < 0 ? -rhs.multiplier() : rhs.multiplier()) << ", " << rhs.precision() << ")";
    }
    return os;
}


}  // namespace

#endif
