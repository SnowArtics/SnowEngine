#pragma once
#include "..\Engine_SOURCE\snScene.h"

struct RoomInfo {
	int		monsterNum;
	bool clear;
};

namespace sn
{
	class Dungeon : public Scene
	{
	public:
		Dungeon();
		virtual ~Dungeon();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void OnEnter() override;
		virtual void OnExit() override;

	public:
		void SetPlayerMapPos(int x, int y) { playerMapPos.first = y, playerMapPos.second = x; }
		void SetPlayerMapPos(std::pair<int, int> _playerMapPos) { playerMapPos = _playerMapPos; }
		void SetRoomInfoArr(std::vector<std::vector<RoomInfo>> _RoomInfoArr) { roomInfoArr = _RoomInfoArr; }
		void SetDungeonClear(bool _dungeonClear) { dungeonClear = _dungeonClear; }

		std::pair<int, int> GetPlayerMapPos() { return playerMapPos; }
		std::vector<std::vector<RoomInfo>> GetRoomInfoArr() { return roomInfoArr; }
		bool GetDungeonClear() { return dungeonClear; }

	protected:
		std::pair<int, int>				playerMapPos;
		std::vector<std::vector<RoomInfo>> roomInfoArr;
		bool							dungeonClear;
	};
}