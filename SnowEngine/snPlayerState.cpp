#include "snPlayerState.h"

#include "snPlayerFSM.h"

	PlayerState::PlayerState(PLAYER_STATE _state)
		: state(_state)
		, playerFSM(nullptr)
	{
	}
	PlayerState::~PlayerState()
	{
	}