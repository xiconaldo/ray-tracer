import subprocess
import time

start = time.time()

try:
	subprocess.call(["./swpathtracer"])
except:
	pass

end = time.time()
total = int(end-start)

hours = total//3600
minutes = (total%3600)//60
seconds = (total%60)

print("Rendering time: ", str(hours), "h", str(minutes), "min", str(seconds), "s")

try:
	subprocess.call(["convert", "images/output_image.ppm", "images/output_image.jpg"])
	subprocess.call(["eog", "images/output_image.jpg"])
except:
	pass