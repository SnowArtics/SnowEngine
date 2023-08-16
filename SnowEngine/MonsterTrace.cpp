#include "MonsterTrace.h"
#include "Monster.h"
#include "snSceneManager.h"
#include "snDungeon.h"
#include "snGameObject.h"
#include "snTransform.h"
#include "snTime.h"

using namespace sn;

MonsterTrace::MonsterTrace()
	: State(MON_STATE::TRACE)
{
}

MonsterTrace::~MonsterTrace()
{
}

void MonsterTrace::Update()
{
	GameObject* player = SceneManager::GetActiveScene()->GetPlayer();
	Transform* playerTr = player->GetComponent<Transform>();
	Vector3 playerPos = playerTr->GetPosition();

	Monster* mon = GetMonster();
	Transform* monTr = mon->GetComponent < Transform>();
	Vector3 monPos = monTr->GetPosition();

	Vector3 moveDir = playerPos - monPos;
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

	monPos += Time::DeltaTime() * moveDir * GetMonster()->GetInfo().fSpeed;

	monTr->SetPosition(monPos);
}

void MonsterTrace::Enter()
{
}

void MonsterTrace::Exit()
{
}
