#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "MeshComponent.h"
#include "UIComponent.h"
#include "PortalMeshComponent.h"
#include "Game.h"
#include "Portal.h"
#include <GL/glew.h>

Renderer::Renderer(Game* game)
: mGame(game)
, mSpriteShader(nullptr)
, mSpriteVerts(nullptr)
, mMeshShader(nullptr)
, mPortalShader(nullptr)
, mWindow(nullptr)
, mContext(nullptr)
, mScreenWidth(1024.0f)
, mScreenHeight(768.0f)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(float width, float height)
{
	mScreenWidth = width;
	mScreenHeight = height;

	// Set OpenGL attributes
#ifndef __EMSCRIPTEN__
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("ITP Portal", 100, 100, static_cast<int>(mScreenWidth),
							   static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);
	// Turn on vsync
	SDL_GL_SetSwapInterval(1);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();

	// Make sure we can create/compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// Create quad for drawing sprites
	CreateSpriteVerts();

	return true;
}

void Renderer::Shutdown()
{
	UnloadData();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
	// Destroy textures
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// Destroy meshes
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	// Fix for issue where "mouse grab" doesn't always work
	Uint32 windowFlags = SDL_GetWindowFlags(mWindow);
	if (windowFlags & SDL_WINDOW_INPUT_FOCUS)
	{
		int x = static_cast<int>(mScreenWidth / 2.0f);
		int y = static_cast<int>(mScreenHeight / 2.0f);
		SDL_WarpMouseInWindow(mWindow, x, y);
	}

	// Draw the main framebuffer
	Draw3DScene(mView, mProjection, static_cast<unsigned>(mScreenWidth),
				static_cast<unsigned>(mScreenHeight));

	// Draw the scene for the blue portal then orange portal, if they exist
	Portal* bluePortal = mGame->GetBluePortal();
	Portal* orangePortal = mGame->GetOrangePortal();

	for (unsigned i = 0; i < MAX_PORTAL_RECURSIONS; i++)
	{
		if (bluePortal && orangePortal)
		{
			glEnable(GL_CULL_FACE);
			glEnable(GL_CLIP_DISTANCE0);

			Draw3DScene(mBluePortal.mView, mProjection, static_cast<unsigned>(mScreenWidth),
						static_cast<unsigned>(mScreenHeight), orangePortal, &mBluePortal,
						BLUE_MASK | i);

			Draw3DScene(mOrangePortal.mView, mProjection, static_cast<unsigned>(mScreenWidth),
						static_cast<unsigned>(mScreenHeight), bluePortal, &mOrangePortal,
						ORANGE_MASK | i);

			// Recalculate the views for the next recursion
			PortalViewRecurse(mBluePortal, bluePortal, orangePortal);
			PortalViewRecurse(mOrangePortal, orangePortal, bluePortal);

			glDisable(GL_CULL_FACE);
			glDisable(GL_CLIP_DISTANCE0);
		}
	}

	glDisable(GL_CULL_FACE);

	// Now disable depth buffering
	glDisable(GL_DEPTH_TEST);
	// Now turn off depth writing and enable alpha blending (for meshes with alpha)
	glDepthMask(GL_FALSE);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);

	// Activate sprite shader/verts
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	// Draw UI components
	for (auto ui : mUIComps)
	{
		ui->Draw(mSpriteShader);
	}

	// Swap the buffers
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddMeshComp(MeshComponent* mesh, bool usesAlpha)
{
	if (!usesAlpha)
	{
		mMeshComps.emplace_back(mesh);
	}
	else
	{
		mMeshCompsAlpha.emplace_back(mesh);
	}
}

void Renderer::RemoveMeshComp(MeshComponent* mesh, bool usesAlpha)
{
	if (!usesAlpha)
	{
		auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
		mMeshComps.erase(iter);
	}
	else
	{
		auto iter = std::find(mMeshCompsAlpha.begin(), mMeshCompsAlpha.end(), mesh);
		mMeshCompsAlpha.erase(iter);
	}
}

void Renderer::AddUIComp(UIComponent* comp)
{
	mUIComps.emplace_back(comp);
}

void Renderer::RemoveUIComp(UIComponent* comp)
{
	auto iter = std::find(mUIComps.begin(), mUIComps.end(), comp);
	mUIComps.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
			return tex;
		}
		else
		{
			delete tex;
			return nullptr;
		}
	}
	return tex;
}

Mesh* Renderer::GetMesh(const std::string& fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
			return m;
		}
		else
		{
			delete m;
			return nullptr;
		}
	}
	return m;
}

bool Renderer::LoadShaders()
{
	// Create sprite shader
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite"))
	{
		return false;
	}

	mSpriteShader->SetActive();
	// Set the view-projection matrix
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	// Create basic mesh shader
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/BasicMesh"))
	{
		return false;
	}

	mMeshShader->SetActive();
	// Set the view-projection matrix
	mView = Matrix4::Identity;
	mProjection = Matrix4::CreateOrtho(mScreenWidth, mScreenHeight, 1000.0f, -1000.0f);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// Create portal shader
	mPortalShader = new Shader();
	if (!mPortalShader->Load("Shaders/Portal"))
	{
		return false;
	}

	mPortalShader->SetActive();
	mPortalShader->SetIntUniform("uTexture", 0);
	mPortalShader->SetIntUniform("uMask", 1);
	mPortalShader->SetIntUniform("uRenderTarget", 2);
	mPortalProjection = Matrix4::CreatePerspectiveFOV(1.22f, 512.0f, 512.0f, 10.0f, 100000.0f);

	return true;
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f, 0.5f,  0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f,  0.5f,  0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,  -0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f	 // bottom left
	};

	unsigned int indices[] = {0, 1, 2, 2, 3, 0};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::Draw3DScene(const Matrix4& view, const Matrix4& projection, unsigned int viewWidth,
						   unsigned int viewHeight, class Actor* portal, PortalData* portalData,
						   unsigned int stencilMask)
{
	// Set viewport size based on scale
	glViewport(0, 0, viewWidth, viewHeight);

	// Set the clear color to light grey
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Clear the color/depth buffer buffer
	glDepthMask(GL_TRUE);
	if (!portal)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	else
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	// Enable depth buffering/disable alpha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	if (!portal)
	{
		glStencilFunc(GL_ALWAYS, 0, 0);
	}
	else
	{
		glStencilFunc(GL_EQUAL, stencilMask, stencilMask);
	}
	// Set the mesh shader active
	mMeshShader->SetActive();

	Vector4 plane;
	if (portal)
	{
		// Setup clip plane
		Vector3 planeNormal = portal->GetQuatForward();
		// Bring this in very slightly so we don't see the exit portal rendered inside the entry
		Vector3 planePos = portal->GetPosition();
		float d = -Vector3::Dot(planeNormal, planePos) - 5.0f;
		plane = Vector4(planeNormal, d);
		mMeshShader->SetVector4Uniform("uClipPlane", plane);
		// Update view-projection matrix
		mMeshShader->SetMatrixUniform("uViewProj", view * projection);
	}
	else
	{
		// Update view-projection matrix
		mMeshShader->SetMatrixUniform("uViewProj", view * projection);
	}

	// Draw mesh components
	for (auto mc : mMeshComps)
	{
		mc->Draw(mMeshShader);
	}

	// Now turn off depth writing and enable alpha blending (for meshes with alpha)
	glDepthMask(GL_FALSE);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);

	// Sort alpha objects based on depth.
	// This is not perfect for lasers because it uses the depth of the owner
	Matrix4 viewProj = view * projection;
	std::sort(mMeshCompsAlpha.begin(), mMeshCompsAlpha.end(),
			  [&viewProj](MeshComponent* a, MeshComponent* b) {
				  // Get depth of a
				  Vector3 aPos = a->GetOwner()->GetPosition();
				  Vector3 aProj = Vector3::TransformWithPerspDiv(aPos, viewProj);
				  float aDepth = aProj.z;

				  // Get depth of b
				  Vector3 bPos = b->GetOwner()->GetPosition();
				  Vector3 bProj = Vector3::TransformWithPerspDiv(bPos, viewProj);
				  float bDepth = bProj.z;
				  return aDepth > bDepth;
			  });

	// Draw mesh components with alpha
	glDisable(GL_CULL_FACE);
	for (auto mc : mMeshCompsAlpha)
	{
		PortalMeshComponent* pmc = dynamic_cast<PortalMeshComponent*>(mc);
		if (mc->GetOwner() != portal || !pmc)
		{
			if (pmc)
			{
				Portal* portalActor = dynamic_cast<Portal*>(mc->GetOwner());
				if (portal)
				{
					glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
				}
				else
				{
					unsigned int portalMask = BLUE_MASK;
					if (portalActor != mGame->GetBluePortal())
					{
						portalMask = ORANGE_MASK;
					}
					glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
					glStencilFunc(GL_ALWAYS, portalMask, portalMask);
				}
				mPortalShader->SetActive();
				mPortalShader->SetVector4Uniform("uClipPlane", plane);
				mPortalShader->SetMatrixUniform("uViewProj", view * projection);
				mc->Draw(mPortalShader);
				mMeshShader->SetActive();
				if (portal)
				{
					glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
				}
				else
				{
					glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
					glStencilFunc(GL_ALWAYS, 0, 0);
				}
			}
			else
			{
				mc->Draw(mMeshShader);
			}
		}
	}

	glEnable(GL_CULL_FACE);
	glDisable(GL_STENCIL_TEST);
}

Vector3 Renderer::Unproject(const Vector3& screenPoint) const
{
	// Convert screenPoint to device coordinates (between -1 and +1)
	Vector3 deviceCoord = screenPoint;
	deviceCoord.x /= mScreenWidth * 0.5f;
	deviceCoord.y /= mScreenHeight * 0.5f;

	// Transform vector by unprojection matrix
	Matrix4 unprojection = mView * mProjection;
	unprojection.Invert();
	return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

void Renderer::PortalViewRecurse(PortalData& portalData, Portal* entryPortal, Portal* exitPortal)
{
	// TODO: Implement Lab 10
}
