#pragma once

#include <map>
#include <snComponent.h>
#include "snPlayerState.h"

	class GameObject;
	class PlayerState; // Forward declaration

	enum class PLAYER_STATE {
		IDLE,
		MOVE,
		ROLL,
		DEAD,
		END,
	};

	enum class PLAYER_DIR {
		UP,
		DOWN,
		RIGHT,
		LEFT,
		END,
	};

namespace sn {

		class PlayerFSM : public Component
		{
		public:

		public:
			PlayerFSM();
			~PlayerFSM();

		public:
			virtual void Update();

		public:
			void ChangeState(PLAYER_STATE _pState);
			void AddState(PlayerState* _pState);

			void SetOwner(sn::GameObject* _m_pOwner) { m_pOwner = _m_pOwner; }
			void SetDirection(PLAYER_DIR _dir) { curDir = _dir; }

			PlayerState* GetState(PLAYER_STATE _state);
			sn::GameObject* GetOwner() { return m_pOwner; }
			PLAYER_DIR GetDirection() { return curDir; }
			std::vector<PLAYER_DIR>& GetActionDir(){ return actionDir; }

		private:
			std::map<PLAYER_STATE, PlayerState*> m_mapeState;
			sn::GameObject*			 m_pOwner;
			PlayerState*							curState;
			PLAYER_STATE					eCurState;
			PLAYER_STATE					ePrevState;

			PLAYER_DIR									curDir;
			PLAYER_DIR									prevDir;

			std::vector<PLAYER_DIR>			actionDir;
		};

	}