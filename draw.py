from frame import Frame
from vec2 import Vec2
from queue import Queue
from enum import Enum
from district import Node, District
from random import randint


def draw_district(district, frame: Frame):
    for node in district.nodes:
        frame.set_color(node.position.x, node.position.y, (255, 0, 0))

    for border_node in district.border_nodes:
        frame.set_color(border_node.position.x,
                        border_node.position.y, (255, 255, 255))

    border = splitline_district(district)
    frame.line(
        Vertex(border[0].position, (0, 0, 255)),
        Vertex(border[1].position, (0, 255, 0))
    )


def draw_state(state, frame: Frame):
    for district in state.districts:
        draw_district(district, frame)


def draw_interpolated_district(district, frame: Frame):
    class Pixel(Enum):
        BORDER = 0
        INSIDE = 1
        OUTSIDE = 2

    borders = district.border_nodes

    max_corner = Vec2(float('-inf'), float('-inf'))
    min_corner = Vec2(float('inf'), float('inf'))

    for border in borders:
        max_corner.x = max(max_corner.x, border.position.x)
        max_corner.y = max(max_corner.y, border.position.y)

        min_corner.x = min(min_corner.x, border.position.x)
        min_corner.y = min(min_corner.y, border.position.y)

    start_x = int(min_corner.x)
    start_y = int(min_corner.y)

    interpolated = []
    for i in range(int(max_corner.x) - int(min_corner.x) + 1):
        interpolated.append([])
        for j in range(int(max_corner.y) - int(min_corner.y) + 1):
            interpolated[i].append(Pixel.OUTSIDE)


    def valid(interpolated, x, y):
        if x >= 0 and y >= 0 and x < len(interpolated) and y < len(interpolated[0]):
            return interpolated[x][y] == Pixel.OUTSIDE
        else:
            print("invalid " + str(x) + " " + str(y))
        
        return False

    def line_interpolation(n1, n2, interpolated, start_x, start_y):
        x0 = n1.position.x
        x1 = n2.position.x
        y0 = n1.position.y
        y1 = n2.position.y

        if x0 > x1:
            xt = x0
            yt = y0
            x0 = x1
            y0 = y1
            x1 = xt
            y1 = yt

        x = x0
        y = y0
        A = 2 * (y1 - y0)
        B = -2 * (x1 - x0)
        wide = False
        tall = False

        if (abs(x1-x0) >= abs(y1 - y0)):  # octants 1/8
            wide = True
            loop_start = x
            loop_end = x1
            dx_east = dx_northeast = 1
            dy_east = 0
            d_east = A
            distance = x1 - x + 1
            if (A > 0):  # octant 1
                d = A + B/2
                dy_northeast = 1
                d_northeast = A + B
            else:  # octant 8
                d = A - B/2
                dy_northeast = -1
                d_northeast = A - B

        else:  # octants 2/7
            tall = True
            dx_east = 0
            dx_northeast = 1
            distance = abs(y1 - y) + 1
            if (A > 0):  # octant 2
                d = A/2 + B
                dy_east = dy_northeast = 1
                d_northeast = A + B
                d_east = B
                loop_start = y
                loop_end = y1
            else:  # octant 7
                d = A/2 - B
                dy_east = dy_northeast = -1
                d_northeast = A - B
                d_east = -1 * B
                loop_start = y1
                loop_end = y

        while (loop_start < loop_end):
            if valid(interpolated, x - start_x, y - start_y):
                interpolated[x - start_x][y - start_y] = Pixel.BORDER

            if ((wide and ((A > 0 and d > 0) or (A < 0 and d < 0))) or
                    (tall and ((A > 0 and d < 0) or (A < 0 and d > 0)))):

                x += dx_northeast
                y += dy_northeast
                d += d_northeast
            else:
                x += dx_east
                y += dy_east
                d += d_east

            loop_start += 1

            if valid(interpolated, x - start_x, y - start_y):
                interpolated[x - start_x][y - start_y] = Pixel.BORDER

    for i in range(len(borders)):
        line_interpolation(borders[i], borders[(i + 1) % len(borders)], interpolated, start_x, start_y)

    nodes = district.nodes

    q = Queue()

    for node in nodes:
        w = randint(1, 255)
        q.put((Vec2(node.position.x, node.position.y), w))

    while not q.empty():
        pos, w = q.get()
        x = int(pos.x) - start_x
        y = int(pos.y) - start_y

        if valid(interpolated, x, y):
            interpolated[x][y] = w
        else:
            continue

        for i in range(4):
            x_step = [0, -1, 1, 0]
            y_step = [1, 0, 0, -1]
                
            q.put((Vec2(pos.x + x_step[i], pos.y + y_step[i]), w))

    for i in range(len(interpolated)):
        for j in range(len(interpolated[i])):
            if interpolated[i][j] == Pixel.BORDER:
                frame.set_color(i + start_x, j + start_y, (0, 0, 0))
            elif interpolated[i][j] != Pixel.OUTSIDE:
                w = interpolated[i][j]
                frame.set_color(i + start_x, j + start_y, (0, int(w * 1/2), w * 2))
                

d = District()
d.add_border(Node(200, 200))
d.add_border(Node(300, 300))
d.add_border(Node(400, 200))
d.add_border(Node(300, 100))

d.add_node(Node(250, 200))
d.add_node(Node(350, 200))
d.add_node(Node(210, 202))
d.add_node(Node(360, 180))


# d.add_border(Node(5, 5))
# d.add_border(Node(7, 7))
# d.add_border(Node(9, 5))
# d.add_border(Node(7, 3))

f = Frame(500, 500)
for i in range(500):
    for j in range(500):
        f.set_color(i, j, (255, 255, 255))
draw_interpolated_district(d, f)
f.image().show()