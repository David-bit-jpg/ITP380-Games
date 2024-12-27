import json
fileName = "tileset.json"

with open(fileName, 'r') as file:
	data = json.load(file)
	outFile = open("Paths.csv", 'w')
	outFile.write("id,pathable\n")
	for tile in data["tiles"]:
		#print(tile)
		for object in tile["properties"]:
			line = str(tile["id"]) + ','
			line += str(object["value"])
			#print(line)
			outFile.write(line + '\n')
