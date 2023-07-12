#pragma once
#include "../Engine_SOURCE/snGameObject.h"


namespace gui
{
	class DebugObject : public sn::GameObject
	{
	public:
		DebugObject();
		~DebugObject();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
	};
}
