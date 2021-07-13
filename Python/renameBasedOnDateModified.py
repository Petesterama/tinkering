import os

path = r"C:\Users\petes\Pictures\AucklandBuildingProgress"
files = [f for f in os.listdir(path) if os.path.isfile(os.path.join(path,f))]

files2 =sorted(files, key = lambda file: os.path.getctime(path + "/" + file ) )

for x, filename in enumerate(files2,1):
    old = "{}/{}".format(path,filename)
    new = "{}/AucklandProgress_{}.jpg".format(path,x)
    os.rename(old,new)
