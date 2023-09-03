#pragma once
#include "snGameObject.h"

#define SINGLE(type) public:\
						static type* GetInst()\
						{\
							static type mgr;\
							return &mgr;\
						}\
					private:\
						type();\
						~type();\

struct RoomInfo {
	int		monsterNum;
	bool	clear;
	int		roomNum;
};

using namespace sn;

class DungeonMapManager
{
	SINGLE(DungeonMapManager);
public:
	void Init();
	void MakeDungeonBackground(std::vector<std::wstring> _DungeonName);
	void MakeCliffCollider(int _num, sn::GameObject* _background);
	void MakeDoor();
	void MonsterSpawn();
	void MonsterSpawn(int _num, int i, int j);

public: //Get(), Set()
	std::vector<std::vector<int>> GetDungeonArr() { return arr; }
	std::pair<int, int> GetPlayerMapPos() { return playerMapPos; }
	std::vector<std::vector<RoomInfo>>& GetRoomInfoArr() { return roomInfoArr; }
	bool GetDungeonClear() { return dungeonClear; }

	void SetDungeonArr(std::vector<std::vector<int>> _arr) { arr = _arr; }
	void SetRoomInfoArr(std::vector<std::vector<RoomInfo>> _roomInfoArr) { roomInfoArr = _roomInfoArr; }
	void SetPlayerMapPos(int x, int y) { playerMapPos.first = y, playerMapPos.second = x; }
	void SetPlayerMapPos(std::pair<int, int> _playerMapPos) { playerMapPos = _playerMapPos; }
	void SetDungeonClear(bool _dungeonClear) { dungeonClear = _dungeonClear; }

private:
	std::vector<std::vector<int>> arr;
	std::pair<int, int>				playerMapPos;
	std::vector<std::vector<RoomInfo>> roomInfoArr;
	bool							dungeonClear;
};

