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

#subprocess.call( ["ffmpeg", "-i", infile, "-c:v", "h264_nvenc", "-b:v", "5M", "-vf", "scale=1280:720", outfile ] ) # low quality
subprocess.call( ["ffmpeg", "-i", infile, "-c:v", "h264_nvenc", "-b:v", "10M", outfile ] ) # high quality