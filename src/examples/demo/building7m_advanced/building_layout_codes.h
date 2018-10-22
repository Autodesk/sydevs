#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_LAYOUT_CODES_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_LAYOUT_CODES_H_

#include <sydevs/core/number_types.h>

namespace sydevs_examples {

using namespace sydevs;


const int64 indoor_code = 0;    // code for grid cells associated with indoor space
const int64 wall_code = 1;      // code for grid cells associated with walls
const int64 outdoor_code = -1;  // code for grid cells associated with outdoor space


}  // namespace

#endif
