#include "SlimeAttack.h"
#include "Monster.h"
#include "snSceneManager.h"
#include "snDungeon.h"
#include "snGameObject.h"
#include "snTransform.h"
#include "snTime.h"
#include "snCollider2D.h"

using namespace sn;

bool CompareFloats(float a, float b, int precision) {
	float epsilon = std::pow(10, -precision);
	return std::abs(a - b) < epsilon;
}

SlimeAttack::SlimeAttack()
	: State(MON_STATE::ATT)
	, time(0.f)
	, targetPos()
	, moveDir()
{
	targetPos = Vector3(0.0f, 0.0f, 0.0f);
	moveDir = Vector3(0.0f, 0.0f, 0.0f);
}

SlimeAttack::~SlimeAttack()
{
}

void SlimeAttack::Update()
{
	time += Time::DeltaTime();

	tMonInfo monInfo = GetMonster()->GetMonsterInfo();

	if (time < monInfo.fAttDelay && attackFlag) {
		return;
	}
	else {
		time = 0.f;
		attackFlag = false;
	}

	sn::GameObject* player = SceneManager::GetActiveScene()->GetPlayer();
	Transform* playerTr = player->GetComponent<Transform>();
	Vector3 playerPos = playerTr->GetPosition();

	Monster* mon = GetMonster();
	Transform* monTr = mon->GetComponent <Transform>();
	Vector3 monPos = monTr->GetPosition();

	if (targetPos == Vector3(0.f, 0.f, 0.f)) {
		targetPos = playerPos;
		moveDir = targetPos - monPos;

		if (moveDir.x > monInfo.fAttRange)
			targetPos.x = monPos.x + monInfo.fAttRange;
		if (moveDir.x < -monInfo.fAttRange)
			targetPos.x = monPos.x - monInfo.fAttRange;
		if (moveDir.y > monInfo.fAttRange)
			targetPos.y = monPos.y + monInfo.fAttRange;
		if (moveDir.y < -monInfo.fAttRange)
			targetPos.y = monPos.y - monInfo.fAttRange;
	}

	//Vector3 moveDir = targetPos - monPos;
	moveDir = targetPos - monPos;

	if (/*(abs(moveDir.x) >= mon->GetMonsterInfo().fAttRange || abs(moveDir.y) >= mon->GetMonsterInfo().fAttRange) ||*/ time >= monInfo.fAttTime|| (CompareFloats(monPos.x, targetPos.x,2)&&(CompareFloats(monPos.y, targetPos.y, 2)))) {
		SceneManager::ChangeMonsterState(GetAI(), MON_STATE::TRACE);
		targetPos = Vector3(0.f, 0.f, 0.f);
		moveDir = Vector3(0.f, 0.f, 0.f);
		time = 0.f;
		attackFlag = true;
	}
	moveDir.Normalize();

	monPos += Time::DeltaTime() * moveDir * GetMonster()->GetInfo().fSpeed*5;

	monTr->SetPosition(monPos);
}

void SlimeAttack::Enter()
{
}

void SlimeAttack::Exit()
{
}
