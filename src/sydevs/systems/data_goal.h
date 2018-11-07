#pragma once
#ifndef SYDEVS_SYSTEMS_DATA_GOAL_H_
#define SYDEVS_SYSTEMS_DATA_GOAL_H_

#include <sydevs/core/number_types.h>
#include <string>

namespace sydevs {
namespace systems {


/**
 * @brief Indicates whether data is meant to serve as `input` or `output`.
 */
enum class data_goal : int8 {
    input,   ///< Indicates input data.
    output,  ///< Indicates output data.
};


const auto input = data_goal::input;    ///< Equivalent to `data_goal::input`.
const auto output = data_goal::output;  ///< Equivalent to `data_goal::output`.


const std::string& string_from_data_goal(data_goal dgoal);  ///< Returns the `std::string` representation of the `dgoal` value.

data_goal data_goal_from_string(const std::string& dgoal_string);  ///< Returns the `data_goal` value indicated by `dmode_string`.


/**
 * @brief Returns the integer representation of the `dgoal` value.
 */
inline constexpr int64 int64_from_data_goal(data_goal dgoal)
{
    return static_cast<std::underlying_type<data_goal>::type>(dgoal);
}


}  // namespace
}  // namespace

#endif
