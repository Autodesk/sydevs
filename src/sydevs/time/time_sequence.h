#pragma once
#ifndef SYDEVS_TIME_SEQUENCE_H_
#define SYDEVS_TIME_SEQUENCE_H_

#include <sydevs/time/time_point.h>

namespace sydevs {


/**
 * @brief A data structure which represents a sequence of increasing time points.
 *
 * @details
 * A `time_sequence` object represents a strictly increasing sequence of 
 * `time_point` objects, saving space by storing `duration`-valued offsets. The
 * offsets are expressed relative to the beginning of each partition, and each
 * partition is associated with a single `time_point` object. Adding the
 * partition and the offset yields the represented time point. Thus, from the
 * outside, a time sequence appears as a vector containing a potentially large
 * number of time points, whereas on the inside it stores time points only for
 * partitions which are generally much smaller in number.
 *
 * The `time_sequence` class can help record time series that are either
 * supplied to simulation runs or result from them. The values associated with
 * these time series are not represented, but each time value has an index that
 * can be used to look up the corresponding value in another data structure. 
 *
 * While it is possible to access the i-th `time_point` object using `ts[i]`,
 * it is preferable to use iterators when possible. In particular, when
 * accessing time points in sequence (either forwards or backwards), the
 * iterator increment and decrement operations are more efficient than `ts[i]`
 * as the latter involves a search through the partitions. To make iterators
 * more convenient and thereby encourage their use, they provide both the time
 * point and the index through the methods `first` and `second`, similar to a
 * `std::map`. In addition to the `begin()` and `end()` methods, an iterator
 * can be obtained using `lower_bound` or `upper_bound`, which search for a
 * time point using the conventions of the standard C++ algorithm of the same
 * name. 
 *
 * A current limitation of a `time_sequence` object is that it can only be
 * modified by appending time points, since inserting or removing them might
 * affect the partitions. Below is an example where a time sequence is created,
 * three time points are added, and then each index and associated time point
 * is accessed in sequence using a C++11 range-based for loop.
 *
 * ~~~
 * auto ts = time_sequence();  // ts is an empty time sequence
 * auto tp = time_point();     // tp is 0
 * ts.append(tp += 5_us);      // tp becomes 5 microseconds, which is added to ts
 * ts.append(tp += 72_fs);     // tp increases by 72 femtoseconds, and is added to ts
 * ts.append(tp += 1_hr);      // tp increases by 3600 seconds, and is added to ts
 *
 * for (const auto& entry : ts) {     // iterate through a time sequence
 *     auto i = entry.first;          // current index
 *     const auto& t = entry.second;  // current time point
 * }
 * ~~~
 */
class time_sequence
{
public:
    class const_iterator;                             ///< The class used to iterate through each entry
    using value_type = std::pair<int64, time_point>;  ///< The type used to store each entry.

    time_sequence();  ///< Constructs an empty time sequence.

    time_sequence(const time_sequence&)            = default;  ///< Copy constructor
    time_sequence& operator=(const time_sequence&) = default;  ///< Copy assignment
    time_sequence(time_sequence&&)                 = default;  ///< Move constructor
    time_sequence& operator=(time_sequence&&)      = default;  ///< Move assignment
    ~time_sequence()                               = default;  ///< Destructor

    bool empty() const;  ///< Returns `true` if the time sequence is empty.
    int64 size() const;  ///< Returns the number of time points.

    void append(const time_point& tp);  ///< Appends `tp` if it exceeds all previously added time points. 

    const_iterator begin() const;  ///< Returns an iterator pointing to the oldest entry.
    const_iterator end() const;    ///< Returns an iterator pointing beyond the newest entry.

    const_iterator lower_bound(const time_point& tp) const;  ///< Returns an iterator to the first position `tp` could be inserted.
    const_iterator upper_bound(const time_point& tp) const;  ///< Returns an iterator to the last position `tp` could be inserted.

    const time_point front() const;                ///< Returns the first time point.
    const time_point back() const;                 ///< Returns the last time point.
    const time_point operator[](int64 rhs) const;  ///< Returns the time point associated with the index `rhs`.

    const std::vector<value_type>& partitions() const;  ///< Returns the partition indices and associated time points.

private:
    std::vector<value_type> partitions_;
    std::vector<duration> offsets_;
};


class time_sequence::const_iterator : public std::iterator<std::random_access_iterator_tag, const time_sequence::value_type, int64>
{
friend class time_sequence;
public:
    const time_sequence::value_type& operator*() const;
    const time_sequence::value_type* operator->() const; 
    const time_sequence::value_type operator[](int64 rhs) const;
    
    const_iterator& operator++();
    const_iterator operator++(int);
    const_iterator& operator--();
    const_iterator operator--(int);
    const_iterator& operator+=(int64 rhs);
    const_iterator& operator-=(int64 rhs);

    const_iterator operator+(int64 rhs) const;
    const_iterator operator-(int64 rhs) const;

    int64 operator-(const_iterator rhs) const;

    bool operator==(const const_iterator& rhs) const;
    bool operator!=(const const_iterator& rhs) const;
    bool operator<(const const_iterator& rhs) const;
    bool operator>(const const_iterator& rhs) const;
    bool operator<=(const const_iterator& rhs) const;
    bool operator>=(const const_iterator& rhs) const;
 
private:
    const_iterator(const time_sequence& ts, int64 pi, int64 ti);

    const time_sequence* ts_ptr_;
    int64 pi_;
    time_sequence::value_type entry_;
};
 

inline bool time_sequence::empty() const
{
    return partitions_.empty();
}


inline int64 time_sequence::size() const
{
    return int64(offsets_.size());
}


inline const std::vector<time_sequence::value_type>& time_sequence::partitions() const
{
    return partitions_;
}


}  // namespace

#endif
