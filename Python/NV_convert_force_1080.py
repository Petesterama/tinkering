# intended to be used with a drag drop registry key edit thing.
# Will convert whatever video file is dropped onto the script into a small mp4.
# Uses FFMPEG and NVENC encoder


import os
import sys
import subprocess

infile = sys.argv[1]
in_ext = infile[-3:]
if in_ext == "mov":
    outfile = infile[:-3] + "mp4"
elif in_ext == "mp4":
    outfile = infile[:-4] + "_smol.mp4"


print(outfile)

subprocess.call( ["ffmpeg", "-i", infile, "-vf", "scale=1920:1080" ,"-c:v", "h264_nvenc", "-b:v", "4M", outfile ] ) # high quality