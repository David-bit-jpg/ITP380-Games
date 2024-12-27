#include "PortalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

PortalMeshComponent::PortalMeshComponent(Actor* owner)
: MeshComponent(owner, true)
{
	SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/Portal.gpmesh"));
	mMaskTexture = GetGame()->GetRenderer()->GetTexture("Assets/Textures/Portal/Mask.png");
	mBlackTexture = GetGame()->GetRenderer()->GetTexture("Assets/Textures/Cube/Black.png");
}

void PortalMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());

		// Set the active texture
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}

		// Set the mask and render target textures
		mMaskTexture->SetActive(1);

		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();

		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
