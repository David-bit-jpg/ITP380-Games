import json
fileName = "HeightMap.json"

with open(fileName, 'r') as file:
	data = json.load(file)
	for layer in data["layers"]:
		if "objects" in layer and layer["name"] == "Enemy":
			#print(layer["name"])
			outFile = open("Path.csv", 'w')
			outFile.write("Type,CellX,CellY\n")
			object = layer["objects"][0]
			startX = object["x"]
			startY = object["y"]
			for coord in object["polyline"]:
				line = "Node,"
				line += str(int((startY + coord["y"]) / 32)) + ','
				line += str(int((startX + coord["x"]) / 32))
				outFile.write(line + '\n')
