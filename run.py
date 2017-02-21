import subprocess

try:
	subprocess.call(["./swpathtracer"])
	subprocess.call(["eog", "output_image.ppm"], timeout = 10)
except:
	pass