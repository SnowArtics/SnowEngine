#include "snShopScene.h"
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
#include "snRenderer.h"
namespace sn
{
	ShopScene::ShopScene()
	{
	}
	ShopScene::~ShopScene()
	{
	}
	void ShopScene::Initialize()
	{
		{
			//ShopBackground
			GameObject* Background = new GameObject();
			AddGameObject(eLayerType::Background, Background);
			MeshRenderer* mr = Background->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"ShopBackgroundMaterial01"));
			Background->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.1f));
			//Background->GetComponent<Transform>()->SetScale(Vector3(6.7f, 4.0f, 2.0f));
			Background->GetComponent<Transform>()->SetScale(Vector3(8.18725f, 10.f, 2.0f));
			//Background->GetComponent<Transform>()->SetScale(Vector3(4.093625f, 5.f, 2.0f));
		}
		{
			//Main Camera
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Player, camera);
			camera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
			Camera* cameraComp = camera->AddComponent<Camera>();
			cameraComp->TurnLayerMask(eLayerType::UI, false);
			camera->AddComponent<CameraScript>();
		}

		//UI Camera
		{
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Player, camera);
			camera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
			Camera* cameraComp = camera->AddComponent<Camera>();
			cameraComp->DisableLayerMasks();
			cameraComp->TurnLayerMask(eLayerType::UI, true);
			//camera->AddComponent<CameraScript>();

			renderer::cameras.push_back(cameraComp);
			renderer::mainCamera = cameraComp;
		}
		Scene::Initialize();
	}
	void ShopScene::Update()
	{
		Scene::Update();
	}
	void ShopScene::LateUpdate()
	{
		Scene::LateUpdate();

		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"DungeonEntrance");
		}
	}
	void ShopScene::Render()
	{
		Scene::Render();
	}
	void ShopScene::OnEnter()
	{
		Initialize();
	}
	void ShopScene::OnExit()
	{
	}
}