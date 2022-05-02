from district import District, Node

import random

def rand(min, max):

    def get_node(x, y):
        n = Node(x, y)

        for party in [ "red", "blue" ]:
            n.add_people(random.randint(min, max), party)
        return n

    return get_node

def split(width, height):
    def get_node(x, y):
        n = Node(x, y)

        if x > width / 2:
            n.add_people(1, "red")

        return n

    return get_node


def get_district(width, height, gen):
    d = District()

    for i in range(0, width):
        for j in range(0, height):
            if i == 0 or i == width - 1 or j == 0 or j == height - 1:
                d.add_border(gen(i, j))
            else:
                d.add_non_border(gen(i, j))
    return d