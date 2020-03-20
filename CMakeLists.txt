################################################################################
# CMake File for SyDEVS
################################################################################
cmake_minimum_required(VERSION 3.4)
project(SyDEVS)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

if(MSVC)
    add_definitions("/wd4244") # Hide type conversion warnings
    add_compile_options(/MP)
    add_compile_options(/std=c++${CMAKE_CXX_STANDARD})
    string(REGEX REPLACE "/Z[iI7]" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7 /bigobj")
else()
    set(WARNING_FLAGS "-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-missing-braces -Wignored-qualifiers -Woverloaded-virtual -Winline")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -std=c++${CMAKE_CXX_STANDARD} ${WARNING_FLAGS}")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	    # Replace g++ standard library with Clang standard library
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
endif(MSVC)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX -debug)
endif()

include_directories(src)
 
# ------------------------------------------------------------------------------
#
#   SyDEVS Library
#
# ------------------------------------------------------------------------------
set(CORE_DIR src/sydevs/core)
file(GLOB CORE_HDRS "${CORE_DIR}/*.h")
aux_source_directory(${CORE_DIR} CORE_SRCS)
source_group("core-include" FILES ${CORE_HDRS})
source_group("core-src" FILES ${CORE_SRCS})

set(TIME_DIR src/sydevs/time)
file(GLOB TIME_HDRS "${TIME_DIR}/*.h")
aux_source_directory(${TIME_DIR} TIME_SRCS)
source_group("time-include" FILES ${TIME_HDRS})
source_group("time-src" FILES ${TIME_SRCS})

set(SYSTEMS_DIR src/sydevs/systems)
file(GLOB SYSTEMS_HDRS "${SYSTEMS_DIR}/*.h")
aux_source_directory(${SYSTEMS_DIR} SYSTEMS_SRCS)
source_group("systems-include" FILES ${SYSTEMS_HDRS})
source_group("systems-src" FILES ${SYSTEMS_SRCS})

add_library(SyDEVS-static STATIC ${CORE_SRCS} ${TIME_SRCS} ${SYSTEMS_SRCS} ${CORE_HDRS} ${TIME_HDRS} ${SYSTEMS_HDRS})
add_library(SyDEVS SHARED ${CORE_SRCS} ${TIME_SRCS} ${SYSTEMS_SRCS} ${CORE_HDRS} ${TIME_HDRS} ${SYSTEMS_HDRS})

# ------------------------------------------------------------------------------
#
#   Multidimensional array examples
#
# ------------------------------------------------------------------------------
set(PERFORMANCE_EXAMPLES_DIR src/examples/test_performance)

set(MULTIDIMENSIONAL_ARRAY_EXAMPLES_DIR ${PERFORMANCE_EXAMPLES_DIR}/multidimensional_arrays)
aux_source_directory(${MULTIDIMENSIONAL_ARRAY_EXAMPLES_DIR} ARRAY_PERFORMANCE_SRCS)
add_executable(array_performance ${ARRAY_PERFORMANCE_SRCS})
target_link_libraries(array_performance)

# ------------------------------------------------------------------------------
#
#   Systems examples
#
# ------------------------------------------------------------------------------
set(SYSTEMS_EXAMPLES_DIR src/examples/test_systems)

set(HELLO_SYSTEMS_DIR ${SYSTEMS_EXAMPLES_DIR}/hello)
file(GLOB HELLO_SYSTEMS_HDRS "${HELLO_SYSTEMS_DIR}/*.h")
aux_source_directory(${HELLO_SYSTEMS_DIR} HELLO_SYSTEMS_SRCS)
add_executable(hello_systems ${HELLO_SYSTEMS_SRCS} ${HELLO_SYSTEMS_HDRS})
target_link_libraries(hello_systems SyDEVS-static)

set(BASIC_SYSTEMS_DIR ${SYSTEMS_EXAMPLES_DIR}/basic)
file(GLOB BASIC_SYSTEMS_HDRS "${BASIC_SYSTEMS_DIR}/*.h")
aux_source_directory(${BASIC_SYSTEMS_DIR} BASIC_SYSTEMS_SRCS)
add_executable(basic_systems ${BASIC_SYSTEMS_SRCS} ${BASIC_SYSTEMS_HDRS})
target_link_libraries(basic_systems SyDEVS-static)

set(SORTING_SYSTEMS_DIR ${SYSTEMS_EXAMPLES_DIR}/sorting)
file(GLOB SORTING_SYSTEMS_HDRS "${SORTING_SYSTEMS_DIR}/*.h")
aux_source_directory(${SORTING_SYSTEMS_DIR} SORTING_SYSTEMS_SRCS)
add_executable(sorting_systems ${SORTING_SYSTEMS_SRCS} ${SORTING_SYSTEMS_HDRS})
target_link_libraries(sorting_systems SyDEVS-static)

set(DATA_SYSTEMS_DIR ${SYSTEMS_EXAMPLES_DIR}/data)
file(GLOB DATA_SYSTEMS_HDRS "${DATA_SYSTEMS_DIR}/*.h")
aux_source_directory(${DATA_SYSTEMS_DIR} DATA_SYSTEMS_SRCS)
add_executable(data_systems ${DATA_SYSTEMS_SRCS} ${DATA_SYSTEMS_HDRS})
target_link_libraries(data_systems SyDEVS-static)

set(AGENTBASED_SYSTEMS_DIR ${SYSTEMS_EXAMPLES_DIR}/agentbased)
file(GLOB AGENTBASED_SYSTEMS_HDRS "${AGENTBASED_SYSTEMS_DIR}/*.h")
aux_source_directory(${AGENTBASED_SYSTEMS_DIR} AGENTBASED_SYSTEMS_SRCS)
add_executable(agentbased_systems ${AGENTBASED_SYSTEMS_SRCS} ${AGENTBASED_SYSTEMS_HDRS})
target_link_libraries(agentbased_systems SyDEVS-static)

set(CELLULAR_SYSTEMS_DIR ${SYSTEMS_EXAMPLES_DIR}/cellular)
file(GLOB CELLULAR_SYSTEMS_HDRS "${CELLULAR_SYSTEMS_DIR}/*.h")
aux_source_directory(${CELLULAR_SYSTEMS_DIR} CELLULAR_SYSTEMS_SRCS)
add_executable(cellular_systems ${CELLULAR_SYSTEMS_SRCS} ${CELLULAR_SYSTEMS_HDRS})
target_link_libraries(cellular_systems SyDEVS-static)

# ------------------------------------------------------------------------------
#
#   Research examples
#
# ------------------------------------------------------------------------------
set(RESEARCH_EXAMPLES_DIR src/examples/research)

set(CUSTOMS_DIR ${RESEARCH_EXAMPLES_DIR}/customs)
file(GLOB CUSTOMS_HDRS "${CUSTOMS_DIR}/*.h")
aux_source_directory(${CUSTOMS_DIR} CUSTOMS_SRCS)
add_executable(customs ${CUSTOMS_SRCS} ${CUSTOMS_HDRS})
target_link_libraries(customs SyDEVS-static)

set(PREDICTION_DIR ${RESEARCH_EXAMPLES_DIR}/prediction)
file(GLOB PREDICTION_HDRS "${PREDICTION_DIR}/*.h")
aux_source_directory(${PREDICTION_DIR} PREDICTION_SRCS)
add_executable(prediction ${PREDICTION_SRCS} ${PREDICTION_HDRS})
target_link_libraries(prediction SyDEVS-static)

set(REALTIME_DIR ${RESEARCH_EXAMPLES_DIR}/realtime)
file(GLOB REALTIME_HDRS "${REALTIME_DIR}/*.h")
aux_source_directory(${REALTIME_DIR} REALTIME_SRCS)
add_executable(realtime ${REALTIME_SRCS} ${REALTIME_HDRS})
target_link_libraries(realtime SyDEVS-static)

set(SUBTYPING_DIR ${RESEARCH_EXAMPLES_DIR}/subtyping)
file(GLOB SUBTYPING_HDRS "${SUBTYPING_DIR}/*.h")
aux_source_directory(${SUBTYPING_DIR} SUBTYPING_SRCS)
add_executable(subtyping ${SUBTYPING_SRCS} ${SUBTYPING_HDRS})
target_link_libraries(subtyping SyDEVS-static)

set(BUILDING7M_ADVANCED_DIR ${RESEARCH_EXAMPLES_DIR}/building7m_advanced)
file(GLOB BUILDING7M_ADVANCED_HDRS "${BUILDING7M_ADVANCED_DIR}/*.h")
aux_source_directory(${BUILDING7M_ADVANCED_DIR} BUILDING7M_ADVANCED_SRCS)
add_executable(building7m_advanced ${BUILDING7M_ADVANCED_SRCS} ${BUILDING7M_ADVANCED_HDRS})
target_link_libraries(building7m_advanced SyDEVS-static)

# ------------------------------------------------------------------------------
#
#   Demo examples
#
# ------------------------------------------------------------------------------
set(DEMO_EXAMPLES_DIR src/examples/demo)

set(BUILDING7M_DIR ${DEMO_EXAMPLES_DIR}/building7m)
file(GLOB BUILDING7M_HDRS "${BUILDING7M_DIR}/*.h")
aux_source_directory(${BUILDING7M_DIR} BUILDING7M_SRCS)
add_executable(building7m ${BUILDING7M_SRCS} ${BUILDING7M_HDRS})
target_link_libraries(building7m SyDEVS-static)
add_custom_command(TARGET building7m POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
                   ${PROJECT_SOURCE_DIR}/${BUILDING7M_DIR}/building7m.png
                   $<TARGET_FILE_DIR:building7m>/.)

set(QUEUEING_DIR ${DEMO_EXAMPLES_DIR}/queueing)
file(GLOB QUEUEING_HDRS "${QUEUEING_DIR}/*.h")
aux_source_directory(${QUEUEING_DIR} QUEUEING_SRCS)
add_executable(queueing ${QUEUEING_SRCS} ${QUEUEING_HDRS})
target_link_libraries(queueing SyDEVS-static)

set(WORKERS_DIR ${DEMO_EXAMPLES_DIR}/workers)
file(GLOB WORKERS_HDRS "${WORKERS_DIR}/*.h")
aux_source_directory(${WORKERS_DIR} WORKERS_SRCS)
add_executable(workers ${WORKERS_SRCS} ${WORKERS_HDRS})
target_link_libraries(workers SyDEVS-static)

# ------------------------------------------------------------------------------
#
#   Unit tests
#
# ------------------------------------------------------------------------------
set(UNIT_TEST_DIR test/unit_tests)
set(CATCH2_DIR ${UNIT_TEST_DIR}/catch2)
include_directories(${UNIT_TEST_DIR})
include_directories(${CATCH2_DIR})
file(GLOB CATCH2_HDRS "${CATCH2_DIR}/*.h" "${CATCH2_DIR}/*.hpp")
aux_source_directory(${UNIT_TEST_DIR} TEST_SRCS)
add_executable(unit_tests ${TEST_SRCS} ${CATCH2_HDRS})
target_link_libraries(unit_tests SyDEVS-static)
add_custom_command(TARGET unit_tests POST_BUILD COMMAND unit_tests COMMENT "Running unit tests")

# ------------------------------------------------------------------------------
#
#   Regression tests
#
# ------------------------------------------------------------------------------
set(REGRESSION_TEST_DIR test/regression_tests)
include_directories(${REGRESSION_TEST_DIR})
file(GLOB REGRESSION_TEST_HDRS "${REGRESSION_TEST_DIR}/*.h")
aux_source_directory(${REGRESSION_TEST_DIR} REGRESSION_TEST_SRCS)

# Remove main.cpp from WORKERS_SRCS to allow the source list to be linked into unit/regression test executable
list(REMOVE_ITEM WORKERS_SRCS src/examples/demo/workers/main.cpp)

# Build regression test executable
add_executable(regression_tests ${REGRESSION_TEST_SRCS} ${REGRESSION_TEST_HDRS} ${WORKERS_SRCS})
target_link_libraries(regression_tests SyDEVS-static)

# Run regression tests
add_custom_command(TARGET regression_tests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory regression_test_data)
add_custom_command(TARGET regression_tests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory regression_test_data/old_data)
add_custom_command(TARGET regression_tests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory regression_test_data/new_data)
file(GLOB RTESTS_DATA ${REGRESSION_TEST_DIR}/regression_test_data/*.txt)
add_custom_command(TARGET regression_tests PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
                   ${RTESTS_DATA}
                   regression_test_data/old_data/.)
add_custom_command(TARGET regression_tests POST_BUILD COMMAND regression_tests COMMENT "Running regression tests")
