#include "State.h"

State::State(MON_STATE _eState)
	: m_eState(_eState)
	, m_pAI(nullptr)
{
}

State::~State()
{
}

Monster* State::GetMonster()
{
	return m_pAI->GetOwner();
}

void State::Update()
{
}

void State::Enter()
{
}

void State::Exit()
{
}
