#pragma once
#include "guiWidget.h"
#include "guiComponent.h"

#include "..\\Engine_SOURCE\\snGameObject.h"

namespace gui
{
	class InspectorView : public Widget
	{
	public:
		InspectorView();
		~InspectorView();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
		sn::GameObject* mTarget;
		std::vector<gui::Component*> mComponents;
	};
}