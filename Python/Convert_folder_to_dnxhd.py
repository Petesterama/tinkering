import os
import sys
import subprocess
import shutil
from threading import Thread

def convert(infile, outfile):
    subprocess.call( ["ffmpeg", "-i", infile, "-c:v", "dnxhd", "-vf", "scale=1920:1080", "-pix_fmt", "yuv422p", "-b:v", "36M",  outfile ] )


folder = os.path.abspath('D:\Ayla_more_videos')
files = os.listdir(folder)


for file in files:

    if os.path.isfile(file):    
        ext = file[:-3]
        basename = os.path.basename(file)
        outfile = os.path.join(folder, 'dnxhd', basename[:-4] + "_dnxhd.mov")
        Thread(target = convert, args=(file, outfile)).start()