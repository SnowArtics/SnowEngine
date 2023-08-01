#pragma once
#include "snPlayerFSM.h"

class PlayerState
{
public:
	PlayerState(PLAYER_STATE _state);
	virtual ~PlayerState();

public:
	virtual void Update() abstract;
	virtual void Enter() abstract;
	virtual void Exit() abstract;

public:
	void SetPlayerFSM(PlayerFSM* _playerFSM) { playerFSM = _playerFSM; }
	void SetState(PLAYER_STATE _state) { state = _state; }

	PlayerFSM* GetPlayerFSM() { return playerFSM; }
	PLAYER_STATE GetState() { return state; }

private:
	PlayerFSM* playerFSM;
	PLAYER_STATE	state;
};

