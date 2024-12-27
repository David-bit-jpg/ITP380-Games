#!/usr/bin/env python
import json
import os
	
def main():
	cwd = os.getcwd()
	#print(cwd)
	with open('build/diagnostics.txt') as f:
		jsonOut = []
		while True:
			line = f.readline()
			if not line:
				break

			#print(line)
			if 'warning' in line or 'error' in line:
				splits = line.split('warning ')
				#print(splits)
				iserror = False
				if len(splits) == 1:
					splits = line.split('error ')
					iserror = True
				
				try:
					if len(splits) > 1:
						#print(splits)
						filePath = splits[0]
						filePath = filePath.replace(cwd + '\\', '')
						filePath = filePath.replace('\\', '/')
						filePathSplits = filePath.split('(')
						filePath = filePathSplits[0]
						lineSplits = filePathSplits[1].split(',')
						#print(filePath)
						message = splits[1]
						messageSplits = message.split(' [')
						message = messageSplits[0]
						#print(message)
						#if not(iserror):
						#	messageSplits = splits[4].rpartition('[')
						#	message = messageSplits[0]
						
						#message = message[1:-1]
						
						if not lineSplits[0].isnumeric():
							lineSplits = lineSplits[0].split(')')
						
						jsonOut.append({
							'file': filePath,
							'line': int(lineSplits[0]),
							'title': 'Build Warning (Windows)' if not (iserror) else 'Build Error (Windows)',
							'message': message,
							'annotation_level': 'warning' if not(iserror) else 'failure'
						})
				except:
					jsonOut.append({
						'file': "Build.txt",
						'line': 1,
						'title': 'Build Warning (Windows)' if not (iserror) else 'Build Error (Windows)',
						'message': 'Failed to generate annotation for this warning/error. Please check the build log.',
						'annotation_level': 'warning' if not(iserror) else 'failure'
					})
					pass

		jsonStr = json.dumps(jsonOut, indent=2)
		#print(jsonStr)
		with open('diagnostics.json', 'w') as outFile:
			outFile.write(jsonStr)
	
if __name__ == '__main__':
	main()
