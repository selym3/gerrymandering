from vec2 import Vec2

class Node:
    """ contains party / population data """

    def __init__(self, x: int, y: int):
        self.position = Vec2(x, y)
        self.population = {}

    def add_people(self, count: int, party: str = "n/a"):
        if party not in self.population:
            self.population[party] = 0

        self.population[party] += count
        return self

    def get_people(self, party: str):
        if party not in self.population:
            return 0
        return self.population[party]

    def __str__(self):
        return f"{self.population} at {self.position}"

    def __repr__(self):
        return f"Node({self})"

class District:
    def __init__(self):
        self.nodes = []
        self.border_nodes = []

        self.totals = Node(0, 0)

    def add_non_border(self, node: Node):
        self.nodes.append(node)
    def add_border(self, node: Node):
        self.border_nodes.append(node)

        

    # FIRST POINT: left-most point
    def add_node(self, node: Node):
        if len(self.border_nodes) < 2:
            self.border_nodes.append(node)
            
        elif len(self.border_nodes) == 2:
            curr_node = self.border_nodes[1].position - self.border_nodes[0].position
            insert_node = node.position - self.border_nodes[0].position

            if curr_node.is_clockwise(insert_node):
                self.border_nodes.append(node)
            else:
                self.border_nodes.insert(1)

        else:
            #TODO: Logic for if the point is to the left of the leftmost point
            # Loop through each pair of borders
            # If the first point to the new point is counterclockwise of the first to second point add it inbetween
            # Check the next pair, and if the pair is counterclockwise from the new point to next, remove the next point and repeat
            is_border = False

            for i in range(len(self.border_nodes)):
                curr_node = self.border_nodes[i]
                next_node = self.border_nodes[(i + 1) % len(self.border_nodes)]
                
                if not (next_node.position - curr_node.position).is_clockwise(node.position - curr_node.position):
                    is_border = True
                    i += 1

                    self.border_nodes.insert(i, node)

                    curr_node = self.border_nodes[(i + 1) % len(self.border_nodes)].position - self.border_nodes[i].position
                    next_node = self.border_nodes[(i + 2) % len(self.border_nodes)].position - self.border_nodes[(i + 2) % len(self.border_nodes)].position

                    while not curr_node.is_clockwise(next_node):
                        self.nodes.append(self.border_nodes.pop((i + 1) % len(self.border_nodes)))
                        curr_node = self.border_nodes[(i + 1) % len(self.border_nodes)].position - self.border_nodes[i].position
                        next_node = self.border_nodes[(i + 2) % len(self.border_nodes)].position - self.border_nodes[(i + 2) % len(self.border_nodes)].position

                    break

            if not is_border:
                self.nodes.append(node)
