#pragma once
#include "../Engine_SOURCE/State.h"
class MonsterIdle :
    public State
{
public:
	MonsterIdle();
	~MonsterIdle();

public:
	virtual void Update();
	virtual void Enter();
	virtual void Exit();
};

