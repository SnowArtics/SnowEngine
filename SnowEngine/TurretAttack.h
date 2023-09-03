#pragma once
#include "State.h"

using namespace sn;

class TurretAttack : public State
{
public:
	TurretAttack();
	~TurretAttack();

public:
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

private:
	float time;
};

