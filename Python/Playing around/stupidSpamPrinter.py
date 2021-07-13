import time
from random import random as rand

quote = "this is a stupid fucking piece of python code I don't even know why I made it lmao"

#print(quote[:26])
l = len(quote)
#print(l)
for x in range(0,(l*2)-1):
	if x < l:
		print("{} {}".format(quote[:x+1],""))
	else:
		print("{} {}".format(quote[:l-x-1],""))

	time.sleep(0.02)