#include "Golem.h"

#include "AI.h"
#include "snAnimator.h"
#include "snSceneManager.h"
#include "snDungeon.h"

Golem::Golem()
{
}

Golem::~Golem()
{
}

void Golem::Initialize()
{
	Monster::Initialize();
}

void Golem::Update()
{
	Monster::Update();
}

void Golem::LateUpdate()
{
	AI* ai = GetComponent<AI>();
	MonDir monDir = ai->GetDir();
	MON_STATE monState = ai->GetStateName();

	Animator* animator = GetComponent<Animator>();
	
	switch (monState)
	{
	case MON_STATE::IDLE:
		break;
	case MON_STATE::WALK:
		break;
	case MON_STATE::TRACE:
	{
		switch (monDir)
		{
		case MonDir::UP:
			animator->PlayAnimation(L"GOLEM_MOVE_UP", true);
			break;
		case MonDir::DOWN:
			animator->PlayAnimation(L"GOLEM_MOVE_DOWN", true);
			break;
		case MonDir::RIGHT:
			animator->PlayAnimation(L"GOLEM_MOVE_RIGHT", true);
			break;
		case MonDir::LEFT:
			animator->PlayAnimation(L"GOLEM_MOVE_LEFT", true);
			break;
		default:
			break;
		}
	}
		break;
	case MON_STATE::ATT:
		break;
	case MON_STATE::TARGET_ATT:
		break;
	case MON_STATE::DEAD:
		break;
	case MON_STATE::END:
		break;
	default:
		break;
	}

	Monster::LateUpdate();
}

void Golem::Render()
{
	Monster::Render();
}

void Golem::OnCollisionEnter(sn::Collider2D* other)
{
	if (other->GetName() == L"SecondCollider") {
		tMonInfo monInfo = GetMonsterInfo();
		monInfo.fHP -= 30.f;
		SetMonsterInfo(monInfo);

		Dungeon* dungeonScene = static_cast<Dungeon*>(SceneManager::GetActiveScene());
		std::vector<std::vector<RoomInfo>> vecRoomInfo = dungeonScene->GetRoomInfoArr();

		if (monInfo.fHP <= 0.f) {
			vecRoomInfo[monsterMapPos.first][monsterMapPos.second].monsterNum -= 1;

			if (vecRoomInfo[monsterMapPos.first][monsterMapPos.second].monsterNum <= 0) {
				vecRoomInfo[monsterMapPos.first][monsterMapPos.second].clear = true;
				dungeonScene->SetRoomInfoArr(vecRoomInfo);
				dungeonScene->SetDungeonClear(true);
			}
		}
	}
}

void Golem::OnCollisionStay(sn::Collider2D* other)
{
}

void Golem::OnCollisionExit(sn::Collider2D* other)
{
}
