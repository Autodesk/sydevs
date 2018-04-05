#include <sydevs/systems/data_goal.h>
#include <array>
#include <algorithm>

namespace sydevs {
namespace systems {


const std::array<data_goal, 2> data_goals = {
    data_goal::input,
    data_goal::output,
};


const std::array<std::string, 11> data_goal_strings = {
    "input",
    "output",
};

    
const std::string& string_from_data_goal(data_goal dgoal)
{
    return data_goal_strings[int64_from_data_goal(dgoal)];
}


data_goal data_goal_from_string(const std::string& dgoal_string)
{
    auto iter = std::find(std::begin(data_goal_strings), std::end(data_goal_strings), dgoal_string);
    if (iter == std::end(data_goal_strings)) throw std::out_of_range("Unrecognized data goal string: " + dgoal_string);
    return data_goals[iter - std::begin(data_goal_strings)];
}


}  // namespace
}  // namespace
