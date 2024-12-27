#include "UIComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"

UIComponent::UIComponent(Actor* owner)
: Component(owner)
{
	GetGame()->GetRenderer()->AddUIComp(this);
}

UIComponent::~UIComponent()
{
	GetGame()->GetRenderer()->RemoveUIComp(this);
}

void UIComponent::Draw(Shader* shader)
{
}

void UIComponent::DrawTexture(class Shader* shader, class Texture* texture, const Vector2& offset,
							  float scale, float angle)
{
	// Scale the quad by the width/height of texture
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(texture->GetWidth()) * scale,
											static_cast<float>(texture->GetHeight()) * scale, 1.0f);
	Matrix4 rotMat = Matrix4::CreateRotationZ(angle);
	// Translate to position on screen
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x, offset.y, 0.0f));
	// Set world transform
	Matrix4 world = scaleMat * rotMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	// Set current texture
	texture->SetActive();
	// Draw quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
