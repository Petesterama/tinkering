import os
import sys
import subprocess
import time


infileTEST = "D:/meaces2.mov"
infile = sys.argv[1]
in_ext = infile[-3:]
outfile = infile[:-3] + "mp4"
print(outfile)
"""
if in_ext == "mov":
    subprocess.call( ["ffmpeg", "-i", infile, "-c:v", "libx264", "-crf", "12", "-pix_fmt", "yuv420p", "-preset", "slow", outfile ] )
else:
    pass
"""
subprocess.call( ["ffmpeg", "-i", infile, "-c:v", "libx264", "-crf", "22", "-pix_fmt", "yuv420p", "-preset", "slow", outfile ] )    
