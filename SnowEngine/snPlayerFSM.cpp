#include "snPlayerFSM.h"
#include "snPlayerState.h"

#include "..\Engine_SOURCE\snAnimator.h"
#include "..\Engine_SOURCE\snGameObject.h"
#include "..\Engine_SOURCE\snTime.h"
#include "..\Engine_SOURCE\snInput.h"

PlayerFSM::PlayerFSM()
    :Component(eComponentType::FSM)
	, m_pOwner(nullptr)
	, curState(nullptr)
	, eCurState(PLAYER_STATE::END)
	, ePrevState(PLAYER_STATE::END)
	, curDir(PLAYER_DIR::END)
	, prevDir(PLAYER_DIR::END)
	, curAttState(0)
	, prevAttState(0)
	, weaponType(WEAPON_TYPE::SHORT_SWORD)
{
}

PlayerFSM::~PlayerFSM()
{
	for (auto it = m_mapeState.begin(); it != m_mapeState.end();) {
		delete it->second;
		it = m_mapeState.erase(it);
	}
}

void PlayerFSM::Update()
{
	if (curState != nullptr)
		curState->Update();

	if (Input::GetKeyDown(eKeyCode::Z)) {
		if (weaponType == WEAPON_TYPE::SHORT_SWORD) {
			weaponType = WEAPON_TYPE::BOW;
		}
		else if (weaponType == WEAPON_TYPE::BOW) {
			weaponType = WEAPON_TYPE::SHORT_SWORD;
		}
	}

	if (curDir == prevDir && eCurState == ePrevState)
		return;

	Animator* at = GetOwner()->GetComponent<Animator>();

	//이 밑은 애니메이션 출력 관련
	switch (eCurState)
	{
	case PLAYER_STATE::IDLE:
		switch (curDir)
		{
		case PLAYER_DIR::UP:
			at->PlayAnimation(L"IDLE_UP", true);
			break;
		case PLAYER_DIR::DOWN:
			at->PlayAnimation(L"IDLE_DOWN", true);
			break;
		case PLAYER_DIR::RIGHT:
			at->PlayAnimation(L"IDLE_RIGHT", true);
			break;
		case PLAYER_DIR::LEFT:
			at->PlayAnimation(L"IDLE_LEFT", true);
			break;
		default:
			break;
		}
		break;
	case PLAYER_STATE::MOVE:
		switch (curDir)
		{
		case PLAYER_DIR::UP:
			at->PlayAnimation(L"MOVE_UP", true);
			break;
		case PLAYER_DIR::DOWN:
			at->PlayAnimation(L"MOVE_DOWN", true);
			break;
		case PLAYER_DIR::RIGHT:
			at->PlayAnimation(L"MOVE_RIGHT", true);
			break;
		case PLAYER_DIR::LEFT:
			at->PlayAnimation(L"MOVE_LEFT", true);
			break;
		default:
			break;
		}
		break;
	case PLAYER_STATE::ROLL:
		switch (curDir)
		{
		case PLAYER_DIR::UP:
			at->PlayAnimation(L"ROLL_UP", true);
			break;
		case PLAYER_DIR::DOWN:
			at->PlayAnimation(L"ROLL_DOWN", true);
			break;
		case PLAYER_DIR::RIGHT:
			at->PlayAnimation(L"ROLL_RIGHT", true);
			break;
		case PLAYER_DIR::LEFT:
 			at->PlayAnimation(L"ROLL_LEFT", true);
			break;
		default:
			break;
		}
		break;
	case PLAYER_STATE::ATTSWORD1:		
		switch (curDir)
		{
		case PLAYER_DIR::UP:
			at->PlayAnimation(L"SWORD_UP1", false);
			break;
		case PLAYER_DIR::DOWN:
			at->PlayAnimation(L"SWORD_DOWN1", false);
			break;
		case PLAYER_DIR::RIGHT:
			at->PlayAnimation(L"SWORD_RIGHT1", false);
			break;
		case PLAYER_DIR::LEFT:
			at->PlayAnimation(L"SWORD_LEFT1", false);
			break;
		}
		break;
	case PLAYER_STATE::ATTSWORD2:
		switch (curDir)
		{
		case PLAYER_DIR::UP:
			at->PlayAnimation(L"SWORD_UP2", false);
			break;
		case PLAYER_DIR::DOWN:
			at->PlayAnimation(L"SWORD_DOWN2", false);
			break;
		case PLAYER_DIR::RIGHT:
			at->PlayAnimation(L"SWORD_RIGHT2", false);
			break;
		case PLAYER_DIR::LEFT:
			at->PlayAnimation(L"SWORD_LEFT2", false);
			break;
		}
		break;
	case PLAYER_STATE::ATTSWORD3:
		switch (curDir)
		{
		case PLAYER_DIR::UP:
			at->PlayAnimation(L"SWORD_UP1", false);
			break;
		case PLAYER_DIR::DOWN:
			at->PlayAnimation(L"SWORD_DOWN1", false);
			break;
		case PLAYER_DIR::RIGHT:
			at->PlayAnimation(L"SWORD_RIGHT1", false);
			break;
		case PLAYER_DIR::LEFT:
			at->PlayAnimation(L"SWORD_LEFT1", false);
			break;
		}
		break;
	case PLAYER_STATE::ATTBOW:
		switch (curDir)
		{
		case PLAYER_DIR::UP:
			at->PlayAnimation(L"BOW_UP", false);
			break;
		case PLAYER_DIR::DOWN:
			at->PlayAnimation(L"BOW_DOWN", false);
			break;
		case PLAYER_DIR::RIGHT:
			at->PlayAnimation(L"BOW_RIGHT", false);
			break;
		case PLAYER_DIR::LEFT:
			at->PlayAnimation(L"BOW_LEFT", false);
			break;
		}
		break;
	case PLAYER_STATE::DEAD:
		break;
	default:
		break;
	}

	prevDir = curDir;
	ePrevState = eCurState;
}

void PlayerFSM::ChangeState(PLAYER_STATE _pState)
{
	ePrevState = eCurState;
	curState->Exit();
	eCurState = _pState;
	std::map<PLAYER_STATE, PlayerState*>::iterator state = m_mapeState.find(eCurState);
	state->second->Enter();
	curState = state->second;
}

void PlayerFSM::AddState(PlayerState* _pState)
{
	curState = nullptr;
	PlayerState* pState = GetState(_pState->GetState());
	if (pState != nullptr)
		return;

	// Implement AddState function
	m_mapeState.insert(std::make_pair(_pState->GetState(), _pState));
	_pState->SetPlayerFSM(this);
	eCurState = _pState->GetState();
	curState = _pState;
	curDir = PLAYER_DIR::DOWN;
}

PlayerState* PlayerFSM::GetState(PLAYER_STATE _state)
{
	std::map<PLAYER_STATE, PlayerState*>::iterator state = m_mapeState.find(_state);
	if (state == m_mapeState.end())
		return nullptr;

	return state->second;
}
