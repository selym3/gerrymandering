from frame import Frame, Vertex
from district import District, Node
from fake import get_district, split
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
            red_votes = 0

            # adds data for one party
            for node in district.nodes:
                if in_border(node.position, an.position, bn.position):
                    red_votes += node.get_people("red")
                else:
                    red_votes -= node.get_people("red")

            if abs(red_votes) < smallest:
                best_border = [an, bn]
                smallest = red_votes
    
    return best_border

def draw_district(district, frame: Frame):
    for node in district.nodes:
        frame.set_color(node.position.x, node.position.y, (255, 0, 0))
    
    for border_node in district.border_nodes:
        frame.set_color(border_node.position.x, border_node.position.y, (255, 255, 255))

    border = splitline_district(district)
    frame.line(
        Vertex(border[0].position, (0, 0, 255)), 
        Vertex(border[1].position, (0, 255, 0))
    )

def draw_state(state, frame: Frame):
    for district in state.districts:
        draw_district(district, frame)

def test_thing():
    frame = Frame(300, 300)
    district = District()

    while True:
        x = input()
        y = input()

        district.add_border(Node(x, y))
        draw_district(district, frame)
        frame.image().show()



if __name__ == "__main__":
    
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
    draw_district(district, frame)
    # im = frame.image()
    # im.save("images/test/mk1.png")
    frame.image().show()
