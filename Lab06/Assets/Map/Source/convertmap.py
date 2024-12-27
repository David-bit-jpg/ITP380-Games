import json
fileName = "overworld.json"

with open(fileName, 'r') as file:
	data = json.load(file)
	for layer in data["layers"]:
		if "objects" in layer:
			#print(layer["name"])
			outFile = open("Objects.csv", 'w')
			outFile.write("Type,x,y,width,height,1,2,3,4,5,6\n")
			for object in layer["objects"]:
				line = object["type"] + ','
				line += str(object["x"] * 2) + ','
				line += str(object["y"] * 2) + ','
				line += str(object["width"] * 2) + ','
				line += str(object["height"] * 2) + ','
				if "properties" in object:
					num_props = len(object["properties"])
					index = 0
					for prop in object["properties"]:
						line += str(prop["value"])
						if index != (num_props - 1):
							line += ','
						index += 1
					while index < num_props:
						line += ','
						index += 1
				else:
					line += ",,,,,"
				#print(line)
				outFile.write(line + '\n')
