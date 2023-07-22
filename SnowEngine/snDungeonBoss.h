#pragma once
#include "snDungeon.h"

namespace sn {
	class DungeonBoss : public Dungeon
	{
	public:
		DungeonBoss();
		virtual ~DungeonBoss();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:


	};
}