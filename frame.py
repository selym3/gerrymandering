from typing import Tuple
import numpy as np
from PIL import Image

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

