#include "MonsterAttack.h"
#include "Monster.h"
#include "snSceneManager.h"
#include "snDungeon.h"
#include "snGameObject.h"
#include "snTransform.h"
#include "snTime.h"
#include "snCollider2D.h"

using namespace sn;

MonsterAttack::MonsterAttack()
	: State(MON_STATE::ATT)
	, time(0.f)
{
}

MonsterAttack::~MonsterAttack()
{
}

void MonsterAttack::Update()
{
	time += Time::DeltaTime();

	sn::GameObject* player = SceneManager::GetActiveScene()->GetPlayer();
	Transform* playerTr = player->GetComponent<Transform>();
	Vector3 playerPos = playerTr->GetPosition();

	Monster* mon = GetMonster();
	Transform* monTr = mon->GetComponent <Transform>();
	Vector3 monPos = monTr->GetPosition();

	Vector3 moveDir = playerPos - monPos;

	tMonInfo monInfo = GetMonster()->GetMonsterInfo();

	if ((abs(moveDir.x) >= mon->GetMonsterInfo().fAttRange || abs(moveDir.y) >= mon->GetMonsterInfo().fAttRange)&& time>= monInfo.fAttTime) {
		SceneManager::ChangeMonsterState(GetAI(), MON_STATE::TRACE);
		time = 0.f;
	}
	moveDir.Normalize();

	bool upDown = true;
	if (abs(moveDir.x) > abs(moveDir.y)) {
		upDown = false;
	}
	else {
		upDown = true;
	}

	if (upDown) {
		if (moveDir.y < 0.f) {
			GetAI()->SetDir(MonDir::DOWN);
		}
		else {
			GetAI()->SetDir(MonDir::UP);
		}
	}
	else {
		if (moveDir.x < 0.f) {
			GetAI()->SetDir(MonDir::LEFT);
		}
		else {
			GetAI()->SetDir(MonDir::RIGHT);
		}
	}

	monTr->SetPosition(monPos);
}

void MonsterAttack::Enter()
{
}

void MonsterAttack::Exit()
{
}
