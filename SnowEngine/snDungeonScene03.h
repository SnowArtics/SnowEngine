#pragma once
#include "snDungeon.h"

namespace sn {
	class DungeonScene03 : public Dungeon
	{
	public:
		DungeonScene03();
		virtual ~DungeonScene03();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:


	};
}