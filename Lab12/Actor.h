#pragma once
#include <vector>
#include <string>
#include <SDL2/SDL_stdinc.h>
#include "Math.h"

enum class ActorState
{
	Active,
	Paused,
	Destroy
};

class Actor
{
public:
	Actor(class Game* game, Actor* parent = nullptr);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const Uint8* keyState, Uint32 mouseButtons, const Vector2& relativeMouse);

	// Getters/setters
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; }
	const Vector3& GetScale() const { return mScale; }
	void SetScale(Vector3 scale) { mScale = scale; }
	void SetScale(float scale) { mScale = Vector3(scale, scale, scale); }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }
	void SetWorldTransform(Matrix4 wt) { mWorldTransform = wt; }
	ActorState GetState() const { return mState; }
	void SetState(ActorState state) { mState = state; }

	class Game* GetGame() { return mGame; }

	// Returns component of type T, or null if doesn't exist
	template <typename T>
	T* GetComponent() const
	{
		for (auto c : mComponents)
		{
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr)
			{
				return t;
			}
		}

		return nullptr;
	}
	Vector3 GetForward() const { return Vector3(Math::Cos(mRotation), Math::Sin(mRotation), 0.0f); }
	void SetRollAngle(float r) { mRollAngle = r; }
	float GetRollAngle() const { return mRollAngle; }
	void CalcWorldTransform();
	void SetQuat(const Quaternion& quat) { mQuat = quat; }
	const Quaternion& GetQuat() const { return mQuat; }
	Vector3 GetQuatForward() const
	{
		Vector3 fwd = Vector3::Transform(Vector3::UnitX, mQuat);
		fwd.Normalize();
		return fwd;
	}
	Vector3 GetRight() const
	{
		return Vector3(Math::Cos(mRotation + Math::PiOver2), Math::Sin(mRotation + Math::PiOver2),
					   0.0f);
	}
	Matrix4 GetWorldRotTrans() const
	{
		Matrix4 rotationMatrixZ = Matrix4::CreateRotationZ(mRotation);
		Matrix4 translationMatrix = Matrix4::CreateTranslation(mPosition);
		Matrix4 rotationMatrixX = Matrix4::CreateRotationX(mRollAngle);
		Matrix4 quaternionMatrix = Matrix4::CreateFromQuaternion(mQuat);
		Matrix4 tempMatrix = rotationMatrixZ * rotationMatrixX * quaternionMatrix *
							 translationMatrix;
		if (mParent)
		{
			tempMatrix *= mParent->GetWorldRotTrans();
		}
		return tempMatrix;
	}

	Vector3 GetWorldPosition() const { return mWorldTransform.GetTranslation(); }

	Vector3 GetWorldForward() const { return mWorldTransform.GetXAxis(); }

	Actor* GetParent() const { return mParent; }

protected:
	// Any actor-specific update code (overridable)
	virtual void OnUpdate(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void OnProcessInput(const Uint8* keyState, Uint32 mouseButtons,
								const Vector2& relativeMouse);

	class Game* mGame;
	// Actor's state
	ActorState mState;

	// Transform
	Vector3 mPosition;
	Vector3 mScale;
	float mRotation;

	// Components
	std::vector<class Component*> mComponents;

private:
	friend class Component;
	// Adds component to Actor (this is automatically called
	// in the component constructor)
	void AddComponent(class Component* c);
	const std::vector<class Component*>& GetComponents();

	void AddChild(Actor* child);
	void RemoveChild(Actor* child);

	Matrix4 mWorldTransform;
	float mRollAngle = 0.0f;

	Quaternion mQuat;
	Actor* mParent;
	std::vector<Actor*> mChildren;
	bool mInheritScale = false;
};
