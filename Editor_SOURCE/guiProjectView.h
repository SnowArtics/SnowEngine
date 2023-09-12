#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"
#include "Engine_SOURCE\\snResources.h"

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

		template <typename T>
		void AddResources(TreeWidget::Node* rootNode, const char* name)
		{
			const std::vector<std::shared_ptr<T>> resources
				= sn::Resources::Finds<T>();

			TreeWidget::Node* stemNode
				= mTreeWidget->AddNode(rootNode, name, 0, true);

			for (std::shared_ptr<T> resource : resources)
			{
				std::string name(resource->GetKey().begin(), resource->GetKey().end());
				mTreeWidget->AddNode(stemNode, name, resource.get());
			}
		}

	private:
		//Tree 자료구조
		TreeWidget* mTreeWidget;
	};
}