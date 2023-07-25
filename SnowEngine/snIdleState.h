#pragma once
#include "snPlayerState.h"

	class IdleState : public PlayerState
	{
	public:
		IdleState();
		~IdleState();

	public:
		virtual void Update();
		virtual void Enter();
		virtual void Exit();

	};	
