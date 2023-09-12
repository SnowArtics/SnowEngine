#include "guiInspectorView.h"
#include "guiTransform.h"

#include "..\\Engine_SOURCE\\snRenderer.h"

namespace gui
{
	InspectorView::InspectorView()
		: mTarget(nullptr)
		, mComponents{}
	{
		mTarget = renderer::guiTarget;
		mComponents.resize((UINT)sn::enums::eComponentType::End);

		mComponents[(UINT)sn::enums::eComponentType::Transform] = new gui::Transform();
		mComponents[(UINT)sn::enums::eComponentType::Transform]->SetSize(ImVec2(0, 150));
		mComponents[(UINT)sn::enums::eComponentType::Transform]->SetTarget(mTarget);

		AddWidget(mComponents[(UINT)sn::enums::eComponentType::Transform]);
	}
	InspectorView::~InspectorView()
	{
		for (gui::Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			delete comp;
			comp = nullptr;
		}
	}
	void InspectorView::FixedUpdate()
	{
	}
	void InspectorView::Update()
	{
		if (mTarget == nullptr) {
			mTarget = renderer::guiTarget;
			mComponents[(UINT)sn::enums::eComponentType::Transform]->SetTarget(mTarget);
		}

	}
	void InspectorView::LateUpdate()
	{
	}
	void InspectorView::Render()
	{
		Widget::Render();
	}
}
