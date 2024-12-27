#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "Math.h"
#include "Mesh.h"

// Data for portals
struct PortalData
{
	class Texture* mTexture = nullptr;
	Matrix4 mView;
	Vector3 mCameraPos;
	Vector3 mCameraForward;
	Vector3 mCameraUp;
};

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize(float width, float height);
	void Shutdown();
	void UnloadData();

	void Draw();

	void AddMeshComp(class MeshComponent* mesh, bool usesAlpha);
	void RemoveMeshComp(class MeshComponent* mesh, bool usesAlpha);

	void AddUIComp(class UIComponent* comp);
	void RemoveUIComp(class UIComponent* comp);

	class Texture* GetTexture(const std::string& fileName);
	Mesh* GetMesh(const std::string& fileName);

	void SetViewMatrix(const Matrix4& view) { mView = view; }
	void SetProjectionMatrix(const Matrix4& proj) { mProjection = proj; }

	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }

	Vector3 Unproject(const Vector3& screenPoint) const;

	PortalData& GetBluePortal() { return mBluePortal; }
	PortalData& GetOrangePortal() { return mOrangePortal; }

private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void Draw3DScene(const Matrix4& view, const Matrix4& projection, unsigned int viewWidth,
					 unsigned int viewHeight, class Actor* portal = nullptr,
					 PortalData* portalData = nullptr, unsigned int stencilMask = 0);
	void PortalViewRecurse(PortalData& portalData, class Portal* entryPortal,
						   class Portal* exitPortal);

	// Map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures;
	// Map of meshes loaded
	std::unordered_map<std::string, Mesh*> mMeshes;

	// All mesh components drawn
	std::vector<class MeshComponent*> mMeshComps;
	// All mesh components w/ alpha
	std::vector<class MeshComponent*> mMeshCompsAlpha;
	// UI components to draw
	std::vector<class UIComponent*> mUIComps;

	// Game
	class Game* mGame;

	// Sprite shader
	class Shader* mSpriteShader;
	// Sprite vertex array
	class VertexArray* mSpriteVerts;

	// Mesh shader
	class Shader* mMeshShader;
	// Portal shader
	class Shader* mPortalShader;

	// View/projection for 3D shaders
	Matrix4 mView;
	Matrix4 mProjection;

	// Window
	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;

	// Width/height of screem
	float mScreenWidth;
	float mScreenHeight;

	PortalData mBluePortal;
	PortalData mOrangePortal;
	Matrix4 mPortalProjection;

	static const unsigned int BLUE_MASK = 64;
	static const unsigned int ORANGE_MASK = 128;
	static const unsigned int MAX_PORTAL_RECURSIONS = 6;
};
