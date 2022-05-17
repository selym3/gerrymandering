class Vec2:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return f"({self.x}, {self.y})"
        
    def __repr__(self):
        return f"Vec2({self})"

    def __eq__(self, rhs):
        return self.x == rhs.x and self.y == rhs.y

    def __sub__(self, rhs):
        return Vec2(self.x - rhs.x, self.y - rhs.y)

    def __add__(self, rhs):
        return Vec2(self.x + rhs.x, self.y + rhs.y)

    def __mul__(self, rhs):
        return Vec2(self.x * rhs.x, self.y * rhs.y)

    def __itruediv__(self, rhs):
        self.x /= rhs.x
        self.y /= rhs.y
        return self

    def mag(self):
        return (self.x * self.x + self.y * self.y) ** 0.5

    def lerp(self, rhs, t: float):
        return self * Vec2(1 - t, 1 - t) + rhs * Vec2(t, t)

    def dot(self, rhs):
        return self.x * rhs.x + self.y * rhs.y

    def cross(self, rhs):
        return rhs.x * self.y - self.x * rhs.y

    #vec calling is clockwise of rhs
    def is_clockwise(self, rhs):
        return self.cross(rhs) >= 0

    def normal(self):
        magnitude = self.mag()
        return Vec2(self.x / magnitude, self.y / magnitude)
        