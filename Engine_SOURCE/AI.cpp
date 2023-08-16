#include "AI.h"

#include "State.h"

AI::AI(Monster* _owner)
	: m_pCurState(nullptr)
	, m_pOwner(_owner)
	, prevDir(MonDir::DOWN)
	,curDir(MonDir::DOWN)
{
}

AI::~AI()
{
	for (auto it = m_mapState.begin(); it != m_mapState.end(); ++it) {
		delete it->second; // 포인터가 가리키는 데이터 해제
	}
	m_mapState.clear();
}

void AI::Update()
{

	m_pCurState->Update();

	prevDir = curDir;
}

void AI::AddState(State* _pState)
{
	State* pState = GetState(_pState->GetType());
	assert(!pState);

	m_mapState.insert(std::make_pair(_pState->GetType(), _pState));
	_pState->m_pAI = this;
}

State* AI::GetState(MON_STATE _eState)
{
	std::map<MON_STATE, State*>::iterator iter = m_mapState.find(_eState);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}

void AI::SetCurState(MON_STATE _eState)
{
	m_pCurState = GetState(_eState);
	assert(m_pCurState);
	curStateName = _eState;
}

void AI::ChangeState(MON_STATE _eNextState)
{
	State* pNextState = GetState(_eNextState);

	assert(m_pCurState != pNextState);

	m_pCurState->Exit();
	m_pCurState = pNextState;
	curStateName = _eNextState;
	m_pCurState->Enter();
}
