# Molecule simulation

This simulates the intermolecular interactions using the [Lennard Jones potential](https://wikipedia.org/wiki/Lennard-Jones_potential) and the [Stefan–Boltzmann Law](https://wikipedia.org/wiki/Stefan%E2%80%93Boltzmann_law).

## Build

To compile for Linux, use `make lbuild`.

To compile for Windows, use `make wbuild`.

To compile for both at once, run `make all`.

## Key binds

For now, the only key binds are O and P, which respectively increase and decrease the temperature.

## Config file

For now, there are only 6 fields:

* WW -> Window Width. (int)
* WH -> Window Height. (int)
* GRAVITY -> Gravitational acceleration. (float)
* NUMBER_OF_BALLS -> Number of balls. (int)
* REPULSION -> Repulsion force. (float)
* MAX_START_SPEED -> Maximum initial Speed (on each axis). (float)
* CHANGE -> Defines how much the speed changes when using the O and P keys, the formula is `Velocity * (1 + CHANGE)` for P and `Velocity * (1 - CHANGE)` for O. (float)
* FPS -> Defines the max frame rate of the program. (int)
* THREADS -> Defines how many threads the program will use, if you don't define it, the program will use the number of threads in your system. (int)
* MAX_COLOR -> Defines the speed that sets the color as red. (float)
* COLOR_DEFINITION -> Sets how the color of each particle will be defined, 1 if it's based on the particle's mass, 0 if it's based on its speed. (int)
* MAX_MASS -> Maximum mass of each particle. (float)
* BOLTZMANN_C -> The Boltzmann constant, must be very small or zero. (float)
* RADIUS -> The ball's radius. (int)

Now variable names can have many characters.

The config format is like CSS code style:
```config
WW: {random number}
# This is a comment
```
