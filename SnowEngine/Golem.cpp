#include "Golem.h"

#include "AI.h"
#include "snAnimator.h"
#include "snSceneManager.h"
#include "DungeonMapManager.h"
#include "MonsterHPBar.h"
#include "snCollider2D.h"
#include "snTime.h"
#include "snPlayer.h"
#include "PlayerHP.h"

Golem::Golem()
	:attTime(0.f)
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
	AI* ai = GetComponent<AI>();
	MonDir monDir = ai->GetCurDir();
	MON_STATE monState = ai->GetCurStateName();

	Transform* monTr = this->GetComponent <Transform>();
	sn::Collider2D* secondCollider = this->GetComponents<sn::Collider2D>()[1];
	sn::Collider2D* collider = this->GetComponent<sn::Collider2D>();

	Animator* animator = GetComponent<Animator>();

	if (attTime >= 0.1f)
		attTime += Time::DeltaTime();

	if (attTime > 0.8f) {
		secondCollider->SetEnable(true);
	}

	if (monDir != ai->GetPrevDir() || monState != ai->GetPrevStateName()) {
		switch (monState)
		{
		case MON_STATE::IDLE:
			break;
		case MON_STATE::WALK:
			break;
		case MON_STATE::TRACE:
		{
			attTime = 0.f;
			monTr->SetScale(1.0f, 1.0f, 1.0f);
			collider->SetSize(Vector2(0.4f, 0.4f));
			secondCollider->SetSize(Vector2(0.4f, 0.4f));
			secondCollider->SetEnable(false);
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
		{
			if (attTime == 0.f) {
				attTime = 0.1f;
			}
			monTr->SetScale(1.5f, 1.5f, 1.0f);
			collider->SetSize(Vector2(0.275f, 0.275f));
			secondCollider->SetSize(Vector2(0.275f, 0.275f));
			switch (monDir)
			{
			case MonDir::UP:
				secondCollider->SetCenter(Vector2(0.0f, 0.3f));
				animator->PlayAnimation(L"GOLEM_ATTACK_UP", true);
				break;
			case MonDir::DOWN:
				secondCollider->SetCenter(Vector2(0.0f, -0.5f));
				animator->PlayAnimation(L"GOLEM_ATTACK_DOWN", true);
				break;
			case MonDir::RIGHT:
				secondCollider->SetCenter(Vector2(+0.5f, -0.1f));
				animator->PlayAnimation(L"GOLEM_ATTACK_RIGHT", true);
				break;
			case MonDir::LEFT:
				secondCollider->SetCenter(Vector2(-0.5f, -0.1f));
				animator->PlayAnimation(L"GOLEM_ATTACK_LEFT", true);
				break;
			default:
				break;
			}
		}
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

void Golem::LateUpdate()
{
	Monster::LateUpdate();
}

void Golem::Render()
{
	Monster::Render();
}

void Golem::OnCollisionEnter(sn::Collider2D* other, sn::Collider2D* me)
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
	if (other->GetName() == L"FisrtCollider" && me->GetName() == L"Second_Collider") {
		snPlayer* player = static_cast<snPlayer*>(other->GetOwner());
		float playerHP = player->GetHP();
		playerHP -= 30.f;
		player->SetHP(playerHP);
		PlayerHP* playerHPComponent = player->GetComponent<PlayerHP>();
		playerHPComponent->PlayDamage(30.f);
	}
}

void Golem::OnCollisionStay(sn::Collider2D* other, sn::Collider2D* me)
{
}

void Golem::OnCollisionExit(sn::Collider2D* other, sn::Collider2D* me)
{
}
