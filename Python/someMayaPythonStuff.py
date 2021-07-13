from pymel.core import *

list = ls(tex=True)
for t in list:   
    cs = t.colorSpace.get()
    
    if cs == "sRGB":
        t.colorSpace.set('Utility - sRGB - Texture')
        print("Converted {} colorspace {} to Utility - sRGB - Texture".format(t,cs) )
        
    if cs == "Raw":    
        t.colorSpace.set('Utility - Raw')
        print("Converted {} colorspace {} to Utility - Raw".format(t,cs) )
    print t.colorSpace.get() 



l = ls(sl=True, shapes=True)

for x in l:
	x.rsEnableDisplacement.set(0)