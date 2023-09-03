#include "DungeonMapManager.h"
#include <snCollider2D.h>
#include "snMeshRenderer.h"
#include "MazeMaker.h"
#include <snScene.h>
#include <snSceneManager.h>
#include <snResources.h>
#include <snAnimator.h>
#include "snDungeonDoor.h"
#include "MonFactory.h"
#include "snGameObject.h"
#include "Monster.h"

using namespace sn;

DungeonMapManager::DungeonMapManager()
	:arr{ {} }
{
}

DungeonMapManager::~DungeonMapManager()
{
}

void DungeonMapManager::Init()
{
	arr.clear();
	arr.shrink_to_fit();
	playerMapPos = std::make_pair(-1, -1);
	roomInfoArr.clear();
	roomInfoArr.shrink_to_fit();
	dungeonClear = false;
}

void DungeonMapManager::MakeDungeonBackground(std::vector<std::wstring> _DungeonName)
{
	int k = 0;
	for (int i = 0; i < arr.size(); i++) {
		std::vector<RoomInfo> tempVectorRoomInfo;
		roomInfoArr.push_back(tempVectorRoomInfo);

		for (int j = 0; j < arr[i].size(); j++) {
			RoomInfo tempRoomInfo;
			tempRoomInfo.monsterNum = 0;
			tempRoomInfo.clear = false;
			roomInfoArr[i].push_back(tempRoomInfo);
			//던전 배경 로딩 //1.593406
			if (arr[i][j] != 0) {
				std::wstring subStr = L"";

				GameObject* Background = new GameObject();
				SceneManager::GetActiveScene()->AddGameObject(eLayerType::Background, Background);
				MeshRenderer* mr = Background->AddComponent<MeshRenderer>();
				mr->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
				if (playerMapPos.second == j && playerMapPos.first == i) {
					std::wstring originStr = L"DungeonBackgroundMaterial0";
					Background->SetName(originStr);
					mr->SetMaterial(Resources::Find<Material>(originStr));
					roomInfoArr[i][j].clear = true;
					DungeonMapManager::GetInst()->MakeCliffCollider(-1, Background);
					roomInfoArr[i][j].roomNum = -1;
				}
				else if (arr[i][j] >= 128) {
					std::wstring originStr = L"DungeonBackgroundMaterial0";
					Background->SetName(originStr);
					mr->SetMaterial(Resources::Find<Material>(originStr));
					roomInfoArr[i][j].clear = false;
					DungeonMapManager::GetInst()->MakeCliffCollider(-1, Background);
					//MonsterSpawn(128, i, j);
					roomInfoArr[i][j].roomNum = 128;
				}
				else {
					std::wstring originStr = _DungeonName[k];
					mr->SetMaterial(Resources::Find<Material>(originStr));
					Background->SetName(originStr);
					k++;
					roomInfoArr[i][j].clear = false;
					subStr = originStr.substr(25, 2);
					int strToNum = std::stoll(subStr);
					DungeonMapManager::GetInst()->MakeCliffCollider(strToNum, Background);
					//MonsterSpawn(strToNum, i, j);
					roomInfoArr[i][j].roomNum = strToNum;
				}

				Background->GetComponent<Transform>()->SetPosition(Vector3((float)j * 9.72f, -((float)i * 5.45f), 0.0f));
				//Background->GetComponent<Transform>()->SetScale(Vector3(6.7f, 4.0f, 2.0f));
				Background->GetComponent<Transform>()->SetScale(Vector3(9.777778f, 5.5f, 0.0f));
			}
		}
	}
}

void DungeonMapManager::MakeCliffCollider(int _num, GameObject* _background)
{	
	sn::Collider2D* cdUP = _background->AddComponent<sn::Collider2D>();
	cdUP->SetSize(Vector2(cdUP->GetSize().x - 0.1f, 0.1f));
	cdUP->SetCenter(Vector2(0.0f, -2.3f));
	sn::Collider2D* cdDOWN = _background->AddComponent<sn::Collider2D>();
	cdDOWN->SetSize(Vector2(cdUP->GetSize().x - 0.1f, 0.1f));
	cdDOWN->SetCenter(Vector2(0.0f, +2.3f));
	sn::Collider2D* cdLEFT = _background->AddComponent<sn::Collider2D>();
	cdLEFT->SetSize(Vector2(0.1f, cdUP->GetSize().y - 1.f));
	cdLEFT->SetCenter(Vector2(-4.18f, 0.0f));
	sn::Collider2D* cdRIGHT = _background->AddComponent<sn::Collider2D>();
	cdRIGHT->SetSize(Vector2(0.1f, cdUP->GetSize().y - 1.f));
	cdRIGHT->SetCenter(Vector2(4.15f, 0.0f));

	switch (_num)
	{
	case 0:
	{
		sn::Collider2D* cd01 = _background->AddComponent<sn::Collider2D>();
		cd01->SetName(L"CliffCollider");
		cd01->SetSize(Vector2(0.295, 0.06f));
		cd01->SetCenter(Vector2(-2.25f, +1.85f));

		//sn::Collider2D* cd02 = _background->AddComponent<sn::Collider2D>();
		//cd02->SetName(L"CliffCollider");
		//cd02->SetSize(Vector2(0.295, 0.06f));
		//cd02->SetCenter(Vector2(-2.25f, +1.85f));
	}
		break;
	case 1:
	{

	}
	break;
	case 2:
	{

	}
	break;
	case 3:
	{

	}
	break;
	case 4:
	{

	}
	break;
	case 5:
	{

	}
	break;
	case 6:
	{

	}
	break;
	case 7:
	{

	}
	break;

	default:
		break;
	}
}

void DungeonMapManager::MakeDoor()
{
	//문의 위치에 맞게 문 생성
	for (int i = 0; i < arr.size(); i++) {
		for (int j = 0; j < arr[i].size(); j++) {
			int doorCount = arr[i][j];
			if (doorCount / DIRRIGHT == 1) {
				{
					GameObject* door = new GameObject();
					SceneManager::GetActiveScene()->AddGameObject(eLayerType::Door, door);
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

					sn::Collider2D* cd = door->AddComponent<sn::Collider2D>();
					cd->SetSize(Vector2(0.4f, 0.3f));

					std::pair<int, int> doorPos = std::make_pair<int, int>((int)j, (int)i);
					door->AddComponent<DungeonDoor>(DoorType::RIGHT, doorPos);
				}

				doorCount -= DIRRIGHT;
			}if (doorCount / DIRLEFT == 1) {
				{
					GameObject* door = new GameObject();
					SceneManager::GetActiveScene()->AddGameObject(eLayerType::Door, door);
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

					sn::Collider2D* cd = door->AddComponent<sn::Collider2D>();
					cd->SetSize(Vector2(0.4f, 0.3f));

					std::pair<int, int> doorPos = std::make_pair<int, int>((int)j, (int)i);
					door->AddComponent<DungeonDoor>(DoorType::LEFT, doorPos);
				}

				doorCount -= DIRLEFT;
			}
			if (doorCount / DIRDOWN == 1) {
				{
					GameObject* door = new GameObject();
					SceneManager::GetActiveScene()->AddGameObject(eLayerType::Door, door);
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

					sn::Collider2D* cd = door->AddComponent<sn::Collider2D>();
					cd->SetSize(Vector2(0.3f, 0.4f));

					std::pair<int, int> doorPos = std::make_pair<int, int>((int)j, (int)i);
					door->AddComponent<DungeonDoor>(DoorType::DOWN, doorPos);
				}

				doorCount -= DIRDOWN;
			}
			if (doorCount / DIRUP == 1) {
				{
					GameObject* door = new GameObject();
					SceneManager::GetActiveScene()->AddGameObject(eLayerType::Door, door);
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
					sn::Collider2D* cd = door->AddComponent<sn::Collider2D>();
					cd->SetSize(Vector2(0.3f, 0.4f));

					std::pair<int, int> doorPos = std::make_pair<int, int>((int)j, (int)i);
					door->AddComponent<DungeonDoor>(DoorType::UP, doorPos);
				}

				doorCount -= DIRUP;
			}
		}
	}
}

void DungeonMapManager::MonsterSpawn()
{
	for (int i = 0; i < arr.size(); i++) {
		for (int j = 0; j < arr[i].size(); j++) {
			if (DungeonMapManager::GetInst()->GetPlayerMapPos().second == j && DungeonMapManager::GetInst()->GetPlayerMapPos().first == i) {
				continue;
			}
			if (arr[i][j] == 0) {
				continue;
			}
			Monster* pMon = MonFactory::CreateMonster(MonType::GOLEMMINIBOSS, Vector2(j * 9.72f, i * -5.45f));
			
			pMon->SetMonsterMapPos(i, j);
			SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
			DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
			//DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].clear = false;
		}
	}
}

void DungeonMapManager::MonsterSpawn(int _num, int i, int j)
{
	DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum = 0;

	//if (DungeonMapManager::GetInst()->GetPlayerMapPos().second == j && DungeonMapManager::GetInst()->GetPlayerMapPos().first == i) {
	//	return;
	//}
	if (arr[i][j] == 0) {
		return;
	}

	switch (_num)
	{
	case 0:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::SLIME, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 2.0f, i * -5.45f+1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 2.0f, i * -5.45f - 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.0f, i * -5.45f + 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.0f, i * -5.45f - 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
		break;
	case 1:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::SLIME, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 2.0f, i * -5.45f + 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 2.0f, i * -5.45f - 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.0f, i * -5.45f + 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.0f, i * -5.45f - 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 2:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::GOLEM, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRETBROKEN, Vector2(j * 9.72f - 2.f, i * -5.45f + 1.f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRETBROKEN, Vector2(j * 9.72f - 3.5f, i * -5.45f - 1.f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 3:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::GOLEM, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRETBROKEN, Vector2(j * 9.72f + 2.f, i * -5.45f - 1.f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRETBROKEN, Vector2(j * 9.72f + 3.5f, i * -5.45f + 1.f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 4:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::GOLEM, Vector2(j * 9.72f, i * -5.45f + 1.f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEM, Vector2(j * 9.72f, i * -5.45f - 1.f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRETBROKEN, Vector2(j * 9.72f + 2.5f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 5:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::SLIME, Vector2(j * 9.72f-2.f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::SLIMEHERMIT, Vector2(j * 9.72f+2.f, i * -5.45f - 1.f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.f, i * -5.45f + 1.f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.f, i * -5.45f - 1.f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 6:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::GOLEM, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRET, Vector2(j * 9.72f + 1.5f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRET, Vector2(j * 9.72f - 1.5f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 7:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::GOLEM, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 1.5f, i * -5.45f - 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.f, i * -5.45f - 1.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.f, i * -5.45f - 0.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 1.5f, i * -5.45f + 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 2.f, i * -5.45f + 1.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 2.f, i * -5.45f + 0.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 8:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::FYLINGREPAIRGOLEM, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRETBROKEN, Vector2(j * 9.72f + 1.5f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRETBROKEN, Vector2(j * 9.72f - 1.5f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 9:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::GOLEM, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRETBROKEN, Vector2(j * 9.72f + 1.5f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRETBROKEN, Vector2(j * 9.72f - 1.5f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 10:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::SLIMEHERMIT, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 1.5f, i * -5.45f - 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.f, i * -5.45f - 1.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.f, i * -5.45f - 0.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 1.5f, i * -5.45f + 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 2.f, i * -5.45f + 1.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 2.f, i * -5.45f + 0.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 11:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::GOLEM, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 1.5f, i * -5.45f - 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.f, i * -5.45f - 1.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f + 2.f, i * -5.45f - 0.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 1.5f, i * -5.45f + 1.0f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 2.f, i * -5.45f + 1.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::BABYSLIME, Vector2(j * 9.72f - 2.f, i * -5.45f + 0.5f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 12:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::GOLEM, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::SLIMEHERMIT, Vector2(j * 9.72f + 1.5f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::SLIME, Vector2(j * 9.72f - 1.5f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 13:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::GOLEMTURRET, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRETBROKEN, Vector2(j * 9.72f + 1.5f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;

		pMon = MonFactory::CreateMonster(MonType::GOLEMTURRETBROKEN, Vector2(j * 9.72f - 1.5f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	break;
	case 128:
	{
		Monster* pMon = MonFactory::CreateMonster(MonType::GOLEMMINIBOSS, Vector2(j * 9.72f, i * -5.45f));

		pMon->SetMonsterMapPos(i, j);
		SceneManager::GetActiveScene()->AddGameObject(eLayerType::Monster, static_cast<GameObject*>(pMon));
		DungeonMapManager::GetInst()->GetRoomInfoArr()[i][j].monsterNum++;
	}
	default:
		break;
	}
}
