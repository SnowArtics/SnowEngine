#include "snScene.h"
#include "snPlayer.h"
#include "snItem.h"
#include "snPlayer.h"

#include "snTime.h"

#include <math.h>

namespace sn {
	Scene::Scene()
		: mTime(0.f)
		, mItemNum(0)
	{
	}

	Scene::~Scene()
	{
		for (GameObject* gameObj : mGameObjects) {
			delete gameObj;
		}
		mGameObjects.clear();
	}

	void Scene::Initialize()
	{
		// 여기서 초기 게임 맵데이터를 세팅해줘야 한다.

		GameObject* player = new Player();
		player->SetName(L"Player");
		((Player*)player)->SetPos(Vector3(0.0f, 0.0f, 0.0f));
		((Player*)player)->SetColor(Vector4(0.3137f, 0.7373f, 0.8745f, 1.0f));
		((Player*)player)->SetScene(this);

		mGameObjects.push_back(player);

		for (float i = -0.96f; i <0.96f ; i+=0.2f) {
			for (float j = -0.96f; j < 0.96f; j += 0.2f) {
				//플레이어 시작위치에는 아이템 생성이 안됨.
				if (i > -0.1f && i<0.1f && j>-0.2f&& j < 0.2f)
					break;

				Item* item = new Item();
				item->SetName(L"Item" + std::to_wstring(mItemNum));
				item->SetScene(this);
				//가로세로 0.2f칸 만큼 구간을 나눠서 아이템 생성
				float posX = (rand() % 20 * 0.01) + i;
				float posY = (rand() % 20 * 0.01) + j;
				item->SetPos(Vector3(posX, posY, 0.0f));

				float colorR = (rand() % 500 + 200) * 0.001;
				float colorG = (rand() % 500 + 200) * 0.001;
				float colorB = (rand() % 500 + 200) * 0.001;

				item->SetColor(Vector4(colorR, colorG, colorB,1.0f));

				mGameObjects.push_back(item);

				mItemNum++;
			}			
		}

		for (GameObject* gameObj : mGameObjects) {
			gameObj->Initialize();
		}
	}

	void Scene::Update()
	{
		mTime += Time::DeltaTime();
		if (mTime >= 1.0f) {
			Item* item = new Item();
			item->SetName(L"Item" + std::to_wstring(mItemNum));
			item->SetScene(this);
			float posX = ((rand() % 2000 - 1000) * 0.001);
			float posY = ((rand() % 2000 - 1000) * 0.001);
			item->SetPos(Vector3(posX, posY, 0.0f));

			float colorR = (rand() % 500 + 200) * 0.001;
			float colorG = (rand() % 500 + 200) * 0.001;
			float colorB = (rand() % 500 + 200) * 0.001;

			item->SetColor(Vector4(colorR, colorG, colorB, 1.0f));

			item->Initialize();

			mGameObjects.push_back(item);
			mItemNum++;

			mTime = 0.f;
		}

		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->Update();
		}
	}

	void Scene::LateUpdate()
	{
		ColliderCheck();
		//RespawnItem();
	}

	void Scene::Render()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->Render();
		}
	}
	void Scene::ColliderCheck()
	{
		Vector3 playerPos = ((Player*)mGameObjects[0])->GetPos();

		for (UINT i = 1; i < mGameObjects.size();i++) {
			Vector3 itemPos = ((Item*)mGameObjects[i])->GetPos();

			float x = playerPos.x - itemPos.x;
			float y = playerPos.y - itemPos.y;

			//x = 0.108f;
			//y = 0.192f;
			if (abs(x) < 0.08f && abs(x) > 0.f && abs(y) < 0.15f && abs(y) > 0.f) {
				((Player*)mGameObjects[0])->OnCollision(mGameObjects[i]->GetName());
				((Item*)mGameObjects[i])->OnCollision(mGameObjects[0]->GetName());
			}
		}
	}

	int Scene::FindObjectIndex(std::wstring _objectName)
	{
		for (UINT i = 0; i < mGameObjects.size(); i++) {
			if (mGameObjects[i]->GetName() == _objectName) {
				return i;
			}
		}

		return -1;
	}

	bool Scene::DeleteObject(UINT _index)
	{
		if (!mGameObjects.empty() && _index < mGameObjects.size()&& _index !=0) {
			//Respawn* respawn = new Respawn;
			//respawn->Pos = ((Item*)mGameObjects[_index])->GetPos();
			//respawn->time = rand() % 3 + 1;
			//respawn->name = mGameObjects[_index]->GetName();
			//mRespawn.push(respawn);

			delete mGameObjects[_index];
			mGameObjects.erase(mGameObjects.begin() + _index);
			return true;
		}
		else {
			return false;
		}
	}
	void Scene::RespawnItem()
	{		
		if (!mRespawn.empty()) {
			mTime += Time::DeltaTime();
			Respawn* respawn = mRespawn.front();
			if (mTime >= respawn->time) {

				Item* item = new Item();
				item->SetName(respawn->name);
				item->SetScene(this);
				item->SetPos(respawn->Pos);

				float colorR = (rand() % 500 + 200) * 0.001;
				float colorG = (rand() % 500 + 200) * 0.001;
				float colorB = (rand() % 500 + 200) * 0.001;
				item->SetColor(Vector4(colorR, colorG, colorB, 1.0f));

				item->Initialize();

				mGameObjects.push_back(item);

				mTime = 0.f;
				mRespawn.pop();
			}
		}
		else {
			return;
		}
	}
}