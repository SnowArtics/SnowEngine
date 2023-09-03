#include "MiniSlime.h"
#include "snSceneManager.h"
#include "DungeonMapManager.h"
#include "snPlayer.h"
#include "PlayerHP.h"

MiniSlime::MiniSlime()
{
}

MiniSlime::~MiniSlime()
{
}

void MiniSlime::Initialize()
{
	Monster::Initialize();
}

void MiniSlime::Update()
{
	Monster::Update();
}

void MiniSlime::LateUpdate()
{
	Monster::LateUpdate();
}

void MiniSlime::Render()
{
	Monster::Render();
}

void MiniSlime::OnCollisionEnter(sn::Collider2D* other, sn::Collider2D* me)
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

void MiniSlime::OnCollisionStay(sn::Collider2D* other, sn::Collider2D* me)
{
}

void MiniSlime::OnCollisionExit(sn::Collider2D* other, sn::Collider2D* me)
{
}
