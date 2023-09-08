#pragma once
#include "guiWidget.h"


namespace gui
{
	class TreeWidget : public Widget
	{
	public:
		struct Node
		{
			void Update();

			Node* parent;
			std::vector<Node*> childs;
			std::string name;
		};

		TreeWidget();
		~TreeWidget();

		virtual void FixedUpdate();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
		virtual void Close() {}

	private:
		Node* mRoot;
	};
}
