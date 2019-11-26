| [Prev](part06.html) | [Getting Started -- Table of Contents](index.html) | Next |
# Part 7: Becoming Proficient

You've now compiled and executed a number of SyDEVS simulations, and learned the basics of the framework. To become proficient at using the library for your own project, you'll need to challenge yourself and develop SyDEVS nodes without the benefit of step-by-step guidance. In this last part of the Getting Started tutorial, we'll begin that process.

Download and open the SyDEVS Building7m Tutorial ([PPTX](https://autodesk.github.io/sydevs/doc/downloads/SyDEVS_Building7m_Tutorial.pptx), [PDF](https://autodesk.github.io/sydevs/doc/downloads/SyDEVS_Building7m_Tutorial.pdf)).

This resource first describes the `building7m` example that you incoporated into your project in [Part 6](part06.html):

1. The `weather_node` produces a sequence of outdoor temperatures that rise and fall in a sinewave pattern.
2. The `thermodynamics_node` computes the temperature at each 20cm x 20cm grid cell over time, accounting for the thermal resistance of building walls.
3. The `occupant_node` tracks the movements of a single building occupant who walks randomly from the current grid cell to one of 4 adjacent grid cells.
4. The `building_visualization_node` periodically prints the current state of the system as a grid of temperature values with the occupant location indicated.

It then lists five ways to enhance the example:

1. The occupant moves diagonally.
2. A new `comfort_node` tracks the temperature at the occupant’s location.
3. A new `average_temperature` statistic node reports the time-averaged temperature experienced by the occupant.
4. The building layout is read from a file.
5. The building visualization uses 2D graphics.

Each enhancement has its own one-page explanation.

The final page gives a set of instructions:

1. Run `building7m`.
2. Open `building7m.h`.
  - Re-run after inserting the following line: `sim.top.building_dynamics.thermodynamics.outdoor_temperature_input.print_on_use();`
  - Re-run after inserting the following line: `sim.top.building_dynamics.occupant.print_on_event();`
  - Re-run after inserting similar event notification and port value printing lines.
3. Undo changes above, and open `weather_node.h.`
  - Find a location in the Planned Event Handler beneath the reassignment of the `rate` state variable.
  - Re-run after inserting the following line: `print(tostring(rate));`
4. Undo changes above, and open `building_closed_system.h`.
  - Re-run after changing the frame duration parameter value from `duration::inf()` to `30_s`. 
5. Implement at least the first 3 enhancements, and ideally all 5.

Follow these intructions and complete at least the first three enhancements. Enhancements 1-3 will improve your understanding of SyDEVS as well as your confidence in using the framework. The last two enhancements should also be instructive, but rather than focussing on SyDEVS they require you to source other C++ libraries.

That's it! Once you've completed the SyDEVS Building7m Tutorial, continue exploring the resources listed on the [SyDEVS webpage](/sydevs/) and [GitHub repo](https://github.com/Autodesk/sydevs), and ***start using SyDEVS!***
