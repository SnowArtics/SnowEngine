#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"

namespace gui
{
	class ProjectView : public Widget
	{
	public:
		ProjectView();
		~ProjectView();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
		//Tree 자료구조
		TreeWidget* mTreeWidget;
	};
}