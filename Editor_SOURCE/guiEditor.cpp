#include "guiEditor.h"
#include "../Engine_SOURCE/snMesh.h"
#include "../Engine_SOURCE/snResources.h"
#include "../Engine_SOURCE/snTransform.h"
#include "../Engine_SOURCE/snMeshRenderer.h"
#include "../Engine_SOURCE/snMaterial.h"
#include "../SnowEngine/snGridScript.h"
#include "../Engine_SOURCE/snRenderer.h"

#include "..\\Engine_SOURCE\\snApplication.h"
#include "..\\Engine_SOURCE\\snGraphicDevice_Dx11.h"

extern sn::Application application;

namespace gui
{
	using namespace sn::enums;
	std::vector<Widget*> Editor::mWidgets = {};
	std::vector<EditorObject*> Editor::mEditorObjects = {};
	std::vector<DebugObject*> Editor::mDebugObjects = {};
	ImGuiIO Editor::mIO = {};

	void Editor::Initialize()
	{
		//static ID3D11Device* g_pd3dDevice = nullptr;
		//static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
		//object

		mDebugObjects.resize((UINT)eColliderType::End);

		//�簢�� ����� ������Ʈ ����
		std::shared_ptr<sn::Mesh> mesh
			= sn::Resources::Find<sn::Mesh>(L"DebugRect");
		std::shared_ptr<sn::Material> material
			= sn::Resources::Find<sn::Material>(L"DebugMaterial");

		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		sn::MeshRenderer* mr
			= mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<sn::MeshRenderer>();
		mr->SetMaterial(material);
		mr->SetMesh(mesh);

		//�� ����� ������Ʈ ����
		mesh = sn::Resources::Find<sn::Mesh>(L"DebugCircle");
		material = sn::Resources::Find<sn::Material>(L"DebugMaterial");

		mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();
		mr = mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<sn::MeshRenderer>();
		mr->SetMaterial(material);
		mr->SetMesh(mesh);

		//������ ������Ʈ ����
		if (renderer::cameras.size() > 0) {
			EditorObject* grid = new EditorObject();
			grid->SetName(L"Grid");

			mr = grid->AddComponent<sn::MeshRenderer>();
			mr->SetMesh(sn::Resources::Find<sn::Mesh>(L"RectMesh"));
			mr->SetMaterial(sn::Resources::Find<sn::Material>(L"GridMaterial"));
			sn::GridScript* gridSc = grid->AddComponent<sn::GridScript>();
			gridSc->SetCamera(renderer::cameras[0]);

			mEditorObjects.push_back(grid);
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		//io.ConfigViewportsNoDefaultParent = true;
		//io.ConfigDockingAlwaysTabBar = true;
		//io.ConfigDockingTransparentPayload = true;
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(application.GetHwnd());
		ImGui_ImplDX11_Init(sn::graphics::GetDevice()->GetID3D11Device()
			, sn::graphics::GetDevice()->GetID3D11DeviceContext());

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);

		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		bool show_demo_window = true;

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		//for (auto& widget : mWidgets)
		//{
		//	widget.second->Update();
		//}
		//for (auto& widget : mWidgets)
		//{
		//	widget.second->Render();
		//}

#pragma region SAMPLE
		//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		//{
		//	static float f = 0.0f;
		//	static int counter = 0;

		//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//	ImGui::Checkbox("Another Window", &show_another_window);

		//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//		counter++;
		//	ImGui::SameLine();
		//	ImGui::Text("counter = %d", counter);

		//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		//	ImGui::End();
		//}

		//// 3. Show another simple window.
		//if (show_another_window)
		//{
		//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//	ImGui::Text("Hello from another window!");
		//	if (ImGui::Button("Close Me"))
		//		show_another_window = false;
		//	ImGui::End();
		//}
#pragma endregion

		// Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}


		//������

		//Microsoft::WRL::ComPtr<ID3D11DepthStencilState> ds
		//	= renderer::depthStencilStates[(UINT)sn::graphics::eDSType::Less];
		//sn::graphics::GetDevice()->BindDepthStencilState(ds.Get());

		//for (EditorObject* obj : mEditorObjects)
		//{
		//	obj->Render();
		//}

		//for (const sn::graphics::DebugMesh& mesh
		//	: renderer::debugMeshs)
		//{
		//	DebugRender(mesh);
		//}
		//renderer::debugMeshs.clear();
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

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void Editor::DebugRender(const sn::graphics::DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];

		// ��ġ ũ�� ȸ�� ������ �޾ƿͼ�
		// �ش� ���ӿ�����Ʈ���� �׷��ָ�ȴ�.
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

		//���� �� ������ �ּ��� �����ϸ� �浹ü�� Camera�� ����ٴ�. �ּ��� ġ�� �浹ü�� ������Ʈ�� �پ� ����. �ʿ��� �� �˾Ƽ� �� ��.
			sn::Camera::SetGpuViewMatrix(mainCamara->GetViewMatrix());
			sn::Camera::SetGpuProjectionMatrix(mainCamara->GetProjectionMatrix());

		//�浹ü ���� ������
		renderer::EditorCB editorCB = {};
		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Editor];
		if (mesh.hit == true) {
			editorCB.ColliderColor = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
			cb->SetData(&editorCB);
		}
		else if(mesh.hit == false) {
			editorCB.ColliderColor = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
			cb->SetData(&editorCB);
		}
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
		
		debugObj->Render();

	}
}
