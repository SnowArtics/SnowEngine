#include "guiGameView.h"

#include "..\\Engine_SOURCE\\snApplication.h"
#include "..\\Engine_SOURCE\\snTexture.h"
#include "..\\Engine_SOURCE\\snResources.h"
#include "..\\Engine_SOURCE\\snGraphicDevice_Dx11.h"
#include "..\\Engine_SOURCE\\snRenderer.h"

namespace gui
{
	GameView::GameView()
	{
	}
	GameView::~GameView()
	{
	}
	void GameView::FixedUpdate()
	{
	}
	void GameView::Update()
	{
		std::shared_ptr<sn::graphics::Texture> renderTarget
			= sn::Resources::Find<sn::graphics::Texture>(L"RenderTarget");

		std::shared_ptr<sn::graphics::Texture> gameTarget
			= std::make_shared<sn::graphics::Texture>();

		D3D11_VIEWPORT viewPort = sn::graphics::GetDevice()->GetViewPort();
		gameTarget->Create(viewPort.Width, viewPort.Height
			, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

		ID3D11ShaderResourceView* srv = nullptr;
		sn::graphics::GetDevice()->BindShaderResource(sn::eShaderStage::PS, 61, &srv);

		sn::graphics::GetDevice()->CopyResource(gameTarget->GetTexture().Get()
			, renderTarget->GetTexture().Get());

		gameTarget->BindShaderResource(sn::eShaderStage::PS, 61);

		ImGuiIO io = ImGui::GetIO();
		ImVec2 pannelSize = ImGui::GetWindowSize();
		ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec2 scene_size = ImVec2(io.DisplaySize.x, io.DisplaySize.y);

		ImGui::Image(gameTarget->GetSRV().Get(), pannelSize
			, ImVec2(0, 0), ImVec2(2, 2), tintColor, ImVec4(0, 0, 0, 0));

	}
	void GameView::LateUpdate()
	{
	}
	void GameView::Render()
	{
		Widget::Render();
	}
}
