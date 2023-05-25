#include "snGraphicDevice_Dx11.h"
#include "snApplication.h"
#include "snRenderer.h"

extern sn::Application application;

namespace sn::graphics
{
	GraphicDevice_Dx11::GraphicDevice_Dx11()
	{
		// Device, Context 생성
		HWND hWnd = application.GetHwnd();
		UINT deviceFlag = D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel = (D3D_FEATURE_LEVEL)0;
		
		D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
			, deviceFlag, nullptr, 0
			, D3D11_SDK_VERSION
			, mDevice.GetAddressOf(), &featureLevel
			, mContext.GetAddressOf());

		// SwapChain
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = application.GetWidth();
		swapChainDesc.BufferDesc.Height = application.GetHeight();

		if (!CreateSwapChain(&swapChainDesc, hWnd))
			return;

		// get rendertarget by swapchain
		if (FAILED(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D)
			, (void**)mRenderTarget.GetAddressOf())))
			return;

		// create rendertarget view
		mDevice->CreateRenderTargetView((ID3D11Resource*)mRenderTarget.Get()
			, nullptr, mRenderTargetView.GetAddressOf());

		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.Width = application.GetWidth();
		depthStencilDesc.Height = application.GetHeight();
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		if (!CreateTexture(&depthStencilDesc, &data))
			return;

		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	}

	GraphicDevice_Dx11::~GraphicDevice_Dx11()
	{
	}

	bool GraphicDevice_Dx11::CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* desc, HWND hWnd)
	{
		DXGI_SWAP_CHAIN_DESC dxgiDesc = {};

		dxgiDesc.OutputWindow = hWnd;
		dxgiDesc.Windowed = true;
		dxgiDesc.BufferCount = desc->BufferCount;
		dxgiDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

		dxgiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxgiDesc.BufferDesc.Width = desc->BufferDesc.Width;
		dxgiDesc.BufferDesc.Height = desc->BufferDesc.Height;
		dxgiDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgiDesc.BufferDesc.RefreshRate.Numerator = 240;
		dxgiDesc.BufferDesc.RefreshRate.Denominator = 1;
		dxgiDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
		dxgiDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		dxgiDesc.SampleDesc.Count = 1;
		dxgiDesc.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory = nullptr;

		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
			return false;

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
			return false;

		if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
			return false;

		if (FAILED(pFactory->CreateSwapChain(mDevice.Get(), &dxgiDesc, mSwapChain.GetAddressOf())))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateBuffer(ID3D11Buffer** buffer, D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data)
	{
		//D3D11_BUFFER_DESC triangleDesc = {};
		//triangleDesc.ByteWidth = desc->ByteWidth;
		//triangleDesc.BindFlags = desc->BindFlags;
		//triangleDesc.CPUAccessFlags = desc->CPUAccessFlags;


		/*D3D11_SUBRESOURCE_DATA triangleData = {};
		triangleData.pSysMem = vertexes;*/

		if (FAILED(mDevice->CreateBuffer(desc, data, buffer)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateShader()
	{
		///* [annotation] */
		//_In_reads_(BytecodeLength)  const void* pShaderBytecode,
		//	/* [annotation] */
		//	_In_  SIZE_T BytecodeLength,
		//	/* [annotation] */
		//	_In_opt_  ID3D11ClassLinkage* pClassLinkage,
		//	/* [annotation] */
		//	_COM_Outptr_opt_  ID3D11VertexShader** ppVertexShader

		ID3DBlob* vsBlob = nullptr;
		//HLSL코드를 읽어와야 한다. 경로 들고오자
		//경로를 이렇게 수동적으로 들고와야하는 이유는 윈도우의 프로젝트로 만든게 아니고
		//공유소스프로젝트라서 이렇게 들고와야한다.
		//시스템상으로 들고오고 싶으면 프로젝트 안의 참조안에 추가해야하는데,
		//공유소스프로젝트는 서로 들고올수가없음.
		std::filesystem::path shaderPath = std::filesystem::current_path().parent_path();
		shaderPath += L"\\Shader_SOURCE\\";

		std::filesystem::path vsPath(shaderPath.c_str());
		vsPath += L"TriangleVS.hlsl";

		//HLSL파일의 컴파일 결과물을 triangleVSBlob에 저장한다.
		D3DCompileFromFile(vsPath.c_str()/*경로*/, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main"/*shader파일에서 사용할 함수 이름*/, "vs_5_0"/*쉐이더컴파일 버전*/
			, 0, 0, &sn::renderer::triangleVSBlob/*결과물 코드 저장 장소*/
			, &sn::renderer::errorBlob/*error를 저장할 장소*/);

		//만약 errorBlob이 채워진다면 에러가 있다는 소리 에러가 있으면, 알아낼 방법이 있음
		//바로 이렇게 코드 짜면 됨.
		if (sn::renderer::errorBlob)
		{
			OutputDebugStringA((char*)sn::renderer::errorBlob->GetBufferPointer());
			sn::renderer::errorBlob->Release();
			assert(false);
		}

		mDevice->CreateVertexShader(sn::renderer::triangleVSBlob->GetBufferPointer()
			//코드 넣어줘야함. 근데 그냥 코드는 Byte코드라서 맞춰서 포인터 넣어줘야함
			, sn::renderer::triangleVSBlob->GetBufferSize()//버퍼 길이 넣어줘야함
			, nullptr, &sn::renderer::triangleVSShader);

		return true;
	}

	bool GraphicDevice_Dx11::CreateTexture(const D3D11_TEXTURE2D_DESC* desc, void* data)
	{
		D3D11_TEXTURE2D_DESC dxgiDesc = {};
		dxgiDesc.BindFlags = desc->BindFlags;
		dxgiDesc.Usage = desc->Usage;
		dxgiDesc.CPUAccessFlags = 0;

		dxgiDesc.Format = desc->Format;
		dxgiDesc.Width = desc->Width;
		dxgiDesc.Height = desc->Height;
		dxgiDesc.ArraySize = desc->ArraySize;

		dxgiDesc.SampleDesc.Count = desc->SampleDesc.Count;
		dxgiDesc.SampleDesc.Quality = 0;

		dxgiDesc.MipLevels = desc->MipLevels;
		dxgiDesc.MiscFlags = desc->MiscFlags;

		if (FAILED(mDevice->CreateTexture2D(&dxgiDesc, nullptr, mDepthStencilBuffer.ReleaseAndGetAddressOf())))
			return false;

		if (FAILED(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf())))
			return false;

		return true;
	}

	void GraphicDevice_Dx11::Draw()
	{
		FLOAT bgColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), bgColor);

		// 

		mSwapChain->Present(0, 0);
	}

}
