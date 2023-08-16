#include "snDungeonScene01.h"
#include "snGameObject.h"
#include "snMeshRenderer.h"
#include "snMesh.h"
#include "snMaterial.h"
#include "snTransform.h"
#include "snCamera.h"
#include "snCameraScript.h"
#include "snAnimator.h"

#include "snPlayerFSM.h"
#include "snMoveState.h"
#include "snIdleState.h"
#include "snRollState.h"
#include "snSwordState1.h"
#include "snSwordState2.h"
#include "snSwordState3.h"
#include "snBowState.h"

#include "snSceneManager.h"
#include "snCollisionManager.h"

#include "snResources.h"
#include "snInput.h"

#include <random>
#include "snRenderer.h"

#include "MazeMaker.h"
#include "snDungeonCamera.h"
#include "snDungeonDoor.h"
#include "MonFactory.h"
#include "Monster.h"

namespace sn {
	DungeonScene01::DungeonScene01()
	{
	}

	DungeonScene01::~DungeonScene01()
	{
	}

	void DungeonScene01::Initialize()
	{
		CollisionManager::SetLayer(eLayerType::Background, eLayerType::Door, false);

		MazeMaker::GetInst()->Init();
		MazeMaker::GetInst()->BackTracking(2, 2);
		arr = MazeMaker::GetInst()->GetDirArr();
		SetPlayerMapPos(MazeMaker::GetInst()->GetStartPos());

		std::vector<std::wstring> DungeonName;
		DungeonName.push_back(L"DungeonBackgroundMaterial0-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial1-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial2-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial3-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial4-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial5-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial6-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial7-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial8-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial9-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial10-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial11-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial12-0");
		DungeonName.push_back(L"DungeonBackgroundMaterial13-0");

		std::shuffle(DungeonName.begin(), DungeonName.end(), std::default_random_engine());

#pragma region DungeonBackground
		int k = 0;
		for (int i = 0; i < arr.size(); i++) {
			std::vector<RoomInfo> tempVectorRoomInfo;
			roomInfoArr.push_back(tempVectorRoomInfo);

			for (int j = 0; j <arr[i].size(); j++) {
				RoomInfo tempRoomInfo;
				roomInfoArr[i].push_back(tempRoomInfo);
				//던전 배경 로딩 //1.593406
				if (arr[i][j] != 0) {
					GameObject* Background = new GameObject();
					Background->SetName(DungeonName[k]);
					AddGameObject(eLayerType::Background, Background);
					MeshRenderer* mr = Background->AddComponent<MeshRenderer>();
					mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
					if (GetPlayerMapPos().second == j && GetPlayerMapPos().first == i) {
						mr->SetMaterial(Resources::Find<Material>(L"DungeonBackgroundMaterial0"));
						roomInfoArr[i][j].clear = true;
					}
					else {
						mr->SetMaterial(Resources::Find<Material>(DungeonName[k]));
						k++;
						roomInfoArr[i][j].clear = false;
					}
					Background->GetComponent<Transform>()->SetPosition(Vector3((float)j * 9.72f, -((float)i * 5.45f), 0.0f));
					//Background->GetComponent<Transform>()->SetScale(Vector3(6.7f, 4.0f, 2.0f));
					Background->GetComponent<Transform>()->SetScale(Vector3(9.777778f, 5.5f, 0.0f));
					Collider2D* cdUP = Background->AddComponent<Collider2D>();
					cdUP->SetSize(Vector2(cdUP->GetSize().x-0.1f, 0.1f));
					cdUP->SetCenter(Vector2(0.0f, -2.3f));
					Collider2D* cdDOWN = Background->AddComponent<Collider2D>();
					cdDOWN->SetSize(Vector2(cdUP->GetSize().x - 0.1f, 0.1f));
					cdDOWN->SetCenter(Vector2(0.0f, +2.3f));
					Collider2D* cdLEFT = Background->AddComponent<Collider2D>();
					cdLEFT->SetSize(Vector2(0.1f, cdUP->GetSize().y - 1.f));
					cdLEFT->SetCenter(Vector2(-4.18f, 0.0f));
					Collider2D* cdRIGHT = Background->AddComponent<Collider2D>();
					cdRIGHT->SetSize(Vector2(0.1f, cdUP->GetSize().y - 1.f));
					cdRIGHT->SetCenter(Vector2(4.15f, 0.0f));

					//문의 위치에 맞게 문 생성
					int doorCount = arr[i][j];
					if (doorCount / DIRRIGHT == 1) {
						{
							GameObject* door = new GameObject();
							AddGameObject(eLayerType::Door, door);
							MeshRenderer* mr = door->AddComponent<MeshRenderer>();
							mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
							mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

							std::shared_ptr<Texture> atlas
								= Resources::Load<Texture>(L"Golem_Basic_Door_Left", L"..\\Resources\\Texture\\Dungeon\\Door\\Golem_Basic_Door_Left.png");
							Animator* at = door->AddComponent<Animator>();
							at->Create(L"CLOSE_DOOR", atlas, Vector2(0.0f, 0.0f), Vector2(48.0f, 78.0f), 7);
							at->Create(L"OPEN_DOOR", atlas, Vector2(288.0f, 0.0f), Vector2(48.0f, 78.0f), 5);
							at->PlayAnimation(L"OPEN_DOOR", false);
							door->GetComponent<Transform>()->SetPosition(Vector3(((float)j * 9.72f) + 3.95f, (-((float)i * 5.45f)), 0.0f));
							door->GetComponent<Transform>()->SetScale(Vector3(1.7f, 1.7f, 0.0f));

							Collider2D* cd = door->AddComponent<Collider2D>();
							cd->SetSize(Vector2(0.4f, 0.3f));

							std::pair<int, int> doorPos = std::make_pair<int, int>((int)j, (int)i);
							door->AddComponent<DungeonDoor>(DoorType::RIGHT, doorPos);
						}

						doorCount -= DIRRIGHT;
						}if (doorCount / DIRLEFT == 1) {
							{
								GameObject* door = new GameObject();
								AddGameObject(eLayerType::Door, door);
								MeshRenderer* mr = door->AddComponent<MeshRenderer>();
								mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
								mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

								std::shared_ptr<Texture> atlas
									= Resources::Load<Texture>(L"Golem_Basic_Door_Right", L"..\\Resources\\Texture\\Dungeon\\Door\\Golem_Basic_Door_Right.png");
								Animator* at = door->AddComponent<Animator>();
								at->Create(L"CLOSE_DOOR", atlas, Vector2(0.0f, 0.0f), Vector2(48.0f, 78.0f), 7);
								at->Create(L"OPEN_DOOR", atlas, Vector2(288.0f, 0.0f), Vector2(48.0f, 78.0f), 5);
								at->PlayAnimation(L"OPEN_DOOR", false);
								door->GetComponent<Transform>()->SetPosition(Vector3(((float)j * 9.72f) - 3.95f, (-((float)i * 5.45f)), 0.0f));

								door->GetComponent<Transform>()->SetScale(Vector3(1.7f, 1.7f, 0.0f));

								Collider2D* cd = door->AddComponent<Collider2D>();
								cd->SetSize(Vector2(0.4f, 0.3f));

								std::pair<int, int> doorPos = std::make_pair<int, int>((int)j, (int)i);
								door->AddComponent<DungeonDoor>(DoorType::LEFT, doorPos);
							}

							doorCount -= DIRLEFT;
						}
						if (doorCount / DIRDOWN == 1) {
							{
								GameObject* door = new GameObject();
								AddGameObject(eLayerType::Door, door);
								MeshRenderer* mr = door->AddComponent<MeshRenderer>();
								mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
								mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

								std::shared_ptr<Texture> atlas
									= Resources::Load<Texture>(L"Golem_Basic_Door_Up", L"..\\Resources\\Texture\\Dungeon\\Door\\Golem_Basic_Door_Up.png");
								Animator* at = door->AddComponent<Animator>();
								at->Create(L"CLOSE_DOOR", atlas, Vector2(0.0f, 0.0f), Vector2(78.0f, 48.0f), 7);
								at->Create(L"OPEN_DOOR", atlas, Vector2(468.0f, 0.0f), Vector2(78.0f, 48.0f), 5);
								at->PlayAnimation(L"OPEN_DOOR", false);
								door->GetComponent<Transform>()->SetPosition(Vector3(((float)j * 9.72f), (-((float)i * 5.45f)) - 2.3f, 0.0f));

								door->GetComponent<Transform>()->SetScale(Vector3(1.7f, 1.7f, 0.0f));

								Collider2D* cd = door->AddComponent<Collider2D>();
								cd->SetSize(Vector2(0.3f, 0.4f));

								std::pair<int, int> doorPos = std::make_pair<int, int>((int)j, (int)i);
								door->AddComponent<DungeonDoor>(DoorType::DOWN, doorPos);
							}

							doorCount -= DIRDOWN;
						}
						if (doorCount / DIRUP == 1) {
							{
								GameObject* door = new GameObject();
								AddGameObject(eLayerType::Door, door);
								MeshRenderer* mr = door->AddComponent<MeshRenderer>();
								mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
								mr->SetMaterial(Resources::Find<Material>(L"SpriteAnimaionMaterial"));

								std::shared_ptr<Texture> atlas
									= Resources::Load<Texture>(L"Golem_Basic_Door_Down", L"..\\Resources\\Texture\\Dungeon\\Door\\Golem_Basic_Door_Down.png");
								Animator* at = door->AddComponent<Animator>();
								at->Create(L"CLOSE_DOOR", atlas, Vector2(0.0f, 0.0f), Vector2(78.0f, 48.0f), 7);
								at->Create(L"OPEN_DOOR", atlas, Vector2(468.0f, 0.0f), Vector2(78.0f, 48.0f), 5);
								at->PlayAnimation(L"OPEN_DOOR", false);
								door->GetComponent<Transform>()->SetPosition(Vector3(((float)j * 9.72f), (-((float)i * 5.45f)) + 2.3f, 0.0f));

								door->GetComponent<Transform>()->SetScale(Vector3(1.7f, 1.7f, 0.0f));
								Collider2D* cd = door->AddComponent<Collider2D>();
								cd->SetSize(Vector2(0.3f, 0.4f));

								std::pair<int, int> doorPos = std::make_pair<int, int>((int)j, (int)i);
								door->AddComponent<DungeonDoor>(DoorType::UP, doorPos);
							}

							doorCount -= DIRUP;
						}
				}
			}
		}
#pragma endregion
#pragma region MonsterSpawn
		{
			for (int i = 0; i < arr.size(); i++) {
				for (int j = 0; j < arr[i].size(); j++) {
					if (GetPlayerMapPos().second == j && GetPlayerMapPos().first == i) {
						continue;
					}
					if (arr[i][j] == 0) {
						continue;
					}
					Monster* pMon = MonFactory::CreateMonster(MonType::GOLEM, Vector2(j*9.72f, i*-5.45f));
					pMon->SetMonsterMapPos(i, j);
					AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
					roomInfoArr[i][j].monsterNum++;
				}
			}
		}

#pragma endregion
#pragma region Player
		{
			//플레이어 생성
			GameObject* Player = new GameObject();
			AddGameObject(eLayerType::Player, Player);
			Player->SetName(L"Player");
			Collider2D* cd = Player->AddComponent<Collider2D>();
			cd->SetSize(Vector2(0.3f, 0.3f));
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

			//Light* lightComp = Player->AddComponent<Light>();
			//lightComp->SetType(eLightType::Point);
			//lightComp->SetColor(Vector4(0.8f, 0.8f, 0.8f, 1.0f));
			//lightComp->SetRadius(2.0f);

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

			Player->GetComponent<Transform>()->SetPosition(Vector3(GetPlayerMapPos().second * 9.72f, GetPlayerMapPos().first * -5.45f, 0.0f));
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
#pragma endregion
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
			camera->AddComponent<DungeonCamera>();
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
			lightComp->SetColor(Vector4(0.8f, 0.8f, 0.8f, 1.0f));
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
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::SetChangeScene(L"DungeonScene02");
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
		renderer::mainCamera = GetMainCamera();
	}

	void DungeonScene01::OnExit()
	{
		DestroyAll();
	}

}
