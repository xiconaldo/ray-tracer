# RT-Template

A very simple C++11 template code for the development of ray tracers.

# Dependencies

This software depends on the GLM library (included) and use a python 3 script to run 
(included in Ubuntu).

# Compiling and Running

I've compiled and executed this software only in Linux (Ubuntu 16.04 LTS).
To compile the program, you need to call 

$ make DEBUG=0  

at the project root directory.  

To render the hardcoded scene, issue the following command from the project root directory:  

$ python3 run.py x_resolution y_resolution spp num_threads output_file_name poweroff_option

where

x_resolution:	   horizontal resolution from the output file that will be generated;  
y_resolution:	   the same as above, but for vertical resolution;  
spp:		 	   samples per pixel. Number of rays launched for each pixel;  
num_threads: 	   number of threads used to process the pixel values;  
output_file_name:  output file name;  
poweroff_option:   if set to "poweroff", the computer will be shuted down at the end of  
				   the process. Note that in this case, the script must be called with  
				   root privilegies.  
