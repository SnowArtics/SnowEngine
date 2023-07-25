#include "snPlayerFSM.h"
#include <map>

#include "snPlayerState.h"
#include "snAnimator.h"

namespace sn {

	PlayerFSM::PlayerFSM()
		:Component(eComponentType::FSM)
		, m_pOwner(nullptr)
		, curState(nullptr)
		, eCurState(PLAYER_STATE::END)
		, ePrevState(PLAYER_STATE::END)
		, curDir(PLAYER_DIR::END)
		, prevDir(PLAYER_DIR::END)
	{
		m_mapeState = std::map<PLAYER_STATE, PlayerState*>();
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
		if(curState != nullptr)
			curState->Update();

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

}