#include "guiEditor.h"
#include "snMesh.h"
#include "snResources.h"
#include "snTransform.h"
#include "snMeshRenderer.h"
#include "snMaterial.h"
#include "snGridScript.h"
#include "snRenderer.h"

namespace gui
{
	using namespace sn::enums;
	std::vector<Widget*> Editor::mWidgets = {};
	std::vector<EditorObject*> Editor::mEditorObjects = {};
	std::vector<DebugObject*> Editor::mDebugObjects = {};

	void Editor::Initialize()
	{
		mDebugObjects.resize((UINT)eColliderType::End);

		std::shared_ptr<sn::Mesh> mesh
			= sn::Resources::Find<sn::Mesh>(L"DebugRect");
		std::shared_ptr<sn::Material> material
			= sn::Resources::Find<sn::Material>(L"DebugMaterial");

		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<sn::Transform>();
		sn::MeshRenderer* mr
			= mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<sn::MeshRenderer>();
		mr->SetMaterial(material);
		mr->SetMesh(mesh);


		EditorObject* grid = new EditorObject();
		grid->SetName(L"Grid");

		mr = grid->AddComponent<sn::MeshRenderer>();
		mr->SetMesh(sn::Resources::Find<sn::Mesh>(L"RectMesh"));
		mr->SetMaterial(sn::Resources::Find<sn::Material>(L"GridMaterial"));
		sn::GridScript* gridSc = grid->AddComponent<sn::GridScript>();
		gridSc->SetCamera(renderer::cameras[0]);

		mEditorObjects.push_back(grid);

	}
	void Editor::Run()
	{
		Update();
		LateUpdate();
		Render();
	}
	void Editor::Update()
	{


		for (EditorObject* obj : mEditorObjects)
		{
			obj->Update();
		}
	}
	void Editor::LateUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->LateUpdate();
		}
	}
	void Editor::Render()
	{
		//Microsoft::WRL::ComPtr<ID3D11DepthStencilState> ds
		//	= renderer::depthStencilStates[(UINT)sn::graphics::eDSType::Less];
		//sn::graphics::GetDevice()->BindDepthStencilState(ds.Get());

		for (EditorObject* obj : mEditorObjects)
		{
			obj->Render();
		}

		for (const sn::graphics::DebugMesh& mesh
			: renderer::debugMeshs)
		{
			DebugRender(mesh);
		}
	}
	void Editor::Release()
	{
	}

	void Editor::DebugRender(const sn::graphics::DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];

		// 위치 크기 회전 정보를 받아와서
		// 해당 게임오브젝트위에 그려주면된다.



		debugObj->Render();
	}
}
