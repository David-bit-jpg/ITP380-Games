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
			if 'warning:' in line or 'error:' in line:
				splits = line.split('warning: ')
				iserror = False
				if len(splits) == 1:
					splits = line.split('error: ')
					iserror = True
				
				try:
					if len(splits) > 1:
						#print(splits)
						leftsplits = splits[0].split(':')
						filePath = leftsplits[0]
						filePath = filePath.replace(cwd + '/', '')
							
						message = splits[1]
						#print(message)
						
						jsonOut.append({
							'file': filePath,
							'line': int(leftsplits[1]),
							'title': 'Build Warning (Mac)' if not (iserror) else 'Build Error (Mac)',
							'message': message,
							'annotation_level': 'warning' if not(iserror) else 'failure'
						})
				except:
					jsonOut.append({
						'file': "Build.txt",
						'line': 1,
						'title': 'Build Warning (Mac)' if not (iserror) else 'Build Error (Mac)',
						'message': 'Failed to generate annotation for this warning/error. Please check the actions build log.',
						'annotation_level': 'warning' if not(iserror) else 'failure'
					})
					pass

		jsonStr = json.dumps(jsonOut, indent=2)
		#print(jsonStr)
		with open('diagnostics.json', 'w') as outFile:
			outFile.write(jsonStr)
	
if __name__ == '__main__':
	main()
