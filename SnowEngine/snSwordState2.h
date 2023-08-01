#pragma once
#include "snPlayerState.h"

class SwordState2 :
    public PlayerState
{
public:
	SwordState2();
	~SwordState2();

public:
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

private:
	float time;
	float attTime;
	bool flag;
};

