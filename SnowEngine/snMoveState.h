#pragma once
#include "snPlayerState.h"

class MoveState :
    public PlayerState
{
public:
	MoveState();
	~MoveState();

public:
	virtual void Update();
	virtual void Enter();
	virtual void Exit();
};

