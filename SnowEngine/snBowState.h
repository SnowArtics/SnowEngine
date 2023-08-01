#pragma once
#include "snPlayerState.h"
class BowState :
    public PlayerState
{
public:
	BowState();
	~BowState();

public:
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

private:
	float time;
	float attTime;
};

