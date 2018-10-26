#pragma once
#ifndef SYDEVS_EXAMPLES_ADVANCED_BUILDING_OCCUPANT_IDS_H_
#define SYDEVS_EXAMPLES_ADVANCED_BUILDING_OCCUPANT_IDS_H_

#include <sydevs/core/qualified_type.h>

namespace sydevs_examples {

using namespace sydevs;


class occupant_unit;

using occupant_id = identity<occupant_unit>;


}  // namespace

#endif
