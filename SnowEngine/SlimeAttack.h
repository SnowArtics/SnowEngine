#pragma once
#include "../Engine_SOURCE/State.h"

using namespace sn;

class SlimeAttack : public State
{
public:
	SlimeAttack();
	~SlimeAttack();

public:
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

private:
	float time;
	Vector3 targetPos;
	Vector3 moveDir;
	bool attackFlag;
};

