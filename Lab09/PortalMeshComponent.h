#pragma once
#include "MeshComponent.h"
#include "Math.h"
#include "SegmentCast.h"
#include <vector>

class PortalMeshComponent : public MeshComponent
{
public:
	PortalMeshComponent(class Actor* owner);

	// Draw this mesh component
	void Draw(class Shader* shader) override;

private:
	class Texture* mMaskTexture = nullptr;
	class Texture* mBlackTexture = nullptr;
};
