# Molecule simulation

This simulates the intermolecular interactions using the [Lennard Jones potential](https://wikipedia.org/wiki/Lennard-Jones_potential).

## Build

Simple! Just run `make` on UNIX like systems.

## Config file

For now, there're only 6 fields:

* WW -> Window Width (int)
* WH -> Window Height (int)
* GRAVITY -> Gravitational acceleration (float)
* NUMBER_OF_BALLS -> Number of balls (int)
* REPULTION -> Repultion force (float)
* MAX_START_SPEED -> Maximum initial Speed (on each axis) (float)

Now variable names can have many characters.

The config format is like CSS code style:
```config
WW: {random number}
```
