#pragma once
#ifndef SYDEVS_TIME_POINT_H_
#define SYDEVS_TIME_POINT_H_

#include <sydevs/core/quantity.h>
#include <stdexcept>
#include <vector>

namespace sydevs {


/**
 * @brief A data structure which represents a point in time as an 
 *        arbitrary-precision multiple of its shortest time precision.
 *
 * @details
 * A `time_point` object represents a base-1000 duration of time, allowing for
 * exact accumulation of finite durations with essentially no bounds on range
 * or resolution. One either default constructs a zero time point, or 
 * constructs a time point based on an initial `duration` value offset. 
 * Thereafter, the time point is adjusted by adding or subtracting other
 * durations. If these durations vary significantly in scale, the time point
 * will grow in memory to accommodate their exact sum. Hence the main 
 * differences between `duration` values and `time_point` objects is that the
 * former are more efficient but the latter are more precise. 
 *
 * Below is an example of time points in use.
 *
 * ~~~
 * auto tp = time_point();
 * tp += 5000388_ns;
 * tp += 1777_ps;
 * tp += 640_us;
 * tp -= 40109_ns;
 * tp -= 5_ms;
 * tp += 223_ps;
 * ~~~
 *
 * Internally, the `time_point` class stores information as a vector containing
 * an 8-bit integer "digit" per necessary base-1000 `scale` value. This digit
 * can be obtained with the `scale_digit` member function. The `scale_phase`
 * function is similar, but gives the scale-specific quantity measured from the
 * left (i.e. from negative infinity) rather than from zero. The `precision` 
 * function yields the shortest scale with a non-zero digit, while the longest
 * sucb precision can be obtained by adding the result of `nscales` minus one.
 *
 * Given two `time_point` objects, the difference between them can be obtained
 * as a duration in one of two ways. The first is regular substraction, which
 * yields the exact result if possible or otherwise an infinite value. The 
 * second way of obtaining a difference is the `gap` member function, which 
 * approximates the result if necessary. The result of `gap` is always accurate
 * to one of its time precisions.
 * 
 * ~~~
 * auto lhs = time_point() + 7_s + 3_ms + 5_us + 6_ns + 2_ps + 9_fs;
 * auto rhs = time_point() + 6_s
 *
 * (lhs - rhs) == duration::inf();    // true
 * lhs.gap(rhs) == 1003005006002_ps;  // true
 * ~~~
 *
 * Time also provide an alternative to the regular accumulation of `duration`
 * values. The alternative is called "multiscale time advancement", and
 * involves accumulation followed by truncation. The truncation is based on the
 * time precision of the duration.
 *
 * ~~~
 * auto tp = time_point(72800444321_ns);
 *
 * tp + 1150_ms;  // result: time_point() + 73_s + 950_ms + 444_us + 321_ns
 *                // (tp is not modified)
 *
 * tp.advance(1150_ms);  // result: time_point() + 73_s + 950_ms
 *                       // (microseconds and nanoseconds are truncated)
 *                       // (tp is modified)
 * ~~~
 *
 * Time points can be compared with other time points or with `duration`
 * values, with duration values appearing on the left- or right-hand side.
 *
 * A `time_point` object may be output or converted to a string using the
 * `operator<<` overload.
 */
class time_point
{
public:
    /**
     * @brief Constructs a zero `time_point` object.
     */
    time_point();

    /**
     * @brief Constructs a `time_point` object with the initial offset
     *        indicated by the `duration` argument.
     * 
     * @details
     * The constructed `time_point` object represents the specified offset from
     * time zero, which can then be modified using the `+=` or `-=` operators
     * or the `advance` member function.
     * 
     * @param dt The initial offset.
     */
    explicit time_point(duration dt);

    time_point(const time_point&)            = default;  ///< Copy constructor
    time_point& operator=(const time_point&) = default;  ///< Copy assignment
    time_point(time_point&&)                 = default;  ///< Move constructor
    time_point& operator=(time_point&&)      = default;  ///< Move assignment
    ~time_point()                            = default;  ///< Destructor

    int64 sign() const;       ///< Returns the sign (+1 or -1) of the `time_point` object.
    scale precision() const;  ///< Returns the precision of the shortest non-zero digit.
    int64 nscales() const;    ///< Returns the length of the vector of digits.

    int64 scale_digit(scale precision) const;  ///< Returns the digit associated with the `precision` scale.
    int64 scale_phase(scale precision) const;  ///< Returns the single-scale offset from the left.
    int64 epoch_phase(scale precision) const;  ///< Returns the 5-scale (epoch) offset from the left.

    /**
     * @brief Advances the `time_point` object by the specified duration.
     * 
     * @details
     * Adds the duration `rhs` to the time point, then truncates the result
     * such that offsets of precisions shorter than `rhs.precision` are
     * discarded.
     *
     * @param rhs The duration to advance by.
     *
     * @return A reference to the modified `time_point` object.
     */
    time_point& advance(duration rhs);

    time_point& operator+=(duration rhs);  ///< Modifies the `time_point` object by adding the duration `rhs`.
    time_point& operator-=(duration rhs);  ///< Modifies the `time_point` object by subtracting the duration `rhs`.

    const time_point operator+(duration rhs) const;  ///< Returns a new `time_point` with duration `rhs` added.
    const time_point operator-(duration rhs) const;  ///< Returns a new `time_point` with duration `rhs` subtracted.

    /**
     * @brief Calculates the exact difference between the `time_point` object
     *        and time_point `rhs`.
     * 
     * @details
     * If a `duration` value lacks the precision required to represent the
     * exact difference, then the result is either positive infinity (if `rhs`
     * is a lesser time point) or negative infinity (if `rhs` is greater).
     *
     * @param rhs The time point to subtract.
     *
     * @return The difference from `rhs` or an infinite duration.
     */
    const duration operator-(const time_point& rhs) const;

    /**
     * @brief Approximates the difference between the `time_point` object and
     *        time_point `rhs`.
     * 
     * @details
     * If a `duration` value lacks the precision required to represent the
     * exact difference, then the result is rounded. The exact difference 
     * should be within one precision of the returned duration, in terms of the
     * return value's time precision.
     *
     * @param rhs The time point to subtract.
     *
     * @return The approximate difference from `rhs`.
     */
    duration gap(const time_point& rhs) const;

    bool operator==(const time_point& rhs) const;  ///< Returns `true` if the `time_point` object equals `rhs`.
    bool operator!=(const time_point& rhs) const;  ///< Returns `true` if the `time_point` object does not equal `rhs`.
    bool operator<(const time_point& rhs) const;   ///< Returns `true` if the `time_point` object is less than `rhs`.
    bool operator>(const time_point& rhs) const;   ///< Returns `true` if the `time_point` object is greater than `rhs`.
    bool operator<=(const time_point& rhs) const;  ///< Returns `true` if the `time_point` object is at most `rhs`.
    bool operator>=(const time_point& rhs) const;  ///< Returns `true` if the `time_point` object is at least `rhs`.

    bool operator==(duration rhs) const;  ///< Returns `true` if the `time_point` object equals duration `rhs`.
    bool operator!=(duration rhs) const;  ///< Returns `true` if the `time_point` object does not equal duration `rhs`.
    bool operator<(duration rhs) const;   ///< Returns `true` if the `time_point` object is less than duration `rhs`.
    bool operator>(duration rhs) const;   ///< Returns `true` if the `time_point` object is greater than duration `rhs`.
    bool operator<=(duration rhs) const;  ///< Returns `true` if the `time_point` object is at most duration `rhs`.
    bool operator>=(duration rhs) const;  ///< Returns `true` if the `time_point` object is at least duration `rhs`.

private:
    scale upper_discrepant_precision(const time_point& rhs) const;

    int8 sign_;
    scale precision_;
    std::vector<int16> digits_;
};


std::ostream& operator<<(std::ostream& os, const time_point& rhs);
    
    
bool operator==(duration lhs, const time_point& rhs);
bool operator!=(duration lhs, const time_point& rhs);
bool operator<(duration lhs, const time_point& rhs);
bool operator>(duration lhs, const time_point& rhs);
bool operator<=(duration lhs, const time_point& rhs);
bool operator>=(duration lhs, const time_point& rhs);


inline int64 time_point::sign() const
{
    return sign_;
}


inline scale time_point::precision() const
{
    return precision_;
}


inline int64 time_point::nscales() const
{
    return digits_.size();
}


}  // namespace

#endif
