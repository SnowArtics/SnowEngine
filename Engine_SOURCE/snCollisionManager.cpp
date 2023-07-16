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
		// 두 충돌체의 ID bool값을 확인
		ColliderID id = {};
		id.left = left->GetColliderID();
		id.right = right->GetColliderID();

		// 충돌정보를 가져온다
		std::map<UINT64, bool>::iterator iter
			= mCollisionMap.find(id.id);

		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(id.id, false));
			iter = mCollisionMap.find(id.id);
		}
		//여기서 겹쳤는지 확인.
		if (Intersect(left, right))
		{
			// 충돌
			if (iter->second == false)
			{
				//최초 충돌
				left->OnCollisionEnter(right);
				right->OnCollisionEnter(left);
				iter->second = true;
			}
			else
			{
				// 충돌 중
				left->OnCollisionStay(right);
				right->OnCollisionStay(left);
			}
		}
		else
		{
			// 충돌 X
			if (iter->second == true)
			{
				// 충돌하고 있다가 나갈떄
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
		// 네모 네모 충돌
		// 분리축 이론

		// To do... (숙제)
		// 분리축이 어렵다 하시는분들은
		// 원 - 원 충돌
		if (leftType == eColliderType::Circle && rightType == eColliderType::Circle)
			return CircleToCircleCollision(left, right);
		else if (leftType == eColliderType::Rect && rightType == eColliderType::Rect)
			return RectToRectCollision(left, right);
		else if ((leftType == eColliderType::Rect && rightType == eColliderType::Circle))
			return CircleToRectCollision(left, right);
		else if ((leftType == eColliderType::Circle && rightType == eColliderType::Rect))
			return CircleToRectCollision(right, left);
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

	bool CollisionManager::RectToRectCollision(Collider2D* left, Collider2D* right)
	{
		std::wstring leftName = left->GetOwner()->GetName();
		std::wstring rightName = right->GetOwner()->GetName();
		Vector3 leftCenter = left->GetDebugMesh().position;
		Vector3 rightCenter = right->GetDebugMesh().position;
		std::vector<Vector3> leftVertexs = left->GetDebugMesh().vertexs;
		std::vector<Vector3> rightVertexs = right->GetDebugMesh().vertexs;

		std::vector<Vector3> leftNormals;
		std::vector<Vector3> rightNormals;

		//각 직사각형의 변에 해당하는 법선벡터 생성.
		for (int i = 0; i < leftVertexs.size(); i++) {
			Vector3 tempNormal;
			Vector3 leftNormal;
			if (i == 3) {
				tempNormal = leftVertexs[0] - leftVertexs[3];
				tempNormal.z = leftVertexs[0].z;
			}
			else {
				tempNormal = leftVertexs[i + 1] - leftVertexs[i];
				tempNormal.z = leftVertexs[0].z;
			}
			leftNormal.x = -tempNormal.y;
			leftNormal.y = tempNormal.x;
			leftNormal.z = tempNormal.z;
			leftNormal.Normalize();
			leftNormals.push_back(leftNormal);
		}
		for (int i = 0; i < rightVertexs.size(); i++) {
			Vector3 tempNormal;
			Vector3 rightNormal;
			if (i == 3) {
				tempNormal = rightVertexs[0] - rightVertexs[3];
			}
			else {
				tempNormal = rightVertexs[i + 1] - rightVertexs[i];
			}
			rightNormal.x = -tempNormal.y;
			rightNormal.y = tempNormal.x;
			rightNormal.z = tempNormal.z;
			rightNormal.Normalize();
			rightNormals.push_back(rightNormal);
		}
		//생성한 법선벡터(축)에 직사각형의 정점들을 투영시키고(내적) 선이 겹치는지 확인
		for (int i = 0; i < leftNormals.size(); i++) {
			float leftMin = 99999999, leftMax = -99999999, rightMin = 99999999, rightMax = -99999999;
			for (int j = 0; j < leftVertexs.size(); j++) {
				if (leftMin > (leftNormals[i].Dot(leftVertexs[j])))
					leftMin = (leftNormals[i].Dot(leftVertexs[j]));
				if (leftMax < (leftNormals[i].Dot(leftVertexs[j])))
					leftMax = (leftNormals[i].Dot(leftVertexs[j]));
			}
			for (int j = 0; j < rightVertexs.size(); j++) {
				if (rightMin > (leftNormals[i].Dot(rightVertexs[j])))
					rightMin = (leftNormals[i].Dot(rightVertexs[j]));
				if (rightMax < (leftNormals[i].Dot(rightVertexs[j])))
					rightMax = (leftNormals[i].Dot(rightVertexs[j]));
			}
			float leftDis = leftNormals[i].Dot(leftCenter);
			float rightDis = leftNormals[i].Dot(rightCenter);

			if (abs(leftDis - rightDis) > abs(((leftMax - leftMin) / 2 + (rightMax - rightMin) / 2)))
				return false;
		}
		for (int i = 0; i < rightNormals.size(); i++) {
			float leftMin = 99999999, leftMax = -99999999, rightMin = 99999999, rightMax = -99999999;
			for (int j = 0; j < leftVertexs.size(); j++) {
				if (leftMin > (rightNormals[i].Dot(leftVertexs[j])))
					leftMin = (rightNormals[i].Dot(leftVertexs[j]));
				if (leftMax < (rightNormals[i].Dot(leftVertexs[j])))
					leftMax = (rightNormals[i].Dot(leftVertexs[j]));
			}
			for (int j = 0; j < rightVertexs.size(); j++) {
				if (rightMin > (rightNormals[i].Dot(rightVertexs[j])))
					rightMin = (rightNormals[i].Dot(rightVertexs[j]));
				if (rightMax < (rightNormals[i].Dot(rightVertexs[j])))
					rightMax = (rightNormals[i].Dot(rightVertexs[j]));
			}
			float leftDis = rightNormals[i].Dot(leftCenter);
			float rightDis = rightNormals[i].Dot(rightCenter);

			if (abs(leftDis - rightDis) > abs(((leftMax - leftMin) / 2 + (rightMax - rightMin) / 2)))
				return false;
		}

		return true;
	}

	bool CollisionManager::CircleToCircleCollision(Collider2D* left, Collider2D* right)
	{
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

	bool CircleToRectCollision(Collider2D* left, Collider2D* right) 
	{
		//left가 무조건 Rect이고, right가 무조건 Circle이다.
		float minDistance = INFINITY;

		Vector3 circlePos = right->GetDebugMesh().position;
		std::vector<Vector3> rectVertices = left->GetDebugMesh().vertexs;

		for (int i = 0; i < rectVertices.size(); i++) {

		}
	}

	float CollisionManager::Magnitude(float x, float y, float z)
	{
		return std::sqrt(x * x + y * y + z * z);
	}
}
