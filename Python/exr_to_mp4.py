import os
import sys
import subprocess

infile = "D:/Volcano_flyover/volcano_flyover_comp_v005_4k/4k_cops_v001ffmpeg/volcano_flyover_comp_v005_4k.%02d.exr"
in_ext = infile[-3:]
if in_ext == "mov":
    outfile = infile[:-3] + "mp4"
else:
    outfile = infile[:-4] + "_ffmpegConvert.mp4"

print(outfile)

subprocess.call( ["ffmpeg", "-i", infile, "-c:v", "libx264", "-crf", "24", "-pix_fmt", "yuv420p", "-preset", "slow", "-vf", "scale=1280:720", outfile ] )