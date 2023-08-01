#include "snPlayerState.h"

PlayerState::PlayerState(PLAYER_STATE _state)
	: state(_state)
	, playerFSM(nullptr)
{
}

PlayerState::~PlayerState()
{
}
