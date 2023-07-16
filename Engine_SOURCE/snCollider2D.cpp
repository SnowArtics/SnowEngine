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
			mMesh.vertexs.clear();
			//충돌체의 각 정점을 계산해서 메쉬에 넣어줌.
			Vector3 vertex1 = Vector3(pos.x - (scale.x / 2.f), pos.y - (scale.y / 2.f), pos.z);
			Vector3 dirVertex1 = vertex1 - pos;
			dirVertex1 = RotateVector(dirVertex1, mMesh.rotation.RadianToAngle().z);
			dirVertex1 = pos + dirVertex1;
			mMesh.vertexs.push_back(dirVertex1);

			Vector3 vertex2 = Vector3(pos.x - (scale.x / 2.f), pos.y + (scale.y / 2.f), pos.z);
			Vector3 dirVertex2 = vertex2 - pos;
			dirVertex2 = RotateVector(dirVertex2, mMesh.rotation.RadianToAngle().z);
			dirVertex2 = pos + dirVertex2;
			mMesh.vertexs.push_back(dirVertex2);

			Vector3 vertex3 = Vector3(pos.x + (scale.x / 2.f), pos.y + (scale.y / 2.f), pos.z);
			Vector3 dirVertex3 = vertex3 - pos;
			dirVertex3 = RotateVector(dirVertex3, mMesh.rotation.RadianToAngle().z);
			dirVertex3 = pos + dirVertex3;
			mMesh.vertexs.push_back(dirVertex3);

			Vector3 vertex4 = Vector3(pos.x + (scale.x / 2.f), pos.y - (scale.y / 2.f), pos.z);
			Vector3 dirVertex4 = vertex4 - pos;
			dirVertex4 = RotateVector(dirVertex4, mMesh.rotation.RadianToAngle().z);
			dirVertex4 = pos + dirVertex4;
			mMesh.vertexs.push_back(dirVertex4);
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
			mMesh.vertexs.clear();
			//충돌체의 각 정점을 계산해서 메쉬에 넣어줌.
			Vector3 vertex1 = Vector3(pos.x - (scale.x / 2.f), pos.y - (scale.y / 2.f), pos.z);
			Vector3 dirVertex1 = vertex1 - pos;
			dirVertex1 = RotateVector(dirVertex1, mMesh.rotation.RadianToAngle().z);
			dirVertex1 = pos + dirVertex1;
			mMesh.vertexs.push_back(dirVertex1);

			Vector3 vertex2 = Vector3(pos.x - (scale.x / 2.f), pos.y + (scale.y / 2.f), pos.z);
			Vector3 dirVertex2 = vertex2 - pos;
			dirVertex2 = RotateVector(dirVertex2, mMesh.rotation.RadianToAngle().z);
			dirVertex2 = pos + dirVertex2;
			mMesh.vertexs.push_back(dirVertex2);

			Vector3 vertex3 = Vector3(pos.x + (scale.x / 2.f), pos.y + (scale.y / 2.f), pos.z);
			Vector3 dirVertex3 = vertex3 - pos;
			dirVertex3 = RotateVector(dirVertex3, mMesh.rotation.RadianToAngle().z);
			dirVertex3 = pos + dirVertex3;
			mMesh.vertexs.push_back(dirVertex3);

			Vector3 vertex4 = Vector3(pos.x + (scale.x / 2.f), pos.y - (scale.y / 2.f), pos.z);
			Vector3 dirVertex4 = vertex4 - pos;
			dirVertex4 = RotateVector(dirVertex4, mMesh.rotation.RadianToAngle().z);
			dirVertex4 = pos + dirVertex4;
			mMesh.vertexs.push_back(dirVertex4);			
		}
		else if (mType == eColliderType::Circle) {
			mMesh.vertexs.clear();
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

	Vector3 Collider2D::RotateVector(const Vector3 vec, float angle) {
		float radians = angle * (3.14159265359f / 180.0f); // 각도를 라디안으로 변환
		float cosVal = std::cos(radians);
		float sinVal = std::sin(radians);

		Vector3 result;
		result.x = vec.x * cosVal - vec.y * sinVal;
		result.y = vec.x * sinVal + vec.y * cosVal;
		result.z = vec.z;
		return result;
	}


}
