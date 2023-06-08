#include "snScene.h"

namespace sn {
	sn::Scene::Scene()
	{
	}

	sn::Scene::~Scene()
	{
	}

	void sn::Scene::Initialize()
	{
		// 여기서 초기 게임 맵데이터를 세팅해줘야 한다.
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
