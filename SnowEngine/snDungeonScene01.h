#pragma once
#include "snDungeon.h"

namespace sn {
	class DungeonScene01 : public Dungeon
	{
	public:
		DungeonScene01();
		virtual ~DungeonScene01();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:


	};
}


