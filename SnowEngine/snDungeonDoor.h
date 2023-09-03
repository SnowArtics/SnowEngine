#pragma once
#include "snScript.h"

enum class DoorType{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

namespace sn
{
	class DungeonDoor : public Script
	{
	public:
		DungeonDoor(DoorType _doorType, std::pair<int,int> _doorMapPos);
		~DungeonDoor();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider2D* other);
		virtual void OnCollisionStay(Collider2D* other);
		virtual void OnCollisionExit(Collider2D* other);

	private:
		DoorType doorType;
		std::pair<int, int> doorMapPos;

		std::pair<int, int>				prevPlayerMapPos;
		std::pair<int, int>				curPlayerMapPos;

		bool									doorOpen;
		static bool						monsterSpawnFlag;
	};
}

