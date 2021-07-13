import os.path
import subprocess

OIIO_Path = r"C:\Users\petes\htoa\htoa-5.6.0.1_rcd4b13f_houdini-18.5.462\htoa-5.6.0.1_rcd4b13f_houdini-18.5.462\scripts\bin\oiiotool.exe"

inputpath = r"C:\AVED3\AVE_309_035_430\AVE_309_035_430_v305\AVE_309_035_430_v305.%04d.exr"


infile = os.path.basename(inputpath)
inDir = os.path.dirname(inputpath)

outfile = infile.replace("exr", "jpg")
outDir = r"C:\Users\petes\Documents\OIIO_Testing\AVE_309_035_430_v305"

outpath = os.path.join(outDir, outfile )

print(outpath)

frames = "1001-1066"



cmds = [OIIO_Path, "-frames", frames, inputpath, "-o", outpath ]

x = subprocess.Popen(cmds)
x.communicate()
x.wait()
