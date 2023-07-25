#pragma once
#include "snPlayerState.h"

    class RollState :
        public PlayerState
    {
	public:
		RollState();
		~RollState();

	public:
		virtual void Update();
		virtual void Enter();
		virtual void Exit();

	private:
		float time;
    };