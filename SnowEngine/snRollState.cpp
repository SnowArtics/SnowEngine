#include "snRollState.h"
#include "..\Engine_SOURCE\snGameObject.h"
#include "..\Engine_SOURCE\snTransform.h"
#include "..\Engine_SOURCE\snTime.h"
#include "..\Engine_SOURCE\snInput.h"

RollState::RollState()
	: PlayerState(PLAYER_STATE::ROLL)
	, time(0.f)
{
}

RollState::~RollState()
{
}

void RollState::Update()
{
	time += Time::DeltaTime();

	std::vector<PLAYER_DIR>& actionDir = GetPlayerFSM()->GetActionDir();
	Transform* tr = GetPlayerFSM()->GetOwner()->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	if (actionDir.size() == 0) {
		switch (GetPlayerFSM()->GetDirection())
		{
		case PLAYER_DIR::UP:
			pos.y += 2.0f * Time::DeltaTime();
			tr->SetPosition(pos);
			break;
		case PLAYER_DIR::DOWN:
			pos.y -= 2.0f * Time::DeltaTime();
			tr->SetPosition(pos);
			break;
		case PLAYER_DIR::RIGHT:
			pos.x += 2.0f * Time::DeltaTime();
			tr->SetPosition(pos);
			break;
		case PLAYER_DIR::LEFT:
			pos.x -= 2.0f * Time::DeltaTime();
			tr->SetPosition(pos);
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < actionDir.size(); i++) {
		switch (actionDir[i])
		{
		case PLAYER_DIR::UP:
			pos.y += 2.0f * Time::DeltaTime();
			tr->SetPosition(pos);
			break;
		case PLAYER_DIR::DOWN:
			pos.y -= 2.0f * Time::DeltaTime();
			tr->SetPosition(pos);
			break;
		case PLAYER_DIR::RIGHT:
			pos.x += 2.0f * Time::DeltaTime();
			tr->SetPosition(pos);
			break;
		case PLAYER_DIR::LEFT:
			pos.x -= 2.0f * Time::DeltaTime();
			tr->SetPosition(pos);
			break;
		default:
			break;
		}
	}

	if (time > 0.48f)
		GetPlayerFSM()->ChangeState(PLAYER_STATE::MOVE);
}

void RollState::Enter()
{
}

void RollState::Exit()
{
	time = 0.f;
}
