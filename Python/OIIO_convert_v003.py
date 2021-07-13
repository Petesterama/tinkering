import os.path
import subprocess
import math
from threading import Thread
import time


def rint(num):
    """Rounds toward the even number if equidistant"""
    return round(num + (num % 2 - 1 if (num % 1 == 0.5) else 0))

OIIO_Path = r"C:\Users\petes\htoa\htoa-5.6.0.1_rcd4b13f_houdini-18.5.462\htoa-5.6.0.1_rcd4b13f_houdini-18.5.462\scripts\bin\oiiotool.exe"

#inputpath = r"C:\AVED3\AVE_309_035_430\AVE_309_035_430_v305\AVE_309_035_430_v305.%04d.exr"
inputpath = r"D:\Google Drive\RandomRenders\InstaFLIP\InstaFLIP.%04d.exr"


infile = os.path.basename(inputpath)
inDir = os.path.dirname(inputpath)

outfile = infile.replace("exr", "jpg")
outDir = r"C:\Users\petes\Documents\OIIO_Testing\AVE_309_035_430_v305"

outpath = os.path.join(outDir, outfile )

first = 2
last = 120

numframes = last - first + 1
numtasks = 16 # concurrent tasks

if numframes < numtasks:
    numtasks = numframes

if numframes % numtasks == 0:
    evenTasks = True
    taskSize = int(numframes/numtasks)
    finalTaskSize = 0
else:
    evenTasks = False
    taskSize_floor = math.floor(numframes/numtasks)
    taskSize_ceil = math.ceil(numframes/numtasks)

    if taskSize_ceil * numtasks > numframes:
        taskSize = taskSize_floor
    else:
        taskSize = taskSize_ceil

    finalTaskSize =  numframes - taskSize * ( numtasks -1 )

print("task size is {}".format(taskSize))
print("final task size is {}".format(finalTaskSize))

tic = time.perf_counter()

for x in range(0, numtasks):

    if evenTasks or not x == numtasks-1:
        ff = first + ( x * taskSize )
        lf = first + ( (x + 1) * taskSize ) - 1
        frames = "{}-{}".format(ff,lf)
    else:
        ff = first + ( x * taskSize )
        lf = ff + finalTaskSize - 1  
        frames = "{}-{}".format(ff,lf)

    print("task {}: {}".format(x+1,frames))
 
    cmds = [OIIO_Path, "-frames", frames, inputpath, "-o", outpath ]

    Thread(target=subprocess.call, args=(cmds,)).start()

toc = time.perf_counter()
print(f"Completed in {toc-tic:0.4f} seconds") # not actually working because it just spawns the threads, then ends current thread. wait()?
