import os
import sys
import subprocess

infile = sys.argv[1]
in_ext = infile[-3:]
outfile = infile[:-4] + "_dnxhd.mov"

basename = os.path.basename(infile)
folder = os.path.dirname(infile)
outfile = os.path.join(folder, 'dnxhd', basename[:-4] + "_dnxhd.mov")

print(outfile)

subprocess.call( ["ffmpeg", "-i", infile, "-c:v", "dnxhd", "-vf", "scale=1920:1080", "-pix_fmt", "yuv422p", "-b:v", "36M",  outfile ] )