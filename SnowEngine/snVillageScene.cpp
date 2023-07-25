#include "snVillageScene.h"
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
#include "snAnimator.h"

#include "snPlayerFSM.h"
#include "snMoveState.h"
#include "snIdleState.h"
#include "snRollState.h"

namespace sn
{
	VillageScene::VillageScene()
	{
	}
	VillageScene::~VillageScene()
	{
	}
	void VillageScene::Initialize()
	{
#pragma region VillageObject
		{
			//마을 배경 로그인 //1.218487394
			GameObject* Background = new GameObject();
			AddGameObject(eLayerType::Background, Background);
			MeshRenderer* mr = Background->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"VillageBackgroundMaterial01"));
			Background->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			Background->GetComponent<Transform>()->SetScale(Vector3(18.27731091f, 15.f, 2.0f));
			//Background->GetComponent<Transform>()->SetScale(Vector3(6.0806f, 5.f, 2.0f));
		}
		{
			//Will의 상점 만들기
			//1.15151515
			GameObject* Background = new GameObject();
			Background->SetName(L"WillsHome");
			AddGameObject(eLayerType::BackObject, Background);
			MeshRenderer* mr = Background->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Build_WiilsHomeMaterial01"));
			Background->GetComponent<Transform>()->SetPosition(Vector3(2.15f, 5.f, 0.0f));
			Background->GetComponent<Transform>()->SetScale(Vector3(3.45454545f, 3.f, 2.0f));

			GameObject* willsFlag = new GameObject();
			willsFlag->SetName(L"WillsFlagBack");
			AddGameObject(eLayerType::BackObject, willsFlag);
			MeshRenderer* mr2 = willsFlag->AddComponent<MeshRenderer>();
			mr2->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr2->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

			std::shared_ptr<Texture> atlas
				= Resources::Load<Texture>(L"WillsFlagSprite", L"..\\Resources\\Texture\\Village\\Build_WillsHome_Flag.png");

			Animator* at = willsFlag->AddComponent<Animator>();
			at->Create(L"Wills_Flag", atlas, Vector2(0.0f, 0.0f), Vector2(93.0f, 190.0f), 53, 190.f);

			willsFlag->GetComponent<Transform>()->SetPosition(Vector3(3.45f, 5.0f, 0.0f));
			willsFlag->GetComponent<Transform>()->SetScale(Vector3(3.45454545f, 3.f, 2.0f));

			at->PlayAnimation(L"Wills_Flag", true);
		}
		{
			//윌의 옆집 (위의 거주지) 만들기
			//0.8903394
			GameObject* Background = new GameObject();
			Background->SetName(L"TopHome1");
			AddGameObject(eLayerType::BackObject, Background);
			MeshRenderer* mr = Background->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"Build_Top1Material01"));
			Background->GetComponent<Transform>()->SetPosition(Vector3(-1.35f, 4.95f, 0.0f));
			Background->GetComponent<Transform>()->SetScale(Vector3(2.2258485f, 2.5f, 2.0f));

			//0.5
			Background = new GameObject();
			Background->SetName(L"TopBuild_Awning01");
			AddGameObject(eLayerType::BackObject, Background);
			MeshRenderer* mr2 = Background->AddComponent<MeshRenderer>();
			mr2->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr2->SetMaterial(Resources::Find<Material>(L"Build_AwningMaterial01"));
			Background->GetComponent<Transform>()->SetPosition(Vector3(-2.89f, 4.58f, 0.0f));
			Background->GetComponent<Transform>()->SetScale(Vector3(0.75f, 1.5f, 2.0f));
		}
		//{
		//	//대장간 생성
		//	//1.32505175
		//	GameObject* Background = new GameObject();
		//	Background->SetName(L"Forge01");
		//	AddGameObject(eLayerType::BackObject, Background);
		//	MeshRenderer* mr = Background->AddComponent<MeshRenderer>();
		//	mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
		//	mr->SetMaterial(Resources::Find<Material>(L"Build_ForgeMaterial01"));
		//	Background->GetComponent<Transform>()->SetPosition(Vector3(8.f, -3.f, 0.0f));
		//	Background->GetComponent<Transform>()->SetScale(Vector3(3.97515525f, 3.5f, 2.0f));
		//}
		{
			//마을 게시판
			//1.5223880
			GameObject* Background = new GameObject();
			Background->SetName(L"BuildBoard");
			AddGameObject(eLayerType::BackObject, Background);
			MeshRenderer* mr = Background->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"BuildBoardMaterial01"));
			Background->GetComponent<Transform>()->SetPosition(Vector3(1.07f, 0.53f, 0.0f));
			Background->GetComponent<Transform>()->SetScale(Vector3(1.2179104f, 0.8f, 1.0f));
		}
#pragma endregion

		{
			//플레이어 생성
			GameObject* Player = new GameObject();
			AddGameObject(eLayerType::Player, Player);
			Player->SetName(L"Player");
			Collider2D* cd = Player->AddComponent<Collider2D>();
			cd->SetSize(Vector2(1.0f, 1.0f));
			MeshRenderer* mr = Player->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

			std::shared_ptr<Texture> atlas
				= Resources::Load<Texture>(L"LinkSprite", L"..\\Resources\\Texture\\Player\\moveState.png");

			Animator* at = Player->AddComponent<Animator>();
			at->Create(L"MOVE_UP", atlas, Vector2(0.0f, 0.0f), Vector2(120.0f, 120.0f), 8);
			at->Create(L"MOVE_DOWN", atlas, Vector2(0.0f, 120.0f), Vector2(120.0f, 120.0f), 8);
			at->Create(L"MOVE_RIGHT", atlas, Vector2(0.0f, 240.0f), Vector2(120.0f, 120.0f), 8);
			at->Create(L"MOVE_LEFT", atlas, Vector2(0.0f, 360.0f), Vector2(120.0f, 120.0f), 8);

			at->Create(L"ROLL_RIGHT", atlas, Vector2(0.0f, 480.0f), Vector2(120.0f, 120.0f), 8);
			at->Create(L"ROLL_LEFT", atlas, Vector2(0.0f, 600.0f), Vector2(120.0f, 120.0f), 8);
			at->Create(L"ROLL_UP", atlas, Vector2(0.0f, 720.0f), Vector2(120.0f, 120.0f), 8);
			at->Create(L"ROLL_DOWN", atlas, Vector2(0.0f, 840.0f), Vector2(120.0f, 120.0f), 8);

			at->Create(L"IDLE_RIGHT", atlas, Vector2(0.0f, 960.0f), Vector2(120.0f, 120.0f), 10);
			at->Create(L"IDLE_LEFT", atlas, Vector2(0.0f, 1080.0f), Vector2(120.0f, 120.0f), 10);
			at->Create(L"IDLE_UP", atlas, Vector2(0.0f, 1200.0f), Vector2(120.0f, 120.0f), 10);
			at->Create(L"IDLE_DOWN", atlas, Vector2(0.0f, 1320.0f), Vector2(120.0f, 120.0f), 10);

			at->PlayAnimation(L"MOVE_RIGHT", true);

			Player->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			Player->GetComponent<Transform>()->SetScale(Vector3(0.8f, 0.8f, 1.0f));

			PlayerFSM* playerFSM = Player->AddComponent<PlayerFSM>();
			playerFSM->AddState(new RollState);
			playerFSM->AddState(new MoveState);
			playerFSM->AddState(new IdleState);

			SetPlayer(Player);
		}

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
	void VillageScene::Update()
	{
		Scene::Update();
	}
	void VillageScene::LateUpdate()
	{
		Scene::LateUpdate();
		if (Input::GetKeyDown(eKeyCode::Z))
		{
			SceneManager::LoadScene(L"ShopScene");
		}
		if (Input::GetKeyDown(eKeyCode::I))
		{
			GameObject* inven = Scene::Find(L"InventoryBase");
			std::wstring asdf = inven->GetName();
			if (inven->GetEnable() == true) inven->SetEnable(false);
			else inven->SetEnable(true);
		}
	}
	void VillageScene::Render()
	{
		Scene::Render();
	}
	void VillageScene::OnEnter()
	{
		if (GetFlag() == true) {
			Initialize();
			SetFlag(false);
		}
	}
	void VillageScene::OnExit()
	{
	}
}