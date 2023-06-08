#include "snScene.h"

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
		// ���⼭ �ʱ� ���� �ʵ����͸� ��������� �Ѵ�.

		GameObject* player = new GameObject();
		mGameObjects.push_back(player);
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
