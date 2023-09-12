#include "guiProjectView.h"
#include "Engine_SOURCE\\snMesh.h"
#include "Engine_SOURCE\\snMaterial.h"
#include "Engine_SOURCE\\snTexture.h"
#include "Engine_SOURCE\\snShader.h"

namespace gui
{
	ProjectView::ProjectView()
		: mTreeWidget(nullptr)
	{
		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("Resources");
		//reeWidget->SetDummyRoot(true);
		AddWidget(mTreeWidget);

		TreeWidget::Node* stem = mTreeWidget->AddNode(nullptr, "Resources", 0, false);

		AddResources<sn::Mesh>(stem, "Mesh");
		AddResources<sn::graphics::Texture>(stem, "Texture");
		AddResources<sn::graphics::Material>(stem, "Materials");
		AddResources<sn::Shader>(stem, "Shaders");
	}

	ProjectView::~ProjectView()
	{
		delete mTreeWidget;
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
