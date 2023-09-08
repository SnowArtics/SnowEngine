#include "guiTransform.h"
#include "..\\Engine_SOURCE\\snTransform.h"

namespace gui
{
	Transform::Transform()
		: Component(sn::enums::eComponentType::Transform)
	{
		SetName("Transform");
	}

	Transform::~Transform()
	{

	}

	void Transform::Update()
	{
		sn::Transform* tr = GetTarget()->GetComponent<sn::Transform>();

		mPosition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();

		Component::Update();
	}

	void Transform::LateUpdate()
	{
		Component::LateUpdate();

		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale   "); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		//ImGui::Text("Ignore Parent Scale"); 
		//ImGui::SameLine(); ImGui::Checkbox("##IgnorParentScale", &m_bIgnorScale);

		if (GetTarget())
		{
			sn::Transform* tr = GetTarget()->GetComponent<sn::Transform>();

			tr->SetPosition(mPosition);
			tr->SetRotationByRadian(mRotation);
			tr->SetScale(mScale);
		}
	}

}
