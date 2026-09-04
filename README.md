# Molecule simulation

This simulates the intermolecular interactions using the [Lennard Jones potential](https://wikipedia.org/wiki/Lennard-Jones_potential).

## Build

Simple! Just run `make` on UNIX like systems.

## Config file

For now, there're only 6 fields:

* WW -> Window Width
* WH -> Window Height
* GR -> Gravity acceleration
* NB -> Number of balls
* RE -> Repultion force
* MS -> Maximum initial Speed (on each axis)

For now, only variables with one line are possible.

The config format is like CSS code style:
```config
WW: {random number}
#  ^
# Use this space!!! (Also, comments can lead to erros)
```
