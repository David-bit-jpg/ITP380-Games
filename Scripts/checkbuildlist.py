#!/usr/bin/env python
import os, subprocess,sys

files = sys.argv[1]
#print(files)
fileSplit = files.split(',')
filtered = list(filter(lambda file: "Lab" in file, fileSplit))
#print(filtered)

build_list = []

for file_filter in filtered:
	file_filter_splits = file_filter.split('/')
	if file_filter_splits[0] not in build_list:
		build_list.append(file_filter_splits[0])

# If there's nothing to build then go ahead and add Lab01 by default
if len(build_list) == 0:
	build_list.append("Lab01")
		
build_list.sort()
#print(build_list)
out_file = open("BuildActual.txt", "w", newline='\n')
for build_temp in build_list:
	out_file.write(build_temp + '\n')
