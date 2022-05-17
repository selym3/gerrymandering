from frame import Frame, Vertex
from district import District, Node
from fake import get_district, split
from draw import *

class State:
    def __init__(self):
        self.districts = [get_district(50, 50)]

    # def from_data(self, file):
    #     with open(file, 'r') as data:
    #         pass

def splitline(state):
    pass

def splitline_district(district):
    smallest = float('inf')
    best_border = [None, None]

    def in_border(p, b1, b2):
        return (b1-b2).is_clockwise(p-b2)

    for an in district.border_nodes:
        for bn in district.border_nodes:
            pop_inside = 0
            pop_outside = 0

            for node in district.nodes:
                if in_border(node.position, an.position, bn.position):
                    pop_inside += node.get_total()
                else:
                    pop_outside += node.get_total()

            if abs(pop_outside - pop_inside) < smallest:
                best_border = [an, bn]
                smallest = abs(pop_outside - pop_inside)
    
    return best_border

def test_input():
    frame = Frame(300, 300)
    district = District()

    while True:
        print("enter x coordinate (or exit)")
        x = input()

        if x == 'exit':
            break

        print("enter y coordinate (or exit)")
        y = input()

        x = int(x)
        y = int(y)

        district.add_border(Node(x, y))
        draw_district(district, frame)
        frame.image().show()



if __name__ == "__main__":
    # test_input()
    
    # generate district
    district = get_district(6, 6, gen=split(6, 6))
    if False: 
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

        for loc in node_locs:
            district.add_node(Node(loc))

    # print(district.nodes)
    # print(district.nodes)

    # create frame & draw district
    frame = Frame(300, 300)
    frame.scale(50)
    draw_district(district, frame)
    # im = frame.image()
    # im.save("images/test/mk1.png")
    frame.image().show()