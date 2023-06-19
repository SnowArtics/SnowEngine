#include "snPlayScene.h"
#include "snTransform.h"
#include "snMeshRenderer.h"

namespace sn {
	PlayScene::PlayScene()
	{
	}
	PlayScene::~PlayScene()
	{
	}
	void sn::PlayScene::Initialize()
	{
		GameObject* player = new GameObject();
		AddGameObject(eLayerType::Player, player);
		player->AddComponent<MeshRenderer>();

		/*GameObject* player2 = new GameObject();
		AddGameObject(eLayerType::Player, player2);
		player2->AddComponent<MeshRenderer>();

		Transform* tr = player->GetComponent<Transform>();
		tr->SetPosition(Vector3(0.5f, 0.5f, 0.0f));*/
	}

	void sn::PlayScene::Update()
	{
		Scene::Update();
	}

	void sn::PlayScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void sn::PlayScene::Render()
	{
		Scene::Render();
	}
}