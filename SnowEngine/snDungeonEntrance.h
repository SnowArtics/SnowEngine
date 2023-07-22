#pragma once
#include "snDungeon.h"

namespace sn {
	class DungeonEntrance : public Dungeon
	{
	public:
		DungeonEntrance();
		virtual ~DungeonEntrance();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:


	};
}