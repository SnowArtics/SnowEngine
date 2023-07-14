#include "guiEditor.h"
#include "../Engine_SOURCE/snMesh.h"
#include "../Engine_SOURCE/snResources.h"
#include "../Engine_SOURCE/snTransform.h"
#include "../Engine_SOURCE/snMeshRenderer.h"
#include "../Engine_SOURCE/snMaterial.h"
#include "../SnowEngine/snGridScript.h"
#include "../Engine_SOURCE/snRenderer.h"

namespace gui
{
	using namespace sn::enums;
	std::vector<Widget*> Editor::mWidgets = {};
	std::vector<EditorObject*> Editor::mEditorObjects = {};
	std::vector<DebugObject*> Editor::mDebugObjects = {};

	void Editor::Initialize()
	{
		mDebugObjects.resize((UINT)eColliderType::End);

		//사각형 디버그 오브젝트 생성
		std::shared_ptr<sn::Mesh> mesh
			= sn::Resources::Find<sn::Mesh>(L"DebugRect");
		std::shared_ptr<sn::Material> material
			= sn::Resources::Find<sn::Material>(L"DebugMaterial");

		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		sn::MeshRenderer* mr
			= mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<sn::MeshRenderer>();
		mr->SetMaterial(material);
		mr->SetMesh(mesh);

		//원 디버그 오브젝트 생성
		mesh = sn::Resources::Find<sn::Mesh>(L"DebugCircle");
		material = sn::Resources::Find<sn::Material>(L"DebugMaterial");

		mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();
		mr = mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<sn::MeshRenderer>();
		mr->SetMaterial(material);
		mr->SetMesh(mesh);

		//에디터 오브젝트 생성
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
		renderer::debugMeshs.clear();
	}
	void Editor::Release()
	{
		for (auto widget : mWidgets)
		{
			delete widget;
			widget = nullptr;
		}

		for (auto editorObj : mEditorObjects)
		{
			delete editorObj;
			editorObj = nullptr;
		}

		for (auto debugObj : mDebugObjects)
		{
			delete debugObj;
			debugObj = nullptr;
		}
	}

	void Editor::DebugRender(const sn::graphics::DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];

		// 위치 크기 회전 정보를 받아와서
		// 해당 게임오브젝트위에 그려주면된다.
		sn::Transform* tr = debugObj->GetComponent<sn::Transform>();

		Vector3 pos = mesh.position;
		pos.z -= 0.01f;

		tr->SetPosition(pos);
		tr->SetScale(mesh.scale);
		tr->SetRotationByRadian(mesh.rotation);

		tr->LateUpdate();

		/*sn::MeshRenderer * mr
			= debugObj->GetComponent<sn::MeshRenderer>();*/
			// main camera
		sn::Camera* mainCamara = renderer::mainCamera;

		sn::Camera::SetGpuViewMatrix(mainCamara->GetViewMatrix());
		sn::Camera::SetGpuProjectionMatrix(mainCamara->GetProjectionMatrix());


		debugObj->Render();
	}
}
