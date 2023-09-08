#include "guiComponent.h"


namespace gui
{
	const char* charComponentType[(int)sn::enums::eComponentType::End] =
	{
		//component
		"Component",
		"Transform",
		"Camera",
		"Collider",
		"Animator",
		"Light",

		//render
		"MeshRenderer",
		"SpriteRenderer",
		"ParticleSystem",
		"SkyBox",
		"Decal",

		//etc
		"Script",
	};

	const wchar_t* wcharComponentType[(int)sn::enums::eComponentType::End] =
	{
		//component
		L"Component",
		L"Transform",
		L"Camera",
		L"Collider",
		L"Animator",
		L"Light",

		//render
		L"MeshRenderer",
		L"SpriteRenderer",
		L"ParticleSystem",
		L"SkyBox",
		L"Decal",

		//etc
		L"Script",
	};


	Component::Component(sn::enums::eComponentType type)
		: mType(type)
		, mTarget(nullptr)
	{

	}

	Component::~Component()
	{

	}

	void Component::LateUpdate()
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
		ImGui::Button(charComponentType[(UINT)mType]);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}
}
