#include <sydevs/systems/data_mode.h>
#include <array>
#include <algorithm>

namespace sydevs {
namespace systems {


const std::array<data_mode, 2> data_modes = {
    data_mode::flow,
    data_mode::message,
};


const std::array<std::string, 11> data_mode_strings = {
    "flow",
    "message",
};

    
const std::string& string_from_data_mode(data_mode dmode)
{
    return data_mode_strings[int64_from_data_mode(dmode)];
}


data_mode data_mode_from_string(const std::string& dmode_string)
{
    auto iter = std::find(std::begin(data_mode_strings), std::end(data_mode_strings), dmode_string);
    if (iter == std::end(data_mode_strings)) throw std::out_of_range("Unrecognized data mode string: " + dmode_string);
    return data_modes[iter - std::begin(data_mode_strings)];
}


}  // namespace
}  // namespace
