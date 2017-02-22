import subprocess

try:
	subprocess.call(["./swpathtracer"])
	subprocess.call(["eog", "output_image.ppm"])
except:
	pass