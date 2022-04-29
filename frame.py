from typing import Tuple
import numpy as np
from PIL import Image

from vec2 import Vec2

class Vertex:
    Color = Tuple[float, float, float]

    def __init__(self, position: Vec2, color: Color = (1.0, 1.0, 1.0)):
        self.position = position
        self.color = color

    def lerp(self, rhs, t: float):
        return Vertex(
            self.position.lerp(rhs.position, t),
            (
                self.color[0] * (1 - t) + rhs.color[0] * t,
                self.color[1] * (1 - t) + rhs.color[1] * t,
                self.color[2] * (1 - t) + rhs.color[2] * t
            )
        )
class Frame:
    Color = Tuple[int, int, int]
    CHANNELS = 3

    def __init__(self, width: int, height: int):
        self.width, self.height = width, height

        self.buffer = np.zeros(
            shape=(height, width, Frame.CHANNELS),
            dtype=np.uint8
        )

    def set_color(self, col: int, row: int, color: Color) -> None:
        self.buffer[row, col] = color

    def image(self) -> Image:
        return Image.fromarray(self.buffer)
    
    def line(self, a: Vertex, b: Vertex):
        # create vector from one point to the other
        dp = (b.position - a.position)

        # normalize vector to the number of pixels
        pixels = round(0.5 + max(abs(dp.x), abs(dp.y)))
        if pixels == 0: return
        dp /= Vec2(pixels, pixels)

        # fill pixels
        at = Vec2(a.position.x, a.position.y)

        for _ in range(pixels): 
            t = (at - a.position).mag() / (b.position - a.position).mag()
            self.set_color(int(at.x), int(at.y), a.lerp(b, t).color)
            at += dp



if __name__ == "__main__":
    Frame(100, 300).image().save("images/test/tall.png")
    Frame(300, 100).image().save("images/test/short.png")

    fr = Frame(100, 100)
    for i in range(0, 100):
        fr.set_color(i, i, (0, 255, 0))
        fr.set_color(i, i // 2, (255, 0, 0))

    im = fr.image()
    im.save("images/test/line.png")
    im.show()

