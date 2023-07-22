#pragma once
#include "snDungeon.h"

namespace sn {
	class DungeonScene02 : public Dungeon
	{
	public:
		DungeonScene02();
		virtual ~DungeonScene02();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:


	};
}