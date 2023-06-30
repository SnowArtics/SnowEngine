#pragma once
//#include "snScene.h"
#include "..\Engine_SOURCE\snScene.h"

namespace sn
{
	class PlayScene : public Scene
	{
	public:
		PlayScene();
		virtual ~PlayScene();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:

	};
}