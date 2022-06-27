# Gerrymandering

Gerrymandering is the manipulation of district borders with the intent to favor one party. Even though a certain state might have a 50-50 split of people between parties, one party may have an electoral advantage because of unfair districts. One way districts may be drawn with bias is when a city with large minority population is split into several districts in order to diminish their vote.

The goal of this project was to determine fair districts given a state and its voters. We represent a map as a grid of unit cells/nodes. Each of these nodes stores its district and voters.

To find new districsts, we flip a random border node to a surrounding district. Before and after a random border node is flipped, we evaluate a fairness metric on the entire map. If this border flip makes the map more fair according to the metric, it is saved. If it doesn't then there is only a small chance the border will remain flipped (the chance that it can be flipped regardless is important for not converging too quickly onto a map configuration).

The initial districts are determined by a voronoi tessellation. Afterwords, the border flip algorithm is run repeatedly in order to take advantage of the metric on the entire map.

## Requirements

* SFML >=2.0
* C++17 compiler

## Setup (with `make`)

* configure `Makefile` for linking SFML and compiler
* recommended to run `make clean` after fresh install
* `make main` to compile `main` executable
* `make` to compile and run

## Instructions

### Keys

* `W` to toggle displaying borders *(may change soon)*
* `D` to switch display mode
* `Space` to toggle evolving (pausing)
* `R` to reset camera
* `O` to use party population metric *(may change soon)*
* `P` to use centering metric *(may change soon)*

### Mouse

* `Click` to receive information on a node
* `Click + Drag` to pan
* `Mouse Wheel` to zoom
