import os
import sys
import subprocess

infile = sys.argv[1]
in_ext = infile[-3:]
if in_ext == "mov":
    outfile = infile[:-3] + "1080.mp4"
elif in_ext == "mp4":
    outfile = infile[:-4] + "_ffmpegConvert_1080.mp4"

print(outfile)

subprocess.call( ["ffmpeg", "-i", infile, "-c:v", "libx264", "-crf", "16", "-pix_fmt", "yuv420p", "-preset", "slow", "-vf", "scale=1920:818", outfile ] )