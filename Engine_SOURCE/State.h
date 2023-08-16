#pragma once
#include "AI.h"

class Monster;

class State 
{
public:
	State(MON_STATE _eState);
	virtual ~State();

public:
	AI* GetAI() { return m_pAI; }
	MON_STATE GetType() { return m_eState; }
	Monster* GetMonster();

public:
	virtual void Update() = 0;
	virtual void Enter() = 0;
	virtual void Exit() = 0;

private:
	AI* m_pAI;
	MON_STATE	m_eState;

	friend class AI;
};