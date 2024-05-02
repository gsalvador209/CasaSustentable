#!/usr/bin/env python

# Will split a png cubemap/skymap image produced by blender into 6 separated image files for use in a skybox within unity
# Requires Python Imaging Library > http://www.pythonware.com/products/pil/

# The author takes no responsibility for any damage this script might cause,
# feel free to use, change and or share this script.
# 2013-07, CanOfColliders, m@canofcolliders.com

from PIL import Image
import sys, os

path = os.path.abspath("C:/Users/Salvador/Documents/Proyecto cg/CasaSustentable/bin/textures/cielo") + "\\";
processed = False
	

def processImage(path, name):
	img = Image.open(os.path.join(path, name))
	size = img.size[0] / 4 # splits the width of the image by 3, expecting the 3x2 layout blender produces.
	splitAndSave(img, size, 0, size, addToFilename(name, "posy"))
	splitAndSave(img, 0, size, size, addToFilename(name, "negx"))
	splitAndSave(img, size, size, size, addToFilename(name, "posz"))
	splitAndSave(img, 2*size, size, size, addToFilename(name, "posx"))
	splitAndSave(img, 3*size, size, size, addToFilename(name, "negz"))
	splitAndSave(img, size, 2*size, size, addToFilename(name, "negy"))

def addToFilename(name, add):
	name = name.split('.')
	return add + "." + name[1]

def splitAndSave(img, startX, startY, size, name):
	area = (startX, startY, startX + size, startY + size)
	saveImage(img.crop(area), path, name)

def saveImage(img, path, name):
	try:
		img.save(os.path.join(path, name))
	except:
		print("*   ERROR: Could not convert image.")
		pass

for arg in sys.argv:
	if ".png" in arg or ".jpg" in arg:
		processImage(path, arg)
		processed = True

if not processed:
	print("*  ERROR: No Image")
	print("   usage: 'python script.py image-name.png'")
