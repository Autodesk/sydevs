#pragma once
#ifndef SYDEVS_STRING_BUILDER_H_
#define SYDEVS_STRING_BUILDER_H_

#include <sydevs/core/number_types.h>
#include <sstream>

namespace sydevs {


/**
 * @brief A class for converting stream objects to string values.
 *
 * @details
 * Several sydevs classes overload the `operator<<` to append a textual
 * representation of the class instance to a `std::ostream` object. The
 * `string_builder` class provides a convenient way to convert these
 * `std::ostream` objects into `std::string` values without requiring
 * additional lines of code. Primitives and other classes can also be converted
 * to strings.
 *
 * ~~~
 * std::string s1 = (string_builder() << "n:" << (3 + 5)).str();          // s1 = "n:8"
 * std::string s2 = (string_builder() << "t:" << (1_min + 7_sec)).str();  // s2 = "t:67_s"
 * ~~~
 */
class string_builder
{
public:
    /**
     * @brief Constructs a `string_builder` object.
     */
    string_builder();

    string_builder(const string_builder&)            = delete;   ///< No copy constructor
    string_builder& operator=(const string_builder&) = delete;   ///< No copy assignment
    string_builder(string_builder&&)                 = delete;   ///< No move constructor
    string_builder& operator=(string_builder&&)      = delete;   ///< No move assignment
    ~string_builder()                                = default;  ///< Destructor

    /**
     * @brief Obtains the data stored in the `string_builder` object as a `std::string`.
     */
    std::string str() const;

    /**
     * @brief Inserts data into the stream.
     * 
     * @details
     * If an operation such as `operator<<(std::ostream&, const T&)` is defined
     * for type `T`, then a copy of the information in `value` is inserted into
     * the `string_builder` object for later retrieval via `str()`.
     *
     * @param value The value to be inserted. 
     *
     * @return A reference to the `string_builder` object, allowing multiple
     *         values to be inserted in a single line.
     */
    template<class T>
    string_builder& operator<<(T const& value);

private:
    std::stringstream stream_;
};


inline string_builder::string_builder()
    : stream_()
{
}


inline std::string string_builder::str() const
{
    return stream_.str();
}


template<class T>
inline string_builder& string_builder::operator<<(T const& value)
{
    stream_ << value;
    return *this;
}


}  // namespace

#endif
