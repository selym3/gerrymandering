class Vec2:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def from_points(a, b):
        # Vec2.from_points(...)
        # (a - b).dot()
        pass

    def __eq__(self, rhs):
        return self.x == rhs.x and self.y == rhs.y

    def __sub__(self, rhs):
        return Vec2(self.x - rhs.x, self.y - rhs.y)

    def dot(self, rhs):
        return self.x * rhs.x + self.y * rhs.y

    def is_clockwise(self, rhs):
        return self.dot(rhs) >= 0