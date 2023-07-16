#pragma once
#include "snGraphics.h"
#include "snComponent.h"
#include "snTransform.h"

namespace sn
{
	class Collider2D : public Component
	{
	public:
		Collider2D(eColliderType type);
		~Collider2D();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void OnCollisionEnter(Collider2D* other);
		void OnCollisionStay(Collider2D* other);
		void OnCollisionExit(Collider2D* other);

		//벡터를 주어진 각도만큼 회전시키는 함수
		Vector3 RotateVector(const Vector3 vec, float angle);

		void SetType(eColliderType type) { mType = type; }
		void SetSize(Vector2 size) { mSize = size; }
		void SetCenter(Vector2 size) { mCenter = size; }

		UINT GetColliderID() { return mColliderID; }
		eColliderType GetColliderType() { return mType; }
		graphics::DebugMesh GetDebugMesh() { return mMesh; }
		Vector3 GetPos() { return mPosition; }

	private:
		static UINT mColliderNumber;
		UINT mColliderID;
		eColliderType mType;
		Transform* mTransform;

		Vector3 mPosition;
		Vector2 mSize;
		Vector2 mCenter;

		graphics::DebugMesh mMesh;
	};
}
