#include "LaserComponent.h"
#include "MeshComponent.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Texture.h"
#include "VertexArray.h"
#include "SegmentCast.h"
#include "Portal.h"
#include <vector>

LaserComponent::LaserComponent(Actor* owner)
: MeshComponent(owner)
{
	SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Meshes/Laser.gpmesh"));
}

LaserComponent::~LaserComponent()
{
}
void LaserComponent::Update(float deltaTime)
{
	if (mIsDisabled)
	{
		mLineSegments.clear();
	}
	else
	{
		mLastHitActor = nullptr;
		mLineSegments.clear();
		LineSegment ls =
			LineSegment(mOwner->GetWorldPosition(),
						mOwner->GetWorldPosition() + LASER_LEN * mOwner->GetWorldForward());
		CastInfo ci;
		if (SegmentCast(GetGame()->GetActors(), ls, ci, mActorToIgnore))
		{
			ls.mEnd = ci.mPoint;
			mLastHitActor = ci.mActor;
		}
		if (GetGame()->GetBluePortal() && GetGame()->GetOrangePortal())
		{
			Portal* entryPortal = nullptr;
			Portal* exitPortal = nullptr;
			if (SegmentCast(GetGame()->GetBluePortal(), ls, ci))
			{
				entryPortal = GetGame()->GetBluePortal();
				exitPortal = GetGame()->GetOrangePortal();
			}
			else if (SegmentCast(GetGame()->GetOrangePortal(), ls, ci))
			{
				entryPortal = GetGame()->GetOrangePortal();
				exitPortal = GetGame()->GetBluePortal();
			}
			if (entryPortal && exitPortal)
			{
				Vector3 newDirection = entryPortal->GetPortalOutVector(mOwner->GetWorldForward(),
																	   exitPortal, 0.0f);
				newDirection.Normalize();
				Vector3 newStartPos = entryPortal->GetPortalOutVector(ci.mPoint, exitPortal, 1.0f) +
									  SECOND_POS * newDirection;
				Vector3 newEndPos = newStartPos + LASER_LEN * newDirection;
				CastInfo ciSecond;
				LineSegment lsSecond = LineSegment(newStartPos, newEndPos);
				if (SegmentCast(GetGame()->GetActors(), lsSecond, ciSecond, exitPortal))
				{
					lsSecond.mEnd = ciSecond.mPoint;
				}
				mLastHitActor = ciSecond.mActor;
				mLineSegments.emplace_back(lsSecond);
			}
		}
		mLineSegments.emplace_back(ls);
		if (mLastHitActor)
		{
			Portal* lastPortal = dynamic_cast<Portal*>(mLastHitActor);
			if (lastPortal)
			{
				mLastHitActor = nullptr;
			}
		}
	}
}
void LaserComponent::Draw(class Shader* shader)
{
	for (LineSegment ls : mLineSegments)
	{
		if (mMesh)
		{
			// Set the world transform
			shader->SetMatrixUniform("uWorldTransform", TransformWorldMatrix(ls));
			// Set the active texture
			Texture* t = mMesh->GetTexture(mTextureIndex);
			if (t)
			{
				t->SetActive();
			}
			// Set the mesh's vertex array as active
			VertexArray* va = mMesh->GetVertexArray();
			va->SetActive();
			glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		}
	}
}
Matrix4 LaserComponent::TransformWorldMatrix(LineSegment ls)
{
	Matrix4 mScaleMatrix = Matrix4::CreateScale(ls.Length(), 1.0f, 1.0f);
	//calculate quat
	Quaternion quat;
	Vector3 mDirection = ls.mEnd - ls.mStart;
	mDirection.Normalize();
	Vector3 normal = mDirection;
	Vector3 defaultV = Vector3::UnitX;
	Vector3 rotationAxis = Vector3::Cross(defaultV, normal);
	rotationAxis.Normalize();
	float dot = Vector3::Dot(normal, defaultV);
	float angle = Math::Acos(dot);
	if (dot == 1.0f)
	{
		quat = Quaternion::Identity;
	}
	else if (dot == -1.0f)
	{
		quat = Quaternion(Vector3::UnitZ, Math::Pi);
	}
	else
	{
		quat = Quaternion(rotationAxis, angle);
	}
	Matrix4 mRotationMatrix = Matrix4::CreateFromQuaternion(quat);
	Matrix4 mTranslationMatrix = Matrix4::CreateTranslation(ls.PointOnSegment(0.5f));
	Matrix4 mResultMatrix = mScaleMatrix * mRotationMatrix * mTranslationMatrix;
	return mResultMatrix;
}