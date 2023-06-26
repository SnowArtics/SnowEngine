#include "snDungeonScene.h"
#include "snGameObject.h"
#include "snMeshRenderer.h"
#include "snMesh.h"
#include "snMaterial.h"
#include "snTransform.h"
#include "snCamera.h"
#include "snCameraScript.h"

#include "snSceneManager.h"

#include "snResources.h"
#include "snInput.h"

namespace sn
{
	DungeonScene::DungeonScene()
	{
	}
	DungeonScene::~DungeonScene()
	{
	}
	void DungeonScene::Initialize()
	{
		{
			//배경 이미지1 생성
			GameObject* Background = new GameObject();
			AddGameObject(eLayerType::Background, Background);
			MeshRenderer* mr = Background->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"DungeonBackgroundMaterial01"));
			Background->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			//Background->GetComponent<Transform>()->SetScale(Vector3(6.7f, 4.0f, 2.0f));
			Background->GetComponent<Transform>()->SetScale(Vector3(9.77777735f, 5.5f, 2.0f));
		}

		{
			//Main Camera
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Player, camera);
			camera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
			Camera* cameraComp = camera->AddComponent<Camera>();
			camera->AddComponent<CameraScript>();
		}
	}
	void DungeonScene::Update()
	{
		Scene::Update();
	}
	void DungeonScene::LateUpdate()
	{
		Scene::LateUpdate();
	}
	void DungeonScene::Render()
	{
		Scene::Render();

		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SceneManager::LoadScene(L"TitleScene");
		}
	}
	void DungeonScene::OnEnter()
	{
		Initialize();
	}
	void DungeonScene::OnExit()
	{
	}
}