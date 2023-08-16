#pragma once
#include "../Engine_SOURCE/State.h"
class MonsterTrace :
    public State
{
public:
	MonsterTrace();
	~MonsterTrace();

public:
	virtual void Update();
	virtual void Enter();
	virtual void Exit();
};

