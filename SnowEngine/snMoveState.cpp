#include "snMoveState.h"
#include "..\Engine_SOURCE\snGameObject.h"
#include "..\Engine_SOURCE\snTransform.h"
#include "..\Engine_SOURCE\snTime.h"
#include "..\Engine_SOURCE\snInput.h"

MoveState::MoveState()
	:PlayerState(PLAYER_STATE::MOVE)
{
}

MoveState::~MoveState()
{
}

void MoveState::Update()
{
	Transform* tr = GetPlayerFSM()->GetOwner()->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	std::vector<PLAYER_DIR>& actionDir = GetPlayerFSM()->GetActionDir();

	actionDir.clear();

	if (Input::GetKey(eKeyCode::A))
	{
		pos.x -= 2.3f * Time::DeltaTime();
		GetPlayerFSM()->SetDirection(PLAYER_DIR::LEFT);
		tr->SetPosition(pos);
		actionDir.push_back(PLAYER_DIR::LEFT);
	}
	if (Input::GetKey(eKeyCode::D))
	{
		pos.x += 2.3f * Time::DeltaTime();
		GetPlayerFSM()->SetDirection(PLAYER_DIR::RIGHT);
		tr->SetPosition(pos);
		actionDir.push_back(PLAYER_DIR::RIGHT);
	}
	if (Input::GetKey(eKeyCode::S))
	{
		pos.y -= 2.3f * Time::DeltaTime();
		GetPlayerFSM()->SetDirection(PLAYER_DIR::DOWN);
		tr->SetPosition(pos);
		actionDir.push_back(PLAYER_DIR::DOWN);
	}
	if (Input::GetKey(eKeyCode::W))
	{
		pos.y += 2.3f * Time::DeltaTime();
		GetPlayerFSM()->SetDirection(PLAYER_DIR::UP);
		tr->SetPosition(pos);
		actionDir.push_back(PLAYER_DIR::UP);
	}

	if (!(Input::GetKey(eKeyCode::A))
		&& !(Input::GetKey(eKeyCode::D))
		&& !(Input::GetKey(eKeyCode::S))
		&& !(Input::GetKey(eKeyCode::W))) {
		GetPlayerFSM()->ChangeState(PLAYER_STATE::IDLE);
	}

	if (Input::GetKey(eKeyCode::SPACE)) {
		GetPlayerFSM()->ChangeState(PLAYER_STATE::ROLL);
		return;
	}

	if (Input::GetKeyDown(eKeyCode::J) && GetPlayerFSM()->GetWeaponType() == WEAPON_TYPE::SHORT_SWORD)
		GetPlayerFSM()->ChangeState(PLAYER_STATE::ATTSWORD1);
	if (Input::GetKeyDown(eKeyCode::J) && GetPlayerFSM()->GetWeaponType() == WEAPON_TYPE::BOW)
		GetPlayerFSM()->ChangeState(PLAYER_STATE::ATTBOW);
}

void MoveState::Enter()
{
}

void MoveState::Exit()
{
}
