#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL2/SDL_log.h>
#include "Math.h"

Mesh::Mesh()
: mVertexArray(nullptr)
, mRadius(0.0f)
{
}

Mesh::~Mesh()
{
}

bool Mesh::Load(const std::string& fileName, Renderer* renderer)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Mesh %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Mesh %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// Check the version
	if (ver != 1)
	{
		SDL_Log("Mesh %s not version 1", fileName.c_str());
		return false;
	}

	mShaderName = doc["shader"].GetString();

	// Skip the vertex format/shader for now
	// (This is changed in a later chapter's code)
	size_t vertSize = 8;

	// Load textures
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		SDL_Log("Mesh %s has no textures, there should be at least one", fileName.c_str());
		return false;
	}

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		// Is this texture already loaded?
		std::string texName = textures[i].GetString();
		Texture* t = renderer->GetTexture(texName);
		if (t == nullptr)
		{
			// If it's null, use the default texture
			t = renderer->GetTexture("Assets/Textures/Default.png");
		}
		mTextures.emplace_back(t);
	}

	// Load in the vertices
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no vertices", fileName.c_str());
		return false;
	}

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);

	mRadius = 0.0f;
	Vector3 minPoint(Math::Infinity);
	Vector3 maxPoint(Math::NegInfinity);

	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// For now, just assume we have 8 elements
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != 8)
		{
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		Vector3 pos(static_cast<float>(vert[0].GetDouble()),
					static_cast<float>(vert[1].GetDouble()),
					static_cast<float>(vert[2].GetDouble()));
		mRadius = Math::Max(mRadius, pos.LengthSq());

		// Update min/max points
		minPoint.x = Math::Min(minPoint.x, pos.x);
		minPoint.y = Math::Min(minPoint.y, pos.y);
		minPoint.z = Math::Min(minPoint.z, pos.z);
		maxPoint.x = Math::Max(maxPoint.x, pos.x);
		maxPoint.y = Math::Max(maxPoint.y, pos.y);
		maxPoint.z = Math::Max(maxPoint.z, pos.z);

		// Add the floats
		for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
		{
			vertices.emplace_back(static_cast<float>(vert[j].GetDouble()));
		}
	}

	// We were computing length squared earlier
	mRadius = Math::Sqrt(mRadius);

	// Now calculate the bounds array
	mBounds[0] = minPoint;
	mBounds[1] = Vector3(maxPoint.x, minPoint.y, minPoint.z);
	mBounds[2] = Vector3(minPoint.x, maxPoint.y, minPoint.z);
	mBounds[3] = Vector3(minPoint.x, minPoint.y, maxPoint.z);
	mBounds[4] = Vector3(minPoint.x, maxPoint.y, maxPoint.z);
	mBounds[5] = Vector3(maxPoint.x, minPoint.y, maxPoint.z);
	mBounds[6] = Vector3(maxPoint.x, maxPoint.y, minPoint.z);
	mBounds[7] = maxPoint;

	// Load in the indices
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// Now create a vertex array
	mVertexArray = new VertexArray(
		vertices.data(), static_cast<unsigned>(vertices.size()) / static_cast<unsigned>(vertSize),
		indices.data(), static_cast<unsigned>(indices.size()));
	return true;
}

void Mesh::Unload()
{
	delete mVertexArray;
	mVertexArray = nullptr;
}

Texture* Mesh::GetTexture(size_t index)
{
	if (index < mTextures.size())
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}
