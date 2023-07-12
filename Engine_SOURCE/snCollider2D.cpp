#include "snCollider2D.h"
#include "snGameObject.h"
#include "snRenderer.h"

namespace sn
{
	Collider2D::Collider2D()
		: Component(eComponentType::Collider2D)
		, mTransform(nullptr)
		, mSize(Vector2::One)
		, mCenter(Vector2::Zero)
	{
	}
	Collider2D::~Collider2D()
	{
	}
	void Collider2D::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
	}

	void Collider2D::Update()
	{
	}
	void Collider2D::LateUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		graphics::DebugMesh mesh = {};
		mesh.position = tr->GetPosition();
		mesh.scale = tr->GetScale();
		mesh.rotation = tr->GetRotation();
		mesh.type = eColliderType::Rect;

		renderer::PushDebugMeshInfo(mesh);
	}
	void Collider2D::Render()
	{
	}
}
