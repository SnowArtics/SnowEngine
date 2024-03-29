#pragma once
#include "SnowEngine.h"
#include "snGraphicDevice_Dx11.h"
#include "snScene.h"

namespace sn
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		void Initialize();
		void Update();
		void LateUpdate();
		void Render();
		void Destroy();
		void ChangeScene();
		void ChangeAIState();
		void Present();

		void SetWindow(HWND hwnd, UINT width, UINT height);


		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
		HWND GetHwnd() { return mHwnd; }

	private:
		bool mbInitialize = false;
		// 오로지 한개의 객체만 만들수 있는 스마트 포인터
		std::unique_ptr<sn::graphics::GraphicDevice_Dx11> graphicDevice;

		// HDC mHdc; -> GPU API
		HWND mHwnd;
		UINT mWidth;
		UINT mHeight;		
	};
}
