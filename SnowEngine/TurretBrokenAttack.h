#pragma once
#include "State.h"

using namespace sn;

class TurretBrokenAttack :
    public State
{
public:
	TurretBrokenAttack();
	~TurretBrokenAttack();

public:
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

private:
	float time;
};

