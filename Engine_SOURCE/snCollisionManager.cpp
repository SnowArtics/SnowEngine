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
				else{
					int a = 0;
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
			std::vector<Collider2D*> leftCols = leftObj->GetComponents<Collider2D>();
			for (Collider2D* leftCol : leftCols) {
				if (leftCol == nullptr)
					continue;
				if (leftObj->GetState() != GameObject::eState::Active && leftObj->GetState() != GameObject::eState::Dead)
					continue;
				//if (leftCol->GetEnable() == false)
				//	continue;

				for (GameObject* rightObj : rights)
				{
					std::vector<Collider2D*> rightCols = rightObj->GetComponents<Collider2D>();			
					//�Ϲ����� ��Ȳ
					for (Collider2D* rightCol : rightCols) {
						if (rightCol == nullptr)
							continue;
						if (leftObj == rightObj)
							continue;
						if (rightObj->GetState() != GameObject::eState::Active && rightObj->GetState() != GameObject::eState::Dead)
							continue;
						//if (rightCol->GetEnable() == false)
						//	continue;						
						ColliderCollision(leftCol, rightCol);
						//�浹�� ������Ʈ�� �׾������ ���
						if ((leftObj->GetState() == GameObject::eState::Dead || rightObj->GetState() == GameObject::eState::Dead)) {
							leftObj->OnCollisionExit(rightCol, leftCol);
							rightObj->OnCollisionExit(leftCol, rightCol);
							leftCol->OnCollisionExit(rightCol, leftCol);
							rightCol->OnCollisionExit(leftCol, rightCol);
						}
					}					
				}
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
			iter->second == false;
		}

		if (iter->second == false && (left->GetEnable() == false || right->GetEnable() == false))
			return;

		//���⼭ ���ƴ��� Ȯ��.
		if (Intersect(left, right))
		{
			// �浹
			if (iter->second == false)
			{
				//���� �浹
				left->OnCollisionEnter(right, left);
				right->OnCollisionEnter(left, right);
				left->GetOwner()->OnCollisionEnter(right, left);
				right->GetOwner()->OnCollisionEnter(left, right);
				iter->second = true;
			}
			else
			{
				// �浹 ��
				left->OnCollisionStay(right, left);
				right->OnCollisionStay(left, right);
				left->GetOwner()->OnCollisionStay(right, left);
				right->GetOwner()->OnCollisionStay(left, right);
			}
		}
		else
		{
			// �浹 X
			if (iter->second == true)
			{
				// �浹�ϰ� �ִٰ� ������
				left->OnCollisionExit(right, left);
				right->OnCollisionExit(left, right);
				left->GetOwner()->OnCollisionExit(right, left);
				right->GetOwner()->OnCollisionExit(left, right);
				iter->second = false;
			}
		}		
	}

	bool CollisionManager::Intersect(Collider2D* left, Collider2D* right)
	{
		eColliderType leftType = left->GetColliderType();
		eColliderType rightType = right->GetColliderType();

		//�ΰ��� �浹ü �� �ϳ��� ��Ȱ��ȭ ���¸� ������ �浹 �ƴ� ����
		if (left->GetEnable() == false || right->GetEnable() == false)
			return false;
		// �׸� �׸� �浹
		// �и��� �̷�

		// To do... (����)
		// �и����� ��ƴ� �Ͻôºе���
		// �� - �� �浹
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
			row = iRight;
			col = iLeft;
		}
		else
		{
			row = iLeft;
			col = iRight;
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

		//�� ���簢���� ���� �ش��ϴ� �������� ����.
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
		//������ ��������(��)�� ���簢���� �������� ������Ű��(����) ���� ��ġ���� Ȯ��
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

	bool CollisionManager::CircleToRectCollision(Collider2D* left, Collider2D* right)
	{
		//left�� ������ Rect�̰�, right�� ������ Circle�̴�.
		//2�����̹Ƿ� ������ ����, ���� ������ �� ����, z���� 0���� �о���.
		float minDistance = 999999999;
		Vector3 minVector;

		Vector3 circlePos = right->GetDebugMesh().position;
		std::vector<Vector3> rectVertices = left->GetDebugMesh().vertexs;

		for (int i = 0; i < rectVertices.size(); i++) {
			//Rect �� ���� ���͸� ����.
			Vector3 side;
			if (i == 3) {
				side = rectVertices[0] - rectVertices[i];
			}
			else {
				side = rectVertices[i + 1] - rectVertices[i];
			}
			//Rect�� �������� ���� �߽ɱ����� ���͸� ����.
			Vector3 toCircle = circlePos - rectVertices[i];
			toCircle.z = 0.f;
			//�� ���͸� ����
			float sideLength = side.Length();//���� ����
			Vector3 sideDirection = side / sideLength;//���� ��������
			//sideDirection.z = rectVertices[i].z;
			float pointFloat = toCircle.Dot(sideDirection);
			pointFloat = std::clamp(pointFloat, 0.0f, sideLength);
			//���� �߽ɿ��� ���� ����� ���� ��ġ��
			Vector3 pointPos = rectVertices[i] + sideDirection * pointFloat;
			pointPos.z = rectVertices[i].z;
			//���� �߽ɿ��� ���� ����� ���� ���̸� ����
			Vector3 resultVec = pointPos - circlePos;
			resultVec.z = 0.f;
			float resultLength = resultVec.Length();
			if (minDistance > resultLength) {
				minDistance = resultLength;
				minVector = resultVec;
			}
		}

		if (minDistance > right->GetDebugMesh().radius)
			return false;
		else
			return true;
	}

	float CollisionManager::Magnitude(float x, float y, float z)
	{
		return std::sqrt(x * x + y * y + z * z);
	}
}
