#pragma once
#include "..\Engine_SOURCE\snScene.h"

namespace sn
{
	class ShopScene : public Scene
	{
	public:
		ShopScene();
		virtual ~ShopScene();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;
		virtual void OnEnter() override;
		virtual void OnExit() override;

	private:


	};
}