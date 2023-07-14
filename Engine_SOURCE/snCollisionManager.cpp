#include "snCollisionManager.h"
#include "snGameObject.h"
#include "snScene.h"
#include "snSceneManager.h"
#include "snLayer.h"
#include "snCollider2D.h"

namespace sn
{	std::bitset<LAYER_MAX> CollisionManager::mMatrix[LAYER_MAX] = {};
	std::map<UINT64, bool> CollisionManager::mCollisionMap = {};

	void CollisionManager::Initialize()
	{
		for (UINT i = 0; i < LAYER_MAX; i++) {
			mMatrix[i].set();
		}
	}

	void CollisionManager::Update()
	{
		for (UINT column = 0; column < (UINT)eLayerType::Camera; column++)
		{
			for (UINT row = column; row < (UINT)eLayerType::Camera; row++)
			{
				if (mMatrix[column][row] == true)
				{
					LayerCollision((eLayerType)column, (eLayerType)row);
				}
			}
		}
	}

	void CollisionManager::LayerCollision(eLayerType left, eLayerType right)
	{
		Scene* activeScene = SceneManager::GetActiveScene();

		const std::vector<GameObject*>& lefts
			= activeScene->GetLayer(left).GetGameObjects();
		const std::vector<GameObject*>& rights
			= activeScene->GetLayer(right).GetGameObjects();

		for (GameObject* leftObj : lefts)
		{
			Collider2D* leftCol = leftObj->GetComponent<Collider2D>();
			if (leftCol == nullptr)
				continue;
			if (leftObj->GetState()
				!= GameObject::eState::Active)
				continue;

			for (GameObject* rightObj : rights)
			{
				Collider2D* rightCol = rightObj->GetComponent<Collider2D>();
				if (rightCol == nullptr)
					continue;
				if (leftObj == rightObj)
					continue;
				if (rightObj->GetState()
					!= GameObject::eState::Active)
					continue;

				ColliderCollision(leftCol, rightCol);
			}
		}
	}

	void CollisionManager::ColliderCollision(Collider2D* left, Collider2D* right)
	{
		// �� �浹ü�� ID bool���� Ȯ��
		ColliderID id = {};
		id.left = left->GetColliderID();
		id.right = right->GetColliderID();

		// �浹������ �����´�
		std::map<UINT64, bool>::iterator iter
			= mCollisionMap.find(id.id);

		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(id.id, false));
			iter = mCollisionMap.find(id.id);
		}
		//���⼭ ���ƴ��� Ȯ��.
		if (Intersect(left, right))
		{
			// �浹
			if (iter->second == false)
			{
				//���� �浹
				left->OnCollisionEnter(right);
				right->OnCollisionEnter(left);
				iter->second = true;
			}
			else
			{
				// �浹 ��
				left->OnCollisionStay(right);
				right->OnCollisionStay(left);
			}
		}
		else
		{
			// �浹 X
			if (iter->second == true)
			{
				// �浹�ϰ� �ִٰ� ������
				left->OnCollisionExit(right);
				right->OnCollisionExit(left);
				iter->second = false;
			}
		}
	}

	bool CollisionManager::Intersect(Collider2D* left, Collider2D* right)
	{
		eColliderType leftType = left->GetColliderType();
		eColliderType rightType = right->GetColliderType();
		// �׸� �׸� �浹
		// �и��� �̷�

		// To do... (����)
		// �и����� ��ƴ� �Ͻôºе���
		// �� - �� �浹
		if(leftType == eColliderType::Circle && rightType == eColliderType::Circle)
			return CircleCollision(left, right);

		return false;
	}

	void CollisionManager::SetLayer(eLayerType left, eLayerType right, bool enable)
	{
		UINT row = -1;
		UINT col = -1;

		UINT iLeft = (UINT)left;
		UINT iRight = (UINT)right;

		if (iLeft <= iRight)
		{
			row = iLeft;
			col = iRight;
		}
		else
		{
			row = iRight;
			col = iLeft;
		}

		mMatrix[col][row] = enable;
	}

	void CollisionManager::Clear()
	{
		mMatrix->reset();
		mCollisionMap.clear();
	}
	bool CollisionManager::CircleCollision(Collider2D* left, Collider2D* right)
	{
		std::wstring leftName = left->GetOwner()->GetName();
		std::wstring rightName = right->GetOwner()->GetName();
		Vector3 leftPos = left->GetDebugMesh().position;
		Vector3 rightPos = right->GetDebugMesh().position;
		float leftRadius = left->GetDebugMesh().radius;
		float rightRadius = right->GetDebugMesh().radius;
		float distance = abs(leftRadius) + abs(rightRadius);

		Vector3 distanceVector = leftPos - rightPos;
		float magnitude = Magnitude(distanceVector.x, distanceVector.y, 0.f);

		if (magnitude <= distance)
			return true;
		else
			return false;
	}

	float CollisionManager::Magnitude(float x, float y, float z)
	{
		return std::sqrt(x * x + y * y + z * z);
	}
}
