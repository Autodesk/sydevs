#pragma once
#ifndef SYDEVS_RANGE_H_
#define SYDEVS_RANGE_H_

#include <sydevs/core/number_types.h>
#include <stdexcept>

namespace sydevs {


/**
 * @brief A data type which represents a range of array indices along a single
 *        dimension.
 *
 * @details
 * A `range` object can be used to create a slice of a multidimensional array
 * (`arraynd`) according to an arithmetic sequence of indices along a single
 * dimension.
 *
 * The arithmetic sequence represented by a `range` object starts either at or
 * immediately after a start index, increments according to a given stride, and
 * terminates either at or immediately before a stop index. The stop index is
 * permitted to be the maximum 64-bit signed integer (increasing sequences) or
 * the minimum 64-bit signed integer (decreasing sequences), indicating that the
 * sequence continues until the end of the array.
 *
 * A `range` object is defined using a chain of member function calls, as
 * demonstrated below.
 *
 * ~~~
 * range()                                          // 0, 1, 2, 3, ...
 * range().stop_before(7)                           // 0, 1, 2, 3, 4, 5, 6
 * range().start_at(2).stop_before(7)               // 2, 3, 4, 5, 6
 * range().start_at(2).stop_before(7).stride_by(2)  // 2, 4, 6
 *
 * range().start_after(7).stride_by(-1)             // 6, 5, 4, 3, ...
 * range().start_after(7).stop_at(0).stride_by(-1)  // 6, 5, 4, 3, 2, 1, 0
 * range().start_after(7).stop_at(0).stride_by(-3)  // 6, 3, 0
 * ~~~
 *
 * As in the above examples, it is recommended to use `start_at` and 
 * `stop_before` for increasing sequences and `start_after` and `stop_at` for
 * decreasing sequences. However, as demonstrated below, it is possible to
 * deviate from this convention.
 *
 * ~~~
 * range().start_after(4).stop_before(6)          // 5
 * range().start_at(9).stop_at(1).stride_by_(-5)  // 9, 5, 1
 * ~~~
 */
class range
{
public:
    /**
     * @brief Constructs a `range` object representing a sequence that starts 
     *        at 0 and increases by 1.
     */
    constexpr range();

private:
    constexpr range(int64 start_limit, int64 stop_limit, int64 stride, bool after, bool before);

public:
    constexpr range(const range&)  = default;  ///< Copy constructor
    range& operator=(const range&) = default;  ///< Copy assignment
    range(range&&)                 = default;  ///< Move constructor
    range& operator=(range&&)      = default;  ///< Move assignment
    ~range()                       = default;  ///< Destructor

    constexpr int64 start() const;   ///< Returns the first index in the sequence.
    constexpr int64 stop() const;    ///< Returns the last possible index in the sequence.
    constexpr int64 stride() const;  ///< Returns the increment between sequence indices.

    constexpr range start_at(int64 limit) const;     ///< Returns a new `range` object starting at `limit`.
    constexpr range start_after(int64 limit) const;  ///< Returns a new `range` object starting immediate after `limit`.

    constexpr range stop_at(int64 limit) const;      ///< Returns a new `range` object stopping at `limit`.
    constexpr range stop_before(int64 limit) const;  ///< Returns a new `range` object stopping immediate before `limit`.

    constexpr range stride_by(int64 stride) const;  ///< Returns a new `range` object incremending by `stride`.

private:
    int64 start_limit_;
    int64 stop_limit_;
    int64 stride_;
    bool after_;
    bool before_;
};


constexpr range::range()
    : start_limit_(0)
    , stop_limit_(std::numeric_limits<int64>::max())
    , stride_(1)
    , after_(false)
    , before_(false)
{
}


constexpr range::range(int64 start_limit, int64 stop_limit, int64 stride, bool after, bool before)
    : start_limit_(start_limit)
    , stop_limit_(stop_limit)
    , stride_(stride)
    , after_(after)
    , before_(before)
{
}


constexpr int64 range::start() const
{
    return start_limit_ + (after_ ? 2*int64(stride_ > 0) - 1 : 0);
}


constexpr int64 range::stop() const
{
    return stop_limit_ + (before_ ? 2*int64(stride_ < 0) - 1 : 0);
}


constexpr int64 range::stride() const
{
    return stride_;
}


constexpr range range::start_at(int64 limit) const
{
    return range(limit, stop_limit_, stride_, false, before_);
}


constexpr range range::start_after(int64 limit) const
{
    return range(limit, stop_limit_, stride_, true, before_);
}


constexpr range range::stop_at(int64 limit) const
{
    return range(start_limit_, limit, stride_, after_, false);
}


constexpr range range::stop_before(int64 limit) const
{
    return range(start_limit_, limit, stride_, after_, true);
}


constexpr range range::stride_by(int64 stride) const
{
    return range(start_limit_, 
                 (stride_ > 0 && stride < 0 && stop_limit_ == std::numeric_limits<int64>::max()) ? std::numeric_limits<int64>::min() :
                 (stride_ < 0 && stride > 0 && stop_limit_ == std::numeric_limits<int64>::min()) ? std::numeric_limits<int64>::max() :
                                                                                                   stop_limit_, 
                 stride, 
                 after_, 
                 before_);
}


}  // namespace

#endif