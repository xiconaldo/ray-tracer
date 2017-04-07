import subprocess
import time
import calendar
import sys

def print_dif_time(current_time):
	hours = current_time//3600
	minutes = (current_time%3600)//60
	seconds = (current_time%60)

	print(str(hours), "h", str(minutes), "min", str(seconds), "s")


now = time.strftime("%Y-%b-%d %H:%M:%S")
print("\nStarted at %s\n" %now)

start = time.time()

try:
	subprocess.call(["./swpathtracer"] + sys.argv[1:], stdout = sys.stdout)
except:
	pass

end = time.time()

now = time.strftime("%Y-%b-%d %H:%M:%S")
print("\nFinished at %s" %now)

total = int(end-start)

print("Rendering time: ", end = "")
print_dif_time(total)
print("")

if(len(sys.argv) > 5):
	output_image = sys.argv[5]
else:
	output_image = "output_image"

try:
	subprocess.call(["mkdir", "-p", "images_ppm", "images_png"])
	subprocess.call(["mv", output_image + ".ppm", "images_ppm/"])	
	subprocess.call(["convert", "images_ppm/" + output_image + ".ppm", "images_png/" + output_image + ".png"])	

	if(len(sys.argv) > 6 and sys.argv[6] == "off"):	
		subprocess.call(["sudo", "poweroff"])
	else:
		subprocess.call(["eog", "images_ppm/" + output_image + ".ppm"])
except:
	pass
	