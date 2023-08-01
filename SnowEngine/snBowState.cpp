#include "snBowState.h"
#include "..\Engine_SOURCE\snGameObject.h"
#include "..\Engine_SOURCE\snTransform.h"
#include "..\Engine_SOURCE\snTime.h"
#include "..\Engine_SOURCE\snInput.h"
#include "snPlayerFSM.h"

BowState::BowState()
	:PlayerState(PLAYER_STATE::ATTBOW)
	, time(0.f)
	, attTime(0.65f)
{
}

BowState::~BowState()
{
}

void BowState::Update()
{
	time += Time::DeltaTime();

	PLAYER_DIR playerDir = GetPlayerFSM()->GetDirection();
	Transform* tr = GetPlayerFSM()->GetOwner()->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	if (playerDir == PLAYER_DIR::UP && time >= 0.1f && time < (attTime - 0.1f)) {
		pos.y -= 1.f * Time::DeltaTime();
		tr->SetPosition(pos);
	}
	else if (playerDir == PLAYER_DIR::DOWN && time >= 0.1f && time < (attTime - 0.1f)) {
		pos.y += 1.f * Time::DeltaTime();
		tr->SetPosition(pos);
	}
	else if (playerDir == PLAYER_DIR::LEFT && time >= 0.1f && time < (attTime - 0.1f)) {
		pos.x += 1.f * Time::DeltaTime();
		tr->SetPosition(pos);
	}
	else if (playerDir == PLAYER_DIR::RIGHT && time >= 0.1f && time < (attTime - 0.1f)) {
		pos.x -= 1.f * Time::DeltaTime();
		tr->SetPosition(pos);
	}

	if(time>=attTime)
		GetPlayerFSM()->ChangeState(PLAYER_STATE::IDLE);
}

void BowState::Enter()
{
}

void BowState::Exit()
{
	time = 0.f;
}
