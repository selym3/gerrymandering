# Gerrymandering

Gerrymandering is the manipulation of district borders with the intent to favor one party. Even though a certain state might have a 50-50 split of people between parties, one party may have an electoral advantage because of unfair districts. One way districts may be drawn with bias is when a city with large minority population is split into several districts in order to diminish their vote.

The goal of this project was to determine fair districts given a state and its voters. We represent a map as a grid of unit cells/nodes. Each of these nodes stores its district and voters.

To find new districsts, we flip a random border node to a surrounding district. Before and after a random border node is flipped, we evaluate a fairness metric on the entire map. If this border flip makes the map more fair according to the metric, it is saved. If it doesn't then there is only a small chance the border will remain flipped (the chance that it can be flipped regardless is important for not converging too quickly onto a map configuration).

The initial districts are determined by a voronoi tessellation. Afterwords, the border flip algorithm is run repeatedly in order to take advantage of the metric on the entire map.

## What we did

We knew that we would want a GUI for this project, but we wanted to separate our districting code from our graphics code. To establish this difference, there is an engine class, which simply contains the window, mouse, keyboard, and a list of "behaviors". A behavior is an interface for additional logic in the engine, and it has two abstract methods: `handle_event` and `execute`. This is where panning and zooming is implemented, as well as the code that draws the map.

We also knew that anything we implemented would have to be quick, as our simulation would only update the map 1 node at a time. What became core to the implementation of maps and their metrics was the creation of running algorithms at the cost of memory. For example, in the map itself, we have a running list and set of border nodes, which allows us to efficiently pick a random border node AND detect what nodes are borders.

The same idealogy is adopted for metrics. However, we knew we would want to implement multiple metrics, so we created an abstraction to conveniently create running versions. In our code, any metric *must* define a method of saying whether a swap of a node from its original district to a new district will be better or worse. Metrics can *optionally* define code that will run when a node is added or removed, which allows metrics to have running behavior.

We demonstrated with two metrics: one that simulates the outcome of an election in the current configuration and one that favors moving nodes closer to their district's center of population. The combination of a population-oriented and shape-oriented metric should create fair and believable districts.

To display the map, we simply generate a color for each district. We also draw the map is several other ways, including a population map and a centroid map.

## Challenges

One unfortuante challenge was the difficulty in finding population data. We had to keep our map to a square as a result, and we had to create "cities" randomly in the map to fill out population based on distances to cities.

Another difficulty we had was in keeping track of border nodes. Our running border set is not 100% accurate, which could technically slow down evolution if it selects non-border nodes that it thinks are borders. The idea we had was that only the neighboring nodes need to be checked when a border is flipped. This didn't work, and it may be faulty code, but we moved on because it proved to be a non-issue because there are't many false border nodes and it's quick to recalculate if it's a border. Basically, we called it "good enough".

An obvious difficulty we had was time. Balancing this project with school, end of year testing, and other endeavors the members of our group had, was difficult. Also, the project picked up steam very late, so a lot of progress was made later. This limited us in the amount of GUI elements we could create. More importantly, it limited the amount of time we had to tune the metrics, which could've helped us produce more convincing results.

We chose C++ over Python to prioritize speed over convenience, but linking and compiling was different for everyone of us so it ended up slowing us down. This was probably a mistake, as with proper multithreading Python could've been fast enough. Looking back, choosing C++ can feel like premature optimization.

Settling on this method of redistricting was also a challenge. We started with methods that seemed simpler, like the split-line algorithm, but weren't practical to implement.

## Extensions

Resolving the challenges (real data, border nodes, GUI-elements, tuned metrics). Improving how the code resolves switching metrics (requires passing along all the nodes, which somewhat defeats the purpose of running metrics).

An interesting idea we initially had was to explore how districting algorithms (and this idea came up again for metrics) can be applied to populations with more than 2 significant parties. We never got the chance to explore this.

Also code we started but never finished dealt with saving evolutions and maps so they could be played back. This would be cool, but wasn't a priority.

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
