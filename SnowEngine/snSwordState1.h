#pragma once
#include "snPlayerState.h"

class SwordState1 :
    public PlayerState
{
public:
	SwordState1();
	~SwordState1();

public:
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

private:
	float time;
	float attTime;
	bool flag;
};

