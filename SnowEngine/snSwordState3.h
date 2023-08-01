#pragma once
#include "snPlayerState.h"
class SwordState3 :
    public PlayerState
{
public:
	SwordState3();
	~SwordState3();

public:
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

private:
	float	time;
	float	attTime;
	bool	flag;
};

