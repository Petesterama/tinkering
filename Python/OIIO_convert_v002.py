import os.path
import subprocess
import math
from threading import Thread

OIIO_Path = r"C:\Users\petes\htoa\htoa-5.6.0.1_rcd4b13f_houdini-18.5.462\htoa-5.6.0.1_rcd4b13f_houdini-18.5.462\scripts\bin\oiiotool.exe"

inputpath = r"C:\AVED3\AVE_309_035_430\AVE_309_035_430_v305\AVE_309_035_430_v305.%04d.exr"


infile = os.path.basename(inputpath)
inDir = os.path.dirname(inputpath)

outfile = infile.replace("exr", "jpg")
outDir = r"C:\Users\petes\Documents\OIIO_Testing\AVE_309_035_430_v305"

outpath = os.path.join(outDir, outfile )

print(outpath)

#frames = "1001-1066"

first = 1001
last = 1066


numframes = last - first + 1
numtasks = 8

taskSize = math.floor(float(numframes) / (float(numtasks) - 1.0) )
finalTaskSize =  int(float(numframes) % float(numtasks - 1))

for x in range(0, numtasks):
    if not x == numtasks-1:
        ff = first + ( x * taskSize )
        lf = first + ( (x + 1) * taskSize ) - 1
        frames = "{}-{}".format(ff,lf)
        print(frames)
    else:
        ff = first + ( x * taskSize )
        lf = ff + finalTaskSize - 1  
        frames = "{}-{}".format(ff,lf)
        print(frames)
 
    cmds = [OIIO_Path, "-frames", frames, inputpath, "-o", outpath ]

    Thread(target=subprocess.call, args=(cmds,)).start()

"""
cmds = [OIIO_Path, "-frames", frames, inputpath, "-o", outpath ]

x = subprocess.Popen(cmds)
x.communicate()
x.wait()
"""