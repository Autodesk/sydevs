#pragma once
#ifndef SYDEVS_SYSTEMS_DATA_MODE_H_
#define SYDEVS_SYSTEMS_DATA_MODE_H_

#include <sydevs/core/number_types.h>
#include <string>

namespace sydevs {
namespace systems {


/**
 * @brief Indicates the relevant data communication paradigm: dataflow (`flow`)
 *        or message-passing (`message`).
 */
enum class data_mode : int8 {
    flow,     ///< Indicates dataflow.
    message,  ///< Indicates message-passing.
};


const auto flow = data_mode::flow;        ///< Equivalent to `data_mode::flow`.
const auto message = data_mode::message;  ///< Equivalent to `data_mode::message`.


const std::string& string_from_data_mode(data_mode dmode);  ///< Returns the `std::string` representation of the `dmode` value.

data_mode data_mode_from_string(const std::string& dmode_string);   ///< Returns the `data_mode` value indicated by `dmode_string`.


/**
 * @brief Returns the integer representation of the `dmode` value.
 */
inline constexpr int64 int64_from_data_mode(data_mode dmode)
{
    return static_cast<std::underlying_type<data_mode>::type>(dmode);
}


}  // namespace
}  // namespace

#endif
