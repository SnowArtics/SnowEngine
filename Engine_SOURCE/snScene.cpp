#include "snScene.h"
#include "snPlayer.h"
#include "snItem.h"

namespace sn {
	sn::Scene::Scene()
	{
	}

	sn::Scene::~Scene()
	{
		for (GameObject* gameObj : mGameObjects) {
			delete gameObj;
		}

		mGameObjects.clear();
	}

	void sn::Scene::Initialize()
	{
		// 여기서 초기 게임 맵데이터를 세팅해줘야 한다.

		GameObject* player = new Player();
		mGameObjects.push_back(player);

		for (float i = -0.96f; i <0.96f ; i+=0.2f) {
			for (float j = -0.96f; j < 0.96f; j += 0.2f) {
				//플레이어 시작위치에는 아이템 생성이 안됨.
				if (i > -0.1f && i<0.1f && j>-0.2f&& j < 0.2f)
					break;

				Item* item = new Item();

				//가로세로 0.2f칸 만큼 구간을 나눠서 아이템 생성
				float posX = (rand() % 20 * 0.01) + i;
				float posY = (rand() % 20 * 0.01) + j;
				item->SetPos(Vector3(posX, posY, 0.0f));

				float colorR = (rand() % 500 + 200) * 0.001;
				float colorG = (rand() % 500 + 200) * 0.001;
				float colorB = (rand() % 500 + 200) * 0.001;

				item->SetColor(Vector4(colorR, colorG, colorB,1.0f));

				mGameObjects.push_back(item);
			}
		}

		for (GameObject* gameObj : mGameObjects) {
			gameObj->Initialize();
		}
	}

	void sn::Scene::Update()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->Update();
		}
	}

	void sn::Scene::LateUpdate()
	{
	}

	void sn::Scene::Render()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			gameObj->Render();
		}
	}
}
