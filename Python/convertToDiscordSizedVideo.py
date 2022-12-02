import os
import sys
import subprocess

infile = sys.argv[1]
in_ext = infile[-3:]
if in_ext == "mov":
    outfile = infile[:-3] + "mp4"
elif in_ext == "mp4":
    outfile = infile[:-4] + "_ffmpegConvert.mp4"

print(outfile)

subprocess.call( ["ffmpeg", "-i", infile, "-c:v", "libx264", "-crf", "22", "-pix_fmt", "yuv420p", "-preset", "slow",  outfile ] )