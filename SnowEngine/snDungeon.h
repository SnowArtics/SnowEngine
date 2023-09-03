#pragma once
#include "..\Engine_SOURCE\snScene.h"

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
	};
}