#pragma once
#include "guiWidget.h"
#include "..\\Engine_SOURCE\\snGameObject.h"

namespace gui
{
	class Component : public Widget
	{
	public:
		Component(sn::enums::eComponentType type);
		~Component();

		virtual void LateUpdate() override;

		void SetTarget(sn::GameObject* gameObj) { mTarget = gameObj; }
		sn::GameObject* GetTarget() { return mTarget; }

	private:
		sn::enums::eComponentType mType;
		sn::GameObject* mTarget;
	};
}