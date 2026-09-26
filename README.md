# Molecule simulation

This simulates the intermolecular interactions using the [Lennard Jones potential](https://wikipedia.org/wiki/Lennard-Jones_potential).

## Build

To compile for Linux, use `make lbuild`.

To compile for Windows, use `make wbuild`.

To compile for both at once, run `make all`.

## Keybinds

For now, the only keybinds are O and P, which respectively increase and decrease the temperature.

## Config file

For now, there're only 6 fields:

* WW -> Window Width. (int)
* WH -> Window Height. (int)
* GRAVITY -> Gravitational acceleration. (float)
* NUMBER_OF_BALLS -> Number of balls. (int)
* REPULTION -> Repultion force. (float)
* MAX_START_SPEED -> Maximum initial Speed (on each axis). (float)
* CHANGE -> Defines how much the speed changes when using the O and P keys, the fomula is `Velocity * (1 + CHANGE)` for P and `Velocity * (1 - CHANGE)` for O. (float)
* FPS -> Defines the max frame rate of the program. (int)
* THREADS -> Defines how many threads the program will use, if you don't define it, the program will use the number of threads in your system. (int)
* MAX_COLOR -> Defines the speed that sets the color as red. (float)
* COLOR_DEFINITION -> Defines if the color is based on the speed or the mass, 1 for the mass, 0 for the speed. (int)
* MAX_MASS -> Maximum mass of each particle. (float)

Now variable names can have many characters.

The config format is like CSS code style:
```config
WW: {random number}
# This is a comment
```
