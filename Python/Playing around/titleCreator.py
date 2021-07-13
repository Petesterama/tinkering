l = ['blender', 'maya', 'nuke', 'cinema4d', 'houdini', 'zbrush', 'mudbox', 'mari', 'substance designer', '3ds max']
for x in l:
	for y in l:
		if y is not x:
			print ("{} vs {}, which is better?".format(x,y))
