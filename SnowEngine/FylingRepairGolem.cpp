#include "FylingRepairGolem.h"
#include "AI.h"
#include "snAnimator.h"
#include "snTransform.h"
#include "snSceneManager.h"
#include "DungeonMapManager.h"
#include "MonsterHPBar.h"
#include "snPlayer.h"
#include "PlayerHP.h"

using namespace sn;

FylingRepairGolem::FylingRepairGolem()
{
}

FylingRepairGolem::~FylingRepairGolem()
{
}

void FylingRepairGolem::Initialize()
{
	Monster::Initialize();
}

void FylingRepairGolem::Update()
{
	AI* ai = GetComponent<AI>();
	MonDir monDir = ai->GetCurDir();
	MON_STATE monState = ai->GetCurStateName();

	sn::Animator* animator = GetComponent<sn::Animator>();

	sn::Transform* monTr = this->GetComponent <sn::Transform>();
	sn::Collider2D* collider = this->GetComponent<sn::Collider2D>();

	if (monDir != ai->GetPrevDir() || monState != ai->GetPrevStateName()) {
		switch (monState)
		{
		case MON_STATE::IDLE:
		{
			animator->PlayAnimation(L"FYLING_REPAIR_GOLEM_IDLE", true);
		}
			break;
		case MON_STATE::WALK:
			break;
		case MON_STATE::TRACE:
		{
			animator->PlayAnimation(L"FYLING_REPAIR_GOLEM_IDLE", true);
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

	}
	Monster::Update();
}

void FylingRepairGolem::LateUpdate()
{
	Monster::LateUpdate();
}

void FylingRepairGolem::Render()
{
	Monster::Render();
}

void FylingRepairGolem::OnCollisionEnter(sn::Collider2D* other, sn::Collider2D* me)
{
	Monster::OnCollisionEnter(other, me);
	if (other->GetName() == L"SecondCollider") {
		tMonInfo monInfo = GetMonsterInfo();
		monInfo.fHP -= 30.f;
		this->GetComponent<MonsterHPBar>()->PlayDamage(30.f);
		this->GetComponent<MonsterHPBar>()->SetEnable(true);
		SetMonsterInfo(monInfo);


		if (monInfo.fHP <= 0.f) {
			std::vector<std::vector<RoomInfo>>& vecRoomInfo = DungeonMapManager::GetInst()->GetRoomInfoArr();
			vecRoomInfo[monsterMapPos.first][monsterMapPos.second].monsterNum -= 1;

			if (vecRoomInfo[monsterMapPos.first][monsterMapPos.second].monsterNum <= 0) {
				vecRoomInfo[monsterMapPos.first][monsterMapPos.second].clear = true;
				DungeonMapManager::GetInst()->SetRoomInfoArr(vecRoomInfo);
				DungeonMapManager::GetInst()->SetDungeonClear(true);
			}
		}
	}
	if (other->GetName() == L"FisrtCollider") {
		snPlayer* player = static_cast<snPlayer*>(other->GetOwner());
		float playerHP = player->GetHP();
		playerHP -= 30.f;
		player->SetHP(playerHP);
		PlayerHP* playerHPComponent = player->GetComponent<PlayerHP>();
		playerHPComponent->PlayDamage(30.f);
	}
}

void FylingRepairGolem::OnCollisionStay(sn::Collider2D* other, sn::Collider2D* me)
{
}

void FylingRepairGolem::OnCollisionExit(sn::Collider2D* other, sn::Collider2D* me)
{
}
