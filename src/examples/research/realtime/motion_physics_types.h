#pragma once
#ifndef SYDEVS_EXAMPLES_MOTION_PHYSICS_TYPES_H_
#define SYDEVS_EXAMPLES_MOTION_PHYSICS_TYPES_H_

#include <sydevs/core/quantity.h>

namespace sydevs_examples {

using namespace sydevs;


using velocity = quantity<decltype(_m/_s)>;
using acceleration = quantity<decltype(_m/_s/_s)>;
using trajectory = std::tuple<distance, velocity, acceleration>;


}  // namespace

#endif
