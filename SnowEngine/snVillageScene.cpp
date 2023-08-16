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
#include "snSwordState1.h"
#include "snSwordState2.h"
#include "snSwordState3.h"
#include "snBowState.h"

#include "MazeMaker.h"

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
			Collider2D* cd = Background->AddComponent<Collider2D>();
			cd->SetName(L"BuildBoardCollder");
			cd->SetSize(Vector2(1.0f, 1.0f));
			cd->SetEnable(true);
		}
#pragma endregion

		{
			//플레이어 생성
			GameObject* Player = new GameObject();
			AddGameObject(eLayerType::Player, Player);
			Player->SetName(L"Player");
			Collider2D* cd = Player->AddComponent<Collider2D>();
			cd->SetSize(Vector2(0.5f, 0.5f));
			cd->SetName(L"FisrtCollider");
			cd->SetEnable(true);
			Collider2D* cd2 = Player->AddComponent<Collider2D>();
			cd2->SetName(L"SecondCollider");
			//cd2->SetSize(Vector2(0.5f, 0.8f));
			//cd2->SetCenter(Vector2(0.2f, -0.1f));
			cd2->SetEnable(false);
			MeshRenderer* mr = Player->AddComponent<MeshRenderer>();
			mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

			Light* lightComp = Player->AddComponent<Light>();
			lightComp->SetType(eLightType::Point);
			lightComp->SetColor(Vector4(0.8f, 0.8f, 0.8f, 1.0f));
			lightComp->SetRadius(2.0f);

			std::shared_ptr<Texture> atlas
				= Resources::Load<Texture>(L"WillMoveSprite", L"..\\Resources\\Texture\\Player\\moveState.png");

			Animator* at = Player->AddComponent<Animator>();
			at->Create(L"MOVE_UP", atlas, Vector2(0.0f, 0.0f), Vector2(120.0f, 120.0f), 8);
			at->Create(L"MOVE_DOWN", atlas, Vector2(0.0f, 120.0f), Vector2(120.0f, 120.0f), 8);
			at->Create(L"MOVE_RIGHT", atlas, Vector2(0.0f, 240.0f), Vector2(120.0f, 120.0f), 8);
			at->Create(L"MOVE_LEFT", atlas, Vector2(0.0f, 360.0f), Vector2(120.0f, 120.0f), 8);

			at->Create(L"ROLL_RIGHT", atlas, Vector2(0.0f, 480.0f), Vector2(120.0f, 120.0f), 8, 110.f, 0.06f);
			at->Create(L"ROLL_LEFT", atlas, Vector2(0.0f, 600.0f), Vector2(120.0f, 120.0f), 8, 110.f, 0.06f);
			at->Create(L"ROLL_UP", atlas, Vector2(0.0f, 720.0f), Vector2(120.0f, 120.0f), 8, 110.f, 0.06f);
			at->Create(L"ROLL_DOWN", atlas, Vector2(0.0f, 840.0f), Vector2(120.0f, 120.0f), 8, 110.f, 0.06f);

			at->Create(L"IDLE_RIGHT", atlas, Vector2(0.0f, 960.0f), Vector2(120.0f, 120.0f), 10);
			at->Create(L"IDLE_LEFT", atlas, Vector2(0.0f, 1080.0f), Vector2(120.0f, 120.0f), 10);
			at->Create(L"IDLE_UP", atlas, Vector2(0.0f, 1200.0f), Vector2(120.0f, 120.0f), 10);
			at->Create(L"IDLE_DOWN", atlas, Vector2(0.0f, 1320.0f), Vector2(120.0f, 120.0f), 10);

			atlas = Resources::Load<Texture>(L"WillSwordAttackSprite", L"..\\Resources\\Texture\\Player\\swordState.png");

			at->Create(L"SWORD_UP1", atlas, Vector2(120.0f, 0.0f), Vector2(120.f, 120.f), 5);
			at->Create(L"SWORD_DOWN1", atlas, Vector2(120.0f, 120.0f), Vector2(120.f, 120.f), 5);
			at->Create(L"SWORD_RIGHT1", atlas, Vector2(120.0f, 240.0f), Vector2(120.f, 120.f), 5);
			at->Create(L"SWORD_LEFT1", atlas, Vector2(120.0f, 360.0f), Vector2(120.f, 120.f), 5);

			at->Create(L"SWORD_UP2", atlas, Vector2(720.f, 0.0f), Vector2(120.f, 120.f), 5);
			at->Create(L"SWORD_DOWN2", atlas, Vector2(720.f, 120.0f), Vector2(120.f, 120.f), 5);
			at->Create(L"SWORD_RIGHT2", atlas, Vector2(720.f, 240.0f), Vector2(120.f, 120.f), 5);
			at->Create(L"SWORD_LEFT2", atlas, Vector2(720.f, 360.0f), Vector2(120.f, 120.f), 5);

			atlas = Resources::Load<Texture>(L"WillBowAttackSprite", L"..\\Resources\\Texture\\Player\\bowState.png");

			at->Create(L"BOW_UP", atlas, Vector2(120.0f, 0.0f), Vector2(120.f, 120.f), 7);
			at->Create(L"BOW_DOWN", atlas, Vector2(120.0f, 120.0f), Vector2(120.f, 120.f), 7);
			at->Create(L"BOW_RIGHT", atlas, Vector2(120.0f, 240.0f), Vector2(120.f, 120.f), 7);
			at->Create(L"BOW_LEFT", atlas, Vector2(120.0f, 360.0f), Vector2(120.f, 120.f), 7);

			atlas = Resources::Load<Texture>(L"WILLCHARGE", L"..\\Resources\\bowChargeState.png");

			at->Create(L"BOW_CHARGE", atlas, Vector2(0.0f, 0.0f), Vector2(42.f, 80.f), 26);

			at->PlayAnimation(L"BOW_DOWN", true);

			Player->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			Player->GetComponent<Transform>()->SetScale(Vector3(1.0f, 1.0f, 1.0f));

			PlayerFSM* playerFSM = Player->AddComponent<PlayerFSM>();
			playerFSM->AddState(new RollState);
			playerFSM->AddState(new MoveState);
			playerFSM->AddState(new SwordState1);
			playerFSM->AddState(new SwordState2);
			playerFSM->AddState(new SwordState3);
			playerFSM->AddState(new BowState);
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
			cameraComp->EnableLayerMasks();
			cameraComp->TurnLayerMask(eLayerType::UI, false);
			camera->AddComponent<CameraScript>();
			renderer::cameras.push_back(cameraComp);
			renderer::mainCamera = cameraComp;
			SetMainCamera(cameraComp);
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

		// Light
		{
			GameObject* light = new GameObject();
			light->SetName(L"DirectionalLight01");
			AddGameObject(eLayerType::Light, light);
			Light* lightComp = light->AddComponent<Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetColor(Vector4(0.4f, 0.4f, 0.4f, 1.0f));
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
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::SetChangeScene(L"ShopScene");
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
		Initialize();
		renderer::mainCamera = GetMainCamera();
	}
	void VillageScene::OnExit()
	{
		DestroyAll();
	}
}