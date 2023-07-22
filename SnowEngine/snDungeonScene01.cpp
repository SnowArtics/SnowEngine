#include "snDungeonScene01.h"
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

#include <random>
#include "snRenderer.h"

namespace sn {
	DungeonScene01::DungeonScene01()
	{
	}

	DungeonScene01::~DungeonScene01()
	{
	}

	void DungeonScene01::Initialize()
	{
		{
			//던전 배경 로딩 //1.593406
			GameObject* Background = new GameObject();
			AddGameObject(eLayerType::Background, Background);
			MeshRenderer* mr = Background->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"DungeonBackgroundMaterial01"));
			Background->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			//Background->GetComponent<Transform>()->SetScale(Vector3(6.7f, 4.0f, 2.0f));
			Background->GetComponent<Transform>()->SetScale(Vector3(9.77777735f, 5.5f, 2.0f));
		}

#pragma region Object
		//오브젝트를 랜덤으로 생성

		// 시드값을 현재 시간으로 설정
		std::random_device rd;
		// 랜덤 엔진 생성
		std::mt19937 mt(rd());
		// 균일 분포를 가진 랜덤 숫자 생성기 생성 (0 이상 99 이하)
		std::uniform_real_distribution<float> distX(1.f, 8.f);	
		std::uniform_real_distribution<float> distY(0.5f, 5.0f);

		{
			GameObject* Object = new GameObject();
			AddGameObject(eLayerType::Obstacle, Object);
			MeshRenderer* mr = Object->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"FountainMaterial"));

			Object->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 10.0f, -1.f));
			float YToXRatio = mr->GetMaterial()->GetTexture()->GetYToXRatio();
			Object->GetComponent<Transform>()->SetScale(Vector3(0.2f*YToXRatio, 0.2f, 0.0f));
		}
#pragma endregion
		{
			//인벤토리 UI 생성
			GameObject* UI = new GameObject();
			UI->SetName(L"InventoryBase");
			AddGameObject(eLayerType::UI, UI);
			MeshRenderer* mr = UI->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"UIInventoryBaseMaterial01"));
			UI->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			UI->GetComponent<Transform>()->SetScale(Vector3(7.576642335766424f, 4.f, 2.0f));
			UI->SetEnable(false);
		}
		{
			//UI 생성 부분
			{
				GameObject* UI = new GameObject();
				AddGameObject(eLayerType::UI, UI);
				MeshRenderer* mr = UI->AddComponent<MeshRenderer>();
				mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				mr->SetMaterial(Resources::Find<Material>(L"UIBaseMaterial01"));
				UI->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
				UI->GetComponent<Transform>()->SetScale(Vector3(9.599958f, 5.4f, 2.0f));
			}
			{
				GameObject* UI = new GameObject();
				AddGameObject(eLayerType::UI, UI);
				MeshRenderer* mr = UI->AddComponent<MeshRenderer>();
				mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				mr->SetMaterial(Resources::Find<Material>(L"UIBaseMaterial02"));
				UI->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
				UI->GetComponent<Transform>()->SetScale(Vector3(9.599958f, 5.4f, 2.0f));
			}
			{
				//UI 우측 상단 무기 UI 생성
				GameObject* UI = new GameObject();
				AddGameObject(eLayerType::UI, UI);
				MeshRenderer* mr = UI->AddComponent<MeshRenderer>();
				mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				mr->SetMaterial(Resources::Find<Material>(L"UIBaseMaterial02"));
				UI->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
				UI->GetComponent<Transform>()->SetScale(Vector3(9.599958f, 5.4f, 2.0f));
			}
			{
				GameObject* UI = new GameObject();
				AddGameObject(eLayerType::UI, UI);
				MeshRenderer* mr = UI->AddComponent<MeshRenderer>();
				mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				mr->SetMaterial(Resources::Find<Material>(L"UIGoldMaterial01"));
				UI->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
				UI->GetComponent<Transform>()->SetScale(Vector3(9.599958f, 5.4f, 2.0f));
			}
			{
				GameObject* UI = new GameObject();
				AddGameObject(eLayerType::UI, UI);
				MeshRenderer* mr = UI->AddComponent<MeshRenderer>();
				mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				mr->SetMaterial(Resources::Find<Material>(L"UIWeaponMaterial02"));
				UI->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
				UI->GetComponent<Transform>()->SetScale(Vector3(9.599958f, 5.4f, 2.0f));
			}
			{
				GameObject* UI = new GameObject();
				AddGameObject(eLayerType::UI, UI);
				MeshRenderer* mr = UI->AddComponent<MeshRenderer>();
				mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				mr->SetMaterial(Resources::Find<Material>(L"UIWeaponMaterial01"));
				UI->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
				UI->GetComponent<Transform>()->SetScale(Vector3(9.599958f, 5.4f, 2.0f));
			}
		}
		{
			//Main Camera
			GameObject* camera = new GameObject();
			AddGameObject(eLayerType::Player, camera);
			camera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
			Camera* cameraComp = camera->AddComponent<Camera>();
			cameraComp->EnableLayerMasks();
			cameraComp->TurnLayerMask(eLayerType::UI, false);
			camera->AddComponent<CameraScript>();
			renderer::cameras.push_back(cameraComp);
			renderer::mainCamera = cameraComp;
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
		}
		Scene::Initialize();
	}

	void DungeonScene01::Update()
	{
		Scene::Update();
	}

	void DungeonScene01::LateUpdate()
	{
		Scene::LateUpdate();
		if (Input::GetKeyDown(eKeyCode::SPACE))
		{
			SceneManager::LoadScene(L"DungeonScene02");
		}
		if (Input::GetKeyDown(eKeyCode::I))
		{
			GameObject* inven = Scene::Find(L"InventoryBase");
			std::wstring asdf = inven->GetName();
			if (inven->GetEnable() == true) inven->SetEnable(false);
			else inven->SetEnable(true);
		}
	}

	void DungeonScene01::Render()
	{
		Scene::Render();
	}

	void DungeonScene01::OnEnter()
	{
		Initialize();
	}

	void DungeonScene01::OnExit()
	{
		
	}

}
