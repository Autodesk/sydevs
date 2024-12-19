| [Prev](part05.html) | [Getting Started -- Table of Contents](index.html) | [Next](part07.html) |
# Part 6: Incorporating Additional Examples

Let's extend the **sydevs-tutorial** project with existing examples.

1. Copy the **demo** folder in the **external/sydevs-[version]/src/examples** directory, and paste the folder into the **src/nodes** directory.
1. In the **src/nodes/demo/building7m** folder, find **main.cpp** and move it into a new **building7m_simulation** folder within **src/simulations**. Open the file and in the `#include` statement, replace the text `examples` with `nodes`.
1. In the **src/nodes/demo/queueing** folder, find **main.cpp** and move it into a new **queueing_simulation** folder within **src/simulations**. Open the file and in the `#include` statement, replace the text `examples` with `nodes`.
1. In the **src/nodes/demo/workers** folder, find **main.cpp** and move it into a new **workers_simulation** folder within **src/simulations**. Open the file and in the `#include` statement, replace the text `examples` with `nodes`.

You should now have the following directory structure.

```
sydevs-examples/
    build/
        ...
    external/
        ...
    src/
        examples/
            demo/
                building7m/
                queueing7/
                workers/
            getting_started/
                waveform/
        simulations/
            building7m_simulation/
            first_simulation/
            queueing_simulation/
            setting_up/
            simulation_with_ports/
            workers_simulation/
```

Open **CMakeLists.txt** and add the following instructions to the `SyDEVS Nodes` section.

```cmake
set(BUILDING7M_DIR ${NODES_DIR}/demo/building7m)
file(GLOB BUILDING7M_HDRS "${BUILDING7M_DIR}/*.h")

set(QUEUEING_DIR ${NODES_DIR}/demo/queueing)
file(GLOB QUEUEING_HDRS "${QUEUEING_DIR}/*.h")

set(WORKERS_DIR ${NODES_DIR}/demo/workers)
file(GLOB WORKERS_HDRS "${WORKERS_DIR}/*.h")
aux_source_directory(${WORKERS_DIR} WORKERS_SRCS)
```

Also add the following to the `Simulations` section at the bottom, and save.

```cmake
set(BUILDING7M_SIMULATION_DIR ${SIMULATIONS_DIR}/building7m_simulation)
aux_source_directory(${BUILDING7M_SIMULATION_DIR} BUILDING7M_SIMULATION_SRCS)
add_executable(building7m_simulation ${BUILDING7M_SIMULATION_SRCS} ${BUILDING7M_HDRS})
target_link_libraries(building7m_simulation SyDEVS-static)
add_custom_command(TARGET building7m_simulation POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
                   ${PROJECT_SOURCE_DIR}/${BUILDING7M_DIR}/building7m.png
                   $<TARGET_FILE_DIR:building7m_simulation>/.)

set(QUEUEING_SIMULATION_DIR ${SIMULATIONS_DIR}/queueing_simulation)
aux_source_directory(${QUEUEING_SIMULATION_DIR} QUEUEING_SIMULATION_SRCS)
add_executable(queueing_simulation ${QUEUEING_SIMULATION_SRCS} ${QUEUEING_HDRS})
target_link_libraries(queueing_simulation SyDEVS-static)

set(WORKERS_SIMULATION_DIR ${SIMULATIONS_DIR}/workers_simulation)
aux_source_directory(${WORKERS_SIMULATION_DIR} WORKERS_SIMULATION_SRCS)
add_executable(workers_simulation ${WORKERS_SIMULATION_SRCS} ${WORKERS_HDRS} ${WORKERS_SRCS})
target_link_libraries(workers_simulation SyDEVS-static)
```

When the project is re-built, there should be an executable named `building7m_simulation` which produces the following results.

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

There should also be an executable named `queueing_simulation` which produces the output below.

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

Finally, there should be a `workers_simulation` executable that produces the following output.

```
60|0|$time:time_point() + 300_s
60|2|top.workplace#arrival_input:0
120|0|$time:time_point() + 600_s
120|2|top.workplace#arrival_input:1
180|0|$time:time_point() + 900_s
180|2|top.workplace#arrival_input:2
240|0|$time:time_point() + 1_ks + 200_s
240|2|top.workplace#arrival_input:3
300|0|$time:time_point() + 1_ks + 500_s
300|2|top.workplace#arrival_input:4
360|0|$time:time_point() + 1_ks + 800_s
360|2|top.workplace#arrival_input:5
420|0|$time:time_point() + 2_ks + 100_s
420|2|top.workplace#arrival_input:6
480|0|$time:time_point() + 2_ks + 400_s
480|2|top.workplace#arrival_input:7
540|0|$time:time_point() + 2_ks + 700_s
540|2|top.workplace#arrival_input:8
600|0|$time:time_point() + 3_ks
600|2|top.workplace#arrival_input:9
660|0|$time:time_point() + 3_ks + 300_s
660|2|top.workplace#arrival_input:10
720|0|$time:time_point() + 3_ks + 600_s
720|2|top.workplace#arrival_input:11
780|0|$time:time_point() + 3_ks + 900_s
780|3|top.workplace#change_output:{0, "break"}
840|0|$time:time_point() + 4_ks + 200_s
840|4|top.workplace#change_output:{1, "break"}
900|0|$time:time_point() + 4_ks + 500_s
900|5|top.workplace#change_output:{2, "break"}
960|0|$time:time_point() + 4_ks + 800_s
960|6|top.workplace#change_output:{3, "break"}
1020|0|$time:time_point() + 5_ks + 99_s + 999_ms + 998_us
1020|6|top.workplace#change_output:{0, "work"}
1021|0|$time:time_point() + 5_ks + 100_s
1021|6|top.workplace#change_output:{4, "break"}
1025|0|$time:time_point() + 5_ks + 118_s + 606_ms + 33_us
1025|6|top.workplace#change_output:{1, "work"}
1028|0|$time:time_point() + 5_ks + 129_s + 12_ms + 734_us
1028|5|top.workplace#change_output:{2, "work"}
1031|0|$time:time_point() + 5_ks + 135_s + 733_ms + 285_us
1031|4|top.workplace#change_output:{3, "work"}
1084|0|$time:time_point() + 5_ks + 400_s
1084|4|top.workplace#change_output:{5, "break"}
1107|0|$time:time_point() + 5_ks + 510_s + 796_ms + 62_us
1107|4|top.workplace#change_output:{4, "work"}
1145|0|$time:time_point() + 5_ks + 700_s
1145|4|top.workplace#change_output:{6, "break"}
1179|0|$time:time_point() + 5_ks + 867_s + 373_ms + 545_us
1179|4|top.workplace#change_output:{5, "work"}
1206|0|$time:time_point() + 6_ks
1206|4|top.workplace#change_output:{7, "break"}
1232|0|$time:time_point() + 6_ks + 127_s + 923_ms + 270_us
1232|4|top.workplace#change_output:{6, "work"}
1267|0|$time:time_point() + 6_ks + 300_s
1267|4|top.workplace#change_output:{8, "break"}
1298|0|$time:time_point() + 6_ks + 452_s + 113_ms + 21_us
1298|4|top.workplace#change_output:{7, "work"}
1328|0|$time:time_point() + 6_ks + 600_s
1328|4|top.workplace#change_output:{9, "break"}
1356|0|$time:time_point() + 6_ks + 735_s + 763_ms + 276_us
1356|4|top.workplace#change_output:{8, "work"}
1389|0|$time:time_point() + 6_ks + 900_s
1389|4|top.workplace#change_output:{10, "break"}
1419|0|$time:time_point() + 7_ks + 46_s + 210_ms + 578_us
1419|4|top.workplace#change_output:{9, "work"}
1450|0|$time:time_point() + 7_ks + 200_s
1450|4|top.workplace#change_output:{11, "break"}
1478|0|$time:time_point() + 7_ks + 339_s + 266_ms + 172_us
1478|4|top.workplace#change_output:{10, "work"}
1522|0|$time:time_point() + 7_ks + 554_s + 916_ms + 930_us
1522|3|top.workplace#change_output:{11, "work"}
1752|0|$time:time_point() + 8_ks + 699_s + 999_ms + 998_us
1752|3|top.workplace#change_output:{0, "break"}
1757|0|$time:time_point() + 8_ks + 718_s + 606_ms + 33_us
1757|4|top.workplace#change_output:{1, "break"}
1760|0|$time:time_point() + 8_ks + 729_s + 12_ms + 734_us
1760|5|top.workplace#change_output:{2, "break"}
1763|0|$time:time_point() + 8_ks + 735_s + 733_ms + 285_us
1763|6|top.workplace#change_output:{3, "break"}
1839|0|$time:time_point() + 9_ks + 110_s + 796_ms + 62_us
1839|7|top.workplace#change_output:{4, "break"}
1911|0|$time:time_point() + 9_ks + 467_s + 373_ms + 545_us
1911|8|top.workplace#change_output:{5, "break"}
1964|0|$time:time_point() + 9_ks + 727_s + 923_ms + 270_us
1964|9|top.workplace#change_output:{6, "break"}
1996|0|$time:time_point() + 9_ks + 882_s + 178_ms + 256_us
1996|9|top.workplace#change_output:{0, "work"}
1997|0|$time:time_point() + 9_ks + 882_s + 182_ms + 278_us
1997|8|top.workplace#change_output:{1, "work"}
1998|0|$time:time_point() + 9_ks + 882_s + 182_ms + 907_us
1998|7|top.workplace#change_output:{2, "work"}
1999|0|$time:time_point() + 9_ks + 882_s + 183_ms + 51_us
1999|6|top.workplace#change_output:{3, "work"}
2017|0|$time:time_point() + 9_ks + 968_s + 410_ms + 36_us
2017|5|top.workplace#change_output:{4, "work"}
2022|0|$time:time_point() + 9_ks + 986_s + 662_ms + 172_us
2022|4|top.workplace#change_output:{5, "work"}
2024|0|$time:time_point() + 9_ks + 990_s + 448_ms + 698_us
2024|3|top.workplace#change_output:{6, "work"}
2037|0|$time:time_point() + 10_ks + 52_s + 113_ms + 21_us
2037|3|top.workplace#change_output:{7, "break"}
2095|0|$time:time_point() + 10_ks + 335_s + 763_ms + 276_us
2095|4|top.workplace#change_output:{8, "break"}
2158|0|$time:time_point() + 10_ks + 646_s + 210_ms + 578_us
2158|5|top.workplace#change_output:{9, "break"}
2217|0|$time:time_point() + 10_ks + 939_s + 266_ms + 172_us
2217|6|top.workplace#change_output:{10, "break"}
2261|0|$time:time_point() + 11_ks + 154_s + 916_ms + 930_us
2261|7|top.workplace#change_output:{11, "break"}
2308|0|$time:time_point() + 11_ks + 380_s + 257_ms + 58_us
2308|7|top.workplace#change_output:{7, "work"}
2314|0|$time:time_point() + 11_ks + 407_s + 130_ms + 350_us
2314|6|top.workplace#change_output:{8, "work"}
2318|0|$time:time_point() + 11_ks + 422_s + 937_ms + 355_us
2318|5|top.workplace#change_output:{9, "work"}
2321|0|$time:time_point() + 11_ks + 430_s + 363_ms + 786_us
2321|4|top.workplace#change_output:{10, "work"}
2322|0|$time:time_point() + 11_ks + 432_s + 212_ms + 525_us
2322|3|top.workplace#change_output:{11, "work"}
2733|0|$time:time_point() + 13_ks + 482_s + 178_ms + 256_us
2733|3|top.workplace#change_output:{0, "break"}
2734|0|$time:time_point() + 13_ks + 482_s + 182_ms + 278_us
2734|4|top.workplace#change_output:{1, "break"}
2735|0|$time:time_point() + 13_ks + 482_s + 182_ms + 907_us
2735|5|top.workplace#change_output:{2, "break"}
2736|0|$time:time_point() + 13_ks + 482_s + 183_ms + 51_us
2736|6|top.workplace#change_output:{3, "break"}
2754|0|$time:time_point() + 13_ks + 568_s + 410_ms + 36_us
2754|7|top.workplace#change_output:{4, "break"}
2759|0|$time:time_point() + 13_ks + 586_s + 662_ms + 172_us
2759|8|top.workplace#change_output:{5, "break"}
2761|0|$time:time_point() + 13_ks + 590_s + 448_ms + 698_us
2761|9|top.workplace#change_output:{6, "break"}
2880|0|$time:time_point() + 14_ks + 183_s + 927_ms + 782_us
2880|9|top.workplace#change_output:{0, "work"}
2880|20|top.workplace#change_output:{1, "work"}
2880|30|top.workplace#change_output:{2, "work"}
2880|39|top.workplace#change_output:{3, "work"}
2881|0|$time:time_point() + 14_ks + 183_s + 960_ms + 155_us
2881|5|top.workplace#change_output:{4, "work"}
2882|0|$time:time_point() + 14_ks + 183_s + 960_ms + 277_us
2882|4|top.workplace#change_output:{5, "work"}
2882|10|top.workplace#change_output:{6, "work"}
3043|0|$time:time_point() + 14_ks + 980_s + 257_ms + 58_us
3043|3|top.workplace#change_output:{7, "break"}
3049|0|$time:time_point() + 15_ks + 7_s + 130_ms + 350_us
3049|4|top.workplace#change_output:{8, "break"}
3053|0|$time:time_point() + 15_ks + 22_s + 937_ms + 355_us
3053|5|top.workplace#change_output:{9, "break"}
3056|0|$time:time_point() + 15_ks + 30_s + 363_ms + 786_us
3056|6|top.workplace#change_output:{10, "break"}
3057|0|$time:time_point() + 15_ks + 32_s + 212_ms + 525_us
3057|7|top.workplace#change_output:{11, "break"}
3178|0|$time:time_point() + 15_ks + 631_s + 353_ms + 70_us
3178|7|top.workplace#change_output:{7, "work"}
3179|0|$time:time_point() + 15_ks + 631_s + 353_ms + 372_us
3179|6|top.workplace#change_output:{8, "work"}
3180|0|$time:time_point() + 15_ks + 631_s + 353_ms + 429_us
3180|5|top.workplace#change_output:{9, "work"}
3181|0|$time:time_point() + 15_ks + 631_s + 353_ms + 433_us
3181|4|top.workplace#change_output:{10, "work"}
3181|10|top.workplace#change_output:{11, "work"}
3612|0|$time:time_point() + 17_ks + 783_s + 927_ms + 782_us
3612|3|top.workplace#change_output:{0, "break"}
3612|10|top.workplace#change_output:{1, "break"}
3612|18|top.workplace#change_output:{2, "break"}
3612|27|top.workplace#change_output:{3, "break"}
3613|0|$time:time_point() + 17_ks + 783_s + 960_ms + 155_us
3613|7|top.workplace#change_output:{4, "break"}
3614|0|$time:time_point() + 17_ks + 783_s + 960_ms + 277_us
3614|8|top.workplace#change_output:{5, "break"}
3614|20|top.workplace#change_output:{6, "break"}
3735|0|$time:time_point() + 18_ks + 383_s + 960_ms + 275_us
3735|9|top.workplace#change_output:{0, "work"}
3735|20|top.workplace#change_output:{1, "work"}
3735|30|top.workplace#change_output:{2, "work"}
3735|39|top.workplace#change_output:{3, "work"}
3735|47|top.workplace#change_output:{4, "work"}
3735|54|top.workplace#change_output:{5, "work"}
3735|60|top.workplace#change_output:{6, "work"}
3906|0|$time:time_point() + 19_ks + 231_s + 353_ms + 70_us
3906|3|top.workplace#change_output:{7, "break"}
3907|0|$time:time_point() + 19_ks + 231_s + 353_ms + 372_us
3907|4|top.workplace#change_output:{8, "break"}
3908|0|$time:time_point() + 19_ks + 231_s + 353_ms + 429_us
3908|5|top.workplace#change_output:{9, "break"}
3909|0|$time:time_point() + 19_ks + 231_s + 353_ms + 433_us
3909|6|top.workplace#change_output:{10, "break"}
3909|16|top.workplace#change_output:{11, "break"}
4030|0|$time:time_point() + 19_ks + 831_s + 353_ms + 431_us
4030|7|top.workplace#change_output:{7, "work"}
4030|16|top.workplace#change_output:{8, "work"}
4030|24|top.workplace#change_output:{9, "work"}
4030|31|top.workplace#change_output:{10, "work"}
4030|37|top.workplace#change_output:{11, "work"}
4461|0|$time:time_point() + 21_ks + 983_s + 960_ms + 275_us
4461|3|top.workplace#change_output:{0, "break"}
4461|10|top.workplace#change_output:{1, "break"}
4461|18|top.workplace#change_output:{2, "break"}
4461|27|top.workplace#change_output:{3, "break"}
4461|37|top.workplace#change_output:{4, "break"}
4461|48|top.workplace#change_output:{5, "break"}
4461|60|top.workplace#change_output:{6, "break"}
4582|0|$time:time_point() + 22_ks + 583_s + 960_ms + 275_us
4582|9|top.workplace#change_output:{0, "work"}
4582|20|top.workplace#change_output:{1, "work"}
4582|30|top.workplace#change_output:{2, "work"}
4582|39|top.workplace#change_output:{3, "work"}
4582|47|top.workplace#change_output:{4, "work"}
4582|54|top.workplace#change_output:{5, "work"}
4582|60|top.workplace#change_output:{6, "work"}
4753|0|$time:time_point() + 23_ks + 431_s + 353_ms + 431_us
4753|3|top.workplace#change_output:{7, "break"}
4753|10|top.workplace#change_output:{8, "break"}
4753|18|top.workplace#change_output:{9, "break"}
4753|27|top.workplace#change_output:{10, "break"}
4753|37|top.workplace#change_output:{11, "break"}
4874|0|$time:time_point() + 24_ks + 31_s + 353_ms + 431_us
4874|7|top.workplace#change_output:{7, "work"}
4874|16|top.workplace#change_output:{8, "work"}
4874|24|top.workplace#change_output:{9, "work"}
4874|31|top.workplace#change_output:{10, "work"}
4874|37|top.workplace#change_output:{11, "work"}
```

The building7m (7-meter building) example demonstrates a number of features of the SyDEVS library.

- Composite Node with Ports and Inward/Outward Links: [building_dynamics_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/building7m/building_dynamics_node.h)
- Composite Node containing a Composite Node (hierarchy): [building_closed_system.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/building7m/building_closed_system.h)
- Multidimensional Arrays: [thermodynamics_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/building7m/thermodynamics_node.h)
- Standard International (SI) Units: [occupant_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/building7m/occupant_node.h)

The queueing example is notable in that it features a commented header file demonstrating each of the four types of nodes.

- Atomic Node: [queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/queueing_node.h)
- Composite Node: [two_stage_queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/two_stage_queueing_node.h)
- Collection Node: [parallel_queueing_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/parallel_queueing_node.h)
- Function Node: [plus_node.h](https://github.com/Autodesk/sydevs/blob/master/src/examples/demo/queueing/plus_node.h)

The workers example is described in a separate tutorial, Agent-Based Modeling with Symmetric DEVS ([Download PPTX](https://autodesk.github.io/sydevs/doc/downloads/AgentBasedSyDEVS_Tutorial.pptx)).

Take a few minutes to familiarize yourself with these examples, then proceed to the final part of the tutorial. In [Part 7](part07.html), you will be challenged to make several enhancements to the `building7m` example.

| [***Continue to Part 7***](part07.html) |


