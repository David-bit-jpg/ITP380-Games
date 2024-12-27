#!/usr/bin/env python
import json
import os
	
def main():
	cwd = os.getcwd()
	#print(cwd)
	if not os.path.exists('clang-format-out.txt'):
		jsonOut = []
		jsonOut.append({
						'file': "Build.txt",
						'line': 1,
						'title': 'Style Warning',
						'message': 'clang-format was not run because no labs successfully compiled.',
						'annotation_level': 'warning'
					})
		with open('diagnostics-format.json', 'w') as outFile:
			outFile.write(json.dumps(jsonOut, indent=2))
	else:
		with open('clang-format-out.txt') as f:
			jsonOut = []
			while True:
				line = f.readline()
				if not line:
					break

				#print(line)
				if '[-Wclang-format-violations]' in line:
					splits = line.split('error:')
					try:
						if len(splits) > 1:
							#print(splits)
							filePath = splits[0]
							#print(cwd)
							filePath = filePath.replace(cwd + '\\', '')
							filePath = filePath.replace('\\', '/')
							#print(filePath)
							filePathSplits = filePath.split(':')
							filePath = filePathSplits[0]
							
							
							jsonOut.append({
								'file': filePath,
								'line': int(filePathSplits[1]),
								'title': 'Style Warning',
								'message': 'clang-format found a formatting issue. Check the style guide and run clang-format on this file.',
								'annotation_level': 'warning'
							})
					except:
						jsonOut.append({
							'file': "Build.txt",
							'line': 1,
							'title': 'Style Warning',
							'message': 'Failed to generate annotation for this warning. Please check the actions build log.',
							'annotation_level': 'warning'
						})
						pass

			jsonStr = json.dumps(jsonOut, indent=2)
			#print(jsonStr)
			with open('diagnostics-format.json', 'w') as outFile:
				outFile.write(jsonStr)
	
if __name__ == '__main__':
	main()
