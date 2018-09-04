| [Prev](part05.html) | [Getting Started -- Table of Contents](index.html) | [Next](part07.html) |
# Part 6: Incorporating Additional Examples

Let's extend the `sydevs-examples` project with two existing examples.

1. Return to the [SyDEVS releases](https://github.com/Autodesk/sydevs/releases) page and download `SyDEVS-v0.4_demo-examples.zip` (instead of v0.5, download the version of `demo-examples` that corresponds with the SyDEVS library you selected in [Part 1](part01.html)).
2. Extract the archive, find the folder named `demo` inside, and copy it into your `sydevs-examples/src/examples` directory.
2. In the `demo/queueing` folder, find `main.cpp` and move it into a new `queueing_simulation` folder within `sydevs-examples/src/simulations`.
3. In the `demo/building7m` folder, find `main.cpp` and move it into a new `building7m_simulation` folder within `sydevs-examples/src/simulations`.

You should now have the following directory structure.

```
sydevs-examples/
    bin/
        ...
    external/
        ...
    src/
        examples/
            demo/
                building7m/
                queueing7/
            getting_started/
                waveform/
        simulations/
            building7m_simulation/
            first_simulation/
            queueing_simulation/
            setting_up/
            simulation_with_ports/
```

Open `CMakeLists.txt` and add the following instructions to the `Examples` section.

```cmake
set(QUEUEING_DIR ${EXAMPLES_DIR}/demo/queueing)
file(GLOB QUEUEING_HDRS "${QUEUEING_DIR}/*.h")

set(BUILDING7M_DIR ${EXAMPLES_DIR}/demo/building7m)
file(GLOB BUILDING7M_HDRS "${BUILDING7M_DIR}/*.h")
```

Also add the following to the `Simulations` section at the bottom, and save.

```cmake
set(QUEUEING_SIMULATION_DIR ${SIMULATIONS_DIR}/queueing_simulation)
aux_source_directory(${QUEUEING_SIMULATION_DIR} QUEUEING_SIMULATION_SRCS)
add_executable(queueing_simulation ${QUEUEING_SIMULATION_SRCS} ${QUEUEING_HDRS})
target_link_libraries(queueing_simulation debug SyDEVS-static-debug optimized SyDEVS-static)

set(BUILDING7M_SIMULATION_DIR ${SIMULATIONS_DIR}/building7m_simulation)
aux_source_directory(${BUILDING7M_SIMULATION_DIR} BUILDING7M_SIMULATION_SRCS)
add_executable(building7m_simulation ${BUILDING7M_SIMULATION_SRCS} ${BUILDING7M_HDRS})
target_link_libraries(building7m_simulation debug SyDEVS-static-debug optimized SyDEVS-static)
add_custom_command(TARGET building7m_simulation POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
                   ${PROJECT_SOURCE_DIR}/${BUILDING7M_DIR}/building7m.png
                   $<TARGET_FILE_DIR:building7m_simulation>/.)
```

When the project is re-built, there should be an executable named `queueing_simulation` which produces the following results.

```
Testing queueing_node
1|0|$time:time_point() + 5_s
1|2|top.queue#job_id_input:0
2|0|$time:time_point() + 10_s
2|2|top.queue#job_id_input:1
3|0|$time:time_point() + 15_s
3|2|top.queue#job_id_input:2
4|0|$time:time_point() + 19_s
4|1|top.queue#job_id_output:0
...                                     [49 lines omitted]
29|0|$time:time_point() + 115_s
29|2|top.queue#job_id_input:22
30|0|$time:time_point() + 117_s
30|1|top.queue#job_id_output:7
Idle time: 5_s

Testing two_stage_queueing_node
1|0|$time:time_point() + 5_s
1|2|top.queue#job_id_input:0
2|0|$time:time_point() + 10_s
2|2|top.queue#job_id_input:1
3|0|$time:time_point() + 15_s
3|2|top.queue#job_id_input:2
4|0|$time:time_point() + 19_s
4|2|top.queue.queue_A#job_id_output:0
...                                     [55 lines omitted]
29|0|$time:time_point() + 115_s
29|2|top.queue#job_id_input:22
30|0|$time:time_point() + 117_s
30|2|top.queue.queue_A#job_id_output:7
30|8|top.queue.queue_B#job_id_output:6
Idle time: 24_s

Testing parallel_queueing_node
0|0|$time:time_point()
0|6|top.queue.0$initialization
1|0|$time:time_point() + 5_s
1|3|top.queue.0$unplanned
1|3|top.queue.0#job_id_input:0
2|0|$time:time_point() + 10_s
2|3|top.queue.0$unplanned
2|3|top.queue.0#job_id_input:1
...                                     [112 lines omitted]
37|0|$time:time_point() + 120_s
37|3|top.queue.0$finalization
37|4|top.queue.1$finalization
37|5|top.queue.2$finalization
Idle time: 18_s
```

There should also be an executable named `building7m_simulation` which produces the output below.

```
0|0|$time:time_point()
0|1|top.initial_temperature#parameter:293150_mK
0|9|top.building_dynamics.occupant$print:quantity<decltype(_s*_K)>(0, unit)
0|9|top.building_dynamics.occupant$print:0_us
165|0|$time:time_point() + 15_s
165|2|top.building_dynamics.weather#outdoor_temperature_output:296150_mK
330|0|$time:time_point() + 30_s
330|2|top.building_dynamics.weather#outdoor_temperature_output:298854_mK
495|0|$time:time_point() + 45_s
495|2|top.building_dynamics.weather#outdoor_temperature_output:300995_mK
660|0|$time:time_point() + 60_s
660|2|top.building_dynamics.weather#outdoor_temperature_output:302362_mK
825|0|$time:time_point() + 75_s
825|2|top.building_dynamics.weather#outdoor_temperature_output:302819_mK
990|0|$time:time_point() + 90_s
990|2|top.building_dynamics.weather#outdoor_temperature_output:302322_mK
1155|0|$time:time_point() + 105_s
1155|2|top.building_dynamics.weather#outdoor_temperature_output:300920_mK
1320|0|$time:time_point() + 120_s
1320|2|top.building_dynamics.weather#outdoor_temperature_output:298751_mK
1485|0|$time:time_point() + 135_s
1485|2|top.building_dynamics.weather#outdoor_temperature_output:296029_mK
1650|0|$time:time_point() + 150_s
1650|2|top.building_dynamics.weather#outdoor_temperature_output:293023_mK
1815|0|$time:time_point() + 165_s
1815|2|top.building_dynamics.weather#outdoor_temperature_output:290030_mK
1980|0|$time:time_point() + 180_s
1980|2|top.building_dynamics.weather#outdoor_temperature_output:287345_mK
2145|0|$time:time_point() + 195_s
2145|2|top.building_dynamics.weather#outdoor_temperature_output:285233_mK
2310|0|$time:time_point() + 210_s
2310|2|top.building_dynamics.weather#outdoor_temperature_output:283902_mK
2475|0|$time:time_point() + 225_s
2475|2|top.building_dynamics.weather#outdoor_temperature_output:283484_mK
2640|0|$time:time_point() + 240_s
2640|2|top.building_dynamics.weather#outdoor_temperature_output:284020_mK
2805|0|$time:time_point() + 255_s
2805|2|top.building_dynamics.weather#outdoor_temperature_output:285457_mK
2970|0|$time:time_point() + 270_s
2970|2|top.building_dynamics.weather#outdoor_temperature_output:287653_mK
3135|0|$time:time_point() + 285_s
3135|2|top.building_dynamics.weather#outdoor_temperature_output:290392_mK
```

The queueing example is notable in that it features a commented header file demonstrating each of the four types of nodes.

- Atomic Node: [`queueing_node.h`](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/queueing_node.h)
- Composite Node: [`two_stage_queueing_node.h`](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/two_stage_queueing_node.h)
- Collection Node: [`parallel_queueing_node.h`](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/parallel_queueing_node.h)
- Function Node: [`plus_node.h`](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/plus_node.h)

The building7m (7-meter building) example demonstrates a number of features of the SyDEVS library.

- Composite Node with Ports and Inward/Outward Links: [`building_dynamics_node.h`](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/building7m/building_dynamics_node.h)
- Composite Node containing a Composite Node (hierarchy): [`building_closed_system.h`](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/building7m/building_closed_system.h)
- Multidimensional Arrays: [`thermodynamics_node.h`](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/building7m/thermodynamics_node.h)
- Standard International (SI) Units: [`occupant_node.h`](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/building7m/occupant_node.h)

Take a few minutes to familiarize yourself with these examples, then proceed to the final part of the tutorial. In [Part 7](part07.html), you will be challenged to make several enhancements to the `building7m` example.

| [***Continue to Part 7***](part07.html) |


