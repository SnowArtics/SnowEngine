#include "guiTreeWidget.h"


namespace gui
{
	void TreeWidget::Node::Update()
	{
		if (ImGui::TreeNode(name.c_str()))
		{
			for (Node* node : childs)
				node->Update();

			ImGui::TreePop();
		}
	}

	TreeWidget::TreeWidget()
		: Widget()
		, mRoot(nullptr)
	{
	}
	TreeWidget::~TreeWidget()
	{
	}
	void TreeWidget::FixedUpdate()
	{
	}
	void TreeWidget::Update()
	{
		if (mRoot == nullptr)
			return;

		mRoot->Update();
	}
	void TreeWidget::LateUpdate()
	{
	}
	void TreeWidget::Render()
	{
	}

}
