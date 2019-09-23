| [Prev](part02.html) | [Getting Started -- Table of Contents](index.html) | [Next](part04.html) |
# Part 3: Customizing Simulation Output

In [Part 2](part02.html) you created a node called `square_wave_closed_system` that used the standard output stream `std::cout` to produce the following output.

```
y = 0
y = 1
y = 0
y = 1
y = 0
y = 1
y = 0
y = 1
y = 0
y = 1
y = 0
y = 1
```

However the use of `std::cout` within a node is not recommended practice. One problem with `std::cout` is that it does not indicate the point in simulated time at which each output occurred. Let's find a better way to extract information from the simulation.

Delete the following line of code, which imports the standard output stream, from the top of the file `square_wave_closed_system.h`.

```cpp
#include <iostream>
```

Now, find the line below...

```cpp
    std::cout << "y = " << float64(phase) << std::endl;
```

...and replace it with the following and save.

```cpp
    print((string_builder() << "y = " << float64(phase)).str());
```

Here the `string_builder` object is used to create a `std::string` value of the form `"y = #"`. The `print` function outputs the string along with contextual information about the simulation. If you rebuild and run the `first_simulation` executable, you should see the following output.

```
0|0|$time:time_point()
0|0|top$print:y = 0
1|0|$time:time_point() + 7_s
1|0|top$print:y = 1
2|0|$time:time_point() + 10_s
2|0|top$print:y = 0
3|0|$time:time_point() + 17_s
3|0|top$print:y = 1
4|0|$time:time_point() + 20_s
4|0|top$print:y = 0
5|0|$time:time_point() + 27_s
5|0|top$print:y = 1
6|0|$time:time_point() + 30_s
6|0|top$print:y = 0
7|0|$time:time_point() + 37_s
7|0|top$print:y = 1
8|0|$time:time_point() + 40_s
8|0|top$print:y = 0
9|0|$time:time_point() + 47_s
9|0|top$print:y = 1
10|0|$time:time_point() + 50_s
10|0|top$print:y = 0
11|0|$time:time_point() + 57_s
11|0|top$print:y = 1
```

Notice that each row begins with two numbers. The first number associates each row with a particular point in simulated time. The rows that begin with 0, for example, are associated with the time at the beginning of the simulation. The rows that begin with 3 are associated with the time point at 17 seconds from the beginning. The second number is a counter of steps within each time point, starting from 0. In this example, there is only one step per time point, so the counter is always at 0.

We still see the printed text `"y = 0"` or `"y = 1"`, but it is associated with a node named `top`. This is the name that is always given to the top-level node in a simulation. In this example, the top node is the only node, but in more complex examples it is useful to see which node produced each output.

Although using the `print` function is recommended over using `std::cout`, it is still not the best practice. Ideally, the signal level would be made available through a message output port, and operations outside of the node would determine how the values are presented. This will be covered in the next part of the tutorial.

Let's explore one more output customization feature.

In the `square_wave.h` file, find the instruction...

```cpp
sim.process_remaining_events();
```

...and immediately above it insert the following line and save.

```cpp
sim.top.print_on_event();
```

Now when the `first_simulation` executable is rebuilt and run, the type of each event is printed. You can see that the `initialization` event occurs first, followed by a succession of `planned` events, followed by the `finalization` event. This extra information helps one locate the function responsible for each printed value.

```
0|0|$time:time_point()
0|0|top$initialization
0|1|top$planned
0|1|top$print:y = 0
1|0|$time:time_point() + 7_s
1|0|top$planned
1|0|top$print:y = 1
2|0|$time:time_point() + 10_s
2|0|top$planned
2|0|top$print:y = 0
3|0|$time:time_point() + 17_s
3|0|top$planned
3|0|top$print:y = 1
4|0|$time:time_point() + 20_s
4|0|top$planned
4|0|top$print:y = 0
5|0|$time:time_point() + 27_s
5|0|top$planned
5|0|top$print:y = 1
6|0|$time:time_point() + 30_s
6|0|top$planned
6|0|top$print:y = 0
7|0|$time:time_point() + 37_s
7|0|top$planned
7|0|top$print:y = 1
8|0|$time:time_point() + 40_s
8|0|top$planned
8|0|top$print:y = 0
9|0|$time:time_point() + 47_s
9|0|top$planned
9|0|top$print:y = 1
10|0|$time:time_point() + 50_s
10|0|top$planned
10|0|top$print:y = 0
11|0|$time:time_point() + 57_s
11|0|top$planned
11|0|top$print:y = 1
12|0|$time:time_point() + 60_s
12|0|top$finalization
```

The `print` and `print_on_event` features are just two of several mechanisms that SyDEVS provides for customizing simulation output. In [Part 4](part04.html), you'll learn how to take a node and give it a set of ports, which opens up additional possibilities for communicating data.
 
| [***Continue to Part 4***](part04.html) |

