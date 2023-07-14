#include "snCollider2D.h"
#include "snGameObject.h"
#include "snRenderer.h"

namespace sn
{
	UINT Collider2D::mColliderNumber = 0;
	Collider2D::Collider2D(eColliderType type)
		: Component(eComponentType::Collider2D)
		, mTransform(nullptr)
		, mSize(Vector2::One)
		, mCenter(Vector2::Zero)
		, mType(type)
		, mMesh{}
	{
		mColliderNumber++;
		mColliderID = mColliderNumber;
	}
	Collider2D::~Collider2D()
	{
	}
	void Collider2D::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
		Vector3 scale = mTransform->GetScale();
		scale.x *= mSize.x;
		scale.y *= mSize.y;

		Vector3 pos = mTransform->GetPosition();
		pos.x += mCenter.x;
		pos.y += mCenter.y;

		mPosition = pos;

		mMesh.position = pos;
		mMesh.scale = scale;
		mMesh.rotation = mTransform->GetRotation();
		mMesh.type = mType;
		if (mType == eColliderType::Rect) {

		}
		else if (mType == eColliderType::Circle) {
			mMesh.radius = scale.x / 2.f;
		}
	}

	void Collider2D::Update()
	{
	}
	void Collider2D::LateUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		Vector3 scale = tr->GetScale();
		scale.x *= mSize.x;
		scale.y *= mSize.y;

		Vector3 pos = tr->GetPosition();
		pos.x += mCenter.x;
		pos.y += mCenter.y;

		mPosition = pos;

		mMesh.position = pos;
		mMesh.scale = scale;
		mMesh.rotation = tr->GetRotation();
		mMesh.type = mType;
		if (mType == eColliderType::Rect) {

		}
		else if (mType == eColliderType::Circle) {
			mMesh.radius = scale.x / 2.f;
		}

		renderer::PushDebugMeshInfo(mMesh);
	}
	void Collider2D::Render()
	{
	}
	void Collider2D::OnCollisionEnter(Collider2D* other)
	{
		const std::vector<Script*>& scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionEnter(other);
		}
	}
	void Collider2D::OnCollisionStay(Collider2D* other)
	{
		const std::vector<Script*>& scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionStay(other);
		}
	}
	void Collider2D::OnCollisionExit(Collider2D* other)
	{
		const std::vector<Script*>& scripts
			= GetOwner()->GetComponents<Script>();

		for (Script* script : scripts)
		{
			script->OnCollisionExit(other);
		}
	}
}
