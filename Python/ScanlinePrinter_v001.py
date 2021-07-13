#scanline renderer python

def scanlinePrint(resolution):
    
    resX = resolution[0]
    resY = resolution[1]

    for y in range(0,resY):
        scan = ""
        for x in range(0,resX):
            scan += "#"
        print(scan)
            

scanlinePrint([100,15])