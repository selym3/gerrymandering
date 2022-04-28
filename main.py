from vec2 import Vec2
from frame import Frame

class Node:
    """ contains party / population data """

    def __init__(self, position: Vec2):
        self.position = position

        self.population = {}

        # self.parties = []
        # self.number = []
        # self.colors = []

    def add_people(self, count: int, party: str = "n/a"):
        if party not in self.population:
            self.population[party] = 0

        self.population[party] += count
        return self

class District:
    def __init__(self):
        self.nodes = []
        self.border_nodes = []

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


class State:
    def __init__(self):


        pass

    def from_data(self, file):
        with open(file, 'r') as data:
            pass

def draw(district, frame: Frame):
    for node in district.nodes:
        frame.set_color(node.position.x, node.position.y, (255, 0, 0))
    
    for border_node in district.border_nodes:
        frame.set_color(border_node.position.x, border_node.position.y, (255, 255, 255))

if __name__ == "__main__":
    
    # generate district
    district = District()
    node_locs = []
    import math
    def gen_points(radius):
        angle = -math.pi

        while angle < +math.pi:
            loc = Vec2(
                150 + int(radius * math.cos(angle)), 
                150 + int(radius * math.sin(angle))
            )
            angle += (2 * math.pi) / 50
            yield loc

    for point in gen_points(50):
        node_locs.append(point)

    for point in gen_points(30):
        node_locs.append(point)


        

    if False:
        node_locs = [
            Vec2(150, 150),
            Vec2(151, 151),
            Vec2(153, 153),
            Vec2(154, 154),
            Vec2(155, 155),
            Vec2(156, 156),
            Vec2(157, 157),
            Vec2(158, 158),
            Vec2(159, 159),
            Vec2(160, 160),
            Vec2(161, 157),
            Vec2(162, 156),
            Vec2(163, 155),
            Vec2(164, 154),
            Vec2(165, 153),
            Vec2(166, 152),
            Vec2(167, 151)
        ]
    for loc in node_locs:
        district.add_node(Node(loc))

    # create frame & draw district
    frame = Frame(300, 300)
    draw(district, frame)
    im = frame.image()
    im.save("images/test/mk1.png")
