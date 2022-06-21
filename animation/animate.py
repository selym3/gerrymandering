from os import remove
from PIL import Image
import imageio.v2 as imageio
import moviepy.editor as mp
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

    clip = mp.VideoFileClip(output_gif)
    clip.write_videofile(output_gif.replace('gif', 'mp4'))
    clip.close()
    
    print(output_gif + "or" + output_gif.replace('gif', 'mp4'))

if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit("Too few arguments")

    animate(sys.argv[1])
