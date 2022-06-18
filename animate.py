from os import remove
from PIL import Image
import imageio.v2 as imageio
import glob

import os
import sys

def animate(name):
    filenames = glob.glob("anim/" + name + "*");
    filenames = sorted(filenames)
    print(filenames)
    images = []
    for filename in filenames:
        images.append(imageio.imread(filename))
    
    output_gif = name+".gif"
    imageio.mimsave(output_gif, images, fps=60)

    print(output_gif)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit("Too few arguments")

    animate(sys.argv[1])