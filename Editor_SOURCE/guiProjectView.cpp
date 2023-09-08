#include "guiProjectView.h"

namespace gui
{



	ProjectView::ProjectView()
		: mTreeWidget(nullptr)
	{
		mTreeWidget = new TreeWidget();
		AddWidget(mTreeWidget);
	}

	ProjectView::~ProjectView()
	{
	}

	void ProjectView::FixedUpdate()
	{
		Widget::FixedUpdate();
	}

	void ProjectView::Update()
	{
		Widget::Update();
	}

	void ProjectView::LateUpdate()
	{
		//Widget::LateUpdate();
	}

	void ProjectView::Render()
	{
		Widget::Render();
	}

}
