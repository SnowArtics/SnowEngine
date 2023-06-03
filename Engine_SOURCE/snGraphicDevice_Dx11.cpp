#include "snGraphicDevice_Dx11.h"
#include "snApplication.h"
#include "snRenderer.h"

#include "snInput.h"
#include "snTime.h"

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
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.CPUAccessFlags = 0;

		depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.Width = application.GetWidth();
		depthStencilDesc.Height = application.GetHeight();
		depthStencilDesc.ArraySize = 1;

		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

		depthStencilDesc.MipLevels = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		if (!CreateTexture(&depthStencilDesc, &data))
			return;

		//우리가 실행하는 곳은 결국 데스크탑의 화면이고 WinAPI의 윈도우 이기 떄문에 해당 정보값을
		//들고 와야한다.
		RECT winRect = {};
		GetClientRect(hWnd, &winRect);

		//typedef struct D3D11_VIEWPORT
		//{
		//	FLOAT TopLeftX;
		//	FLOAT TopLeftY;
		//	FLOAT Width;
		//	FLOAT Height;
		//	FLOAT MinDepth;
		//	FLOAT MaxDepth;
		//} 	D3D11_VIEWPORT;

		//마지막 두개의 파라미터는 깊이값을 말한다. DX는 0~1이므로 밑에처럼 넣어준다.
		mViewPort =
		{
			0.0f, 0.0f
			, (float)(winRect.right - winRect.left)
			, (float)(winRect.bottom - winRect.top)
			, 0.0f, 1.0f
		};

		//이렇게 들고온 윈도우 해상도 관련 정보값을 GPU객체인 mDevice랑 묶어준다.
		BindViewPort(&mViewPort);

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
			, 0, 0, &renderer::triangleVSBlob/*결과물 코드 저장 장소*/
			, &renderer::errorBlob/*error를 저장할 장소*/);

		//만약 errorBlob이 채워진다면 에러가 있다는 소리 에러가 있으면, 알아낼 방법이 있음
		//바로 이렇게 코드 짜면 됨.
		if (renderer::errorBlob)
		{
			OutputDebugStringA((char*)renderer::errorBlob->GetBufferPointer());
			renderer::errorBlob->Release();
			assert(false);
		}

		mDevice->CreateVertexShader(renderer::triangleVSBlob->GetBufferPointer()
			//코드 넣어줘야함. 근데 그냥 코드는 Byte코드라서 맞춰서 포인터 넣어줘야함
			, renderer::triangleVSBlob->GetBufferSize()//버퍼 길이 넣어줘야함
			, nullptr, &renderer::triangleVSShader);


		std::filesystem::path psPath(shaderPath.c_str());
		psPath += L"TrianglePS.hlsl";

		D3DCompileFromFile(psPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, "main", "ps_5_0", 0, 0, &renderer::trianglePSBlob, &renderer::errorBlob);

		if (renderer::errorBlob)
		{
			OutputDebugStringA((char*)renderer::errorBlob->GetBufferPointer());
			renderer::errorBlob->Release();
		}

		mDevice->CreatePixelShader(renderer::trianglePSBlob->GetBufferPointer()
			, renderer::trianglePSBlob->GetBufferSize()
			, nullptr, &renderer::trianglePSShader);

		// Input layout 정점 구조 정보를 넘겨줘야한다.
		D3D11_INPUT_ELEMENT_DESC arrLayout[2] = {}; //우리의 정점정보는 2개이다. 위치와, 정보

		arrLayout[0].AlignedByteOffset = 0;//첫번째 정점데이터의 시작 위치를 말한다.
		arrLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;//정점중에서도 첫번째 위치 값인 Pos의 크기를 맞춤
		arrLayout[0].InputSlot = 0;//입력을 여러개를 받을 수 있는데 그 때 설정한다고 함. 지금은 안함
		arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;//나는 정점데이터야 라고 알려줌
		arrLayout[0].SemanticName = "POSITION";//나는 정점데이터야 라고 알려줌
		arrLayout[0].SemanticIndex = 0;//Semantic이름이 겹칠 때 구분할려고 사용한다.
		//여기까지의 정보는 0번에서 12바이트까지는 정점데이터고 sementic이름은 POSITION이라는 것이다.
		//이걸 위에꺼 포함 2번한다.

		arrLayout[1].AlignedByteOffset = 12;//위치값입력이 끝나고 색값 정점데이터의 시작 위치를 말한다
		arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//총 16바이트로 이루어져 있다
		arrLayout[1].InputSlot = 0;//위와 마찬가지
		arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;//VertexData라고 알려준다.
		arrLayout[1].SemanticName = "COLOR";//Sementic의 이름은 COLOR이다.
		arrLayout[1].SemanticIndex = 0;//위와 마찬가지

		//이제 이걸로 InputLayout을 생성한다.
		mDevice->CreateInputLayout(arrLayout, 2//우리가 만든 DESC를 넣고, 크기를 넣어준다.
			, renderer::triangleVSBlob->GetBufferPointer()//InputLayout은 특정 셰이더와 연결해야 함으로 넣어준다.
			, renderer::triangleVSBlob->GetBufferSize()//셰이더 코드의 길이를 넣는다.
			, &renderer::triangleLayout);//이후 triangleLayout에 저장된다.

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

	void GraphicDevice_Dx11::BindViewPort(D3D11_VIEWPORT* viewPort)
	{
		mContext->RSSetViewports(1, viewPort);
	}

	void GraphicDevice_Dx11::SetConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size)
	{
		//Context에서 생성한 Map을 MAPPED_SUBRESOURCE에 묶어준다.
		D3D11_MAPPED_SUBRESOURCE subRes = {};
		mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);

		//이렇게 하면, 우리가 만든 상수버퍼랑(buffer)랑 subRes랑 연결이 된것이다.

		//subRes의 데이터에 우리가 만든 데이터를 할당해준다.
		memcpy(subRes.pData, data, size);

		//subRes는 지역변수임으로 이 함수가 끝나면 사라진다. 그래서 Unmap 해주자.
		mContext->Unmap(buffer, 0);
	}

	void GraphicDevice_Dx11::BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
	{
		//스위치 케이스로 셰이더에 맞게 만든 상수 데이터를 넘겨준다.
		//지금은 넘겨주는게 다 똑같다.
		//이 코드에서 알 수 있는 점은 상수버퍼는 어디든지 다 넘겨줄 수 있다는 것이다!

		switch (stage)
		{
		case eShaderStage::VS:
			mContext->VSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::HS:
			mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::DS:
			mContext->DSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::GS:
			mContext->GSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::PS:
			mContext->PSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::CS:
			mContext->CSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case eShaderStage::End:
			break;
		default:
			break;
		}
	}

	void GraphicDevice_Dx11::BindsConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
	{
		mContext->VSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->DSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->GSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->PSSetConstantBuffers((UINT)type, 1, &buffer);
		mContext->CSSetConstantBuffers((UINT)type, 1, &buffer);
	}

	void GraphicDevice_Dx11::Draw()
	{
		//삼각형을 움직이기 위한 코드
		//끊겨서 움직임
		//if (sn::Input::GetKeyDown(sn::eKeyCode::W)) {
		//	renderer::constantBufferPos += Vector4(0.0f, 0.1f, 0.0f, 0.0f);
		//}else if (sn::Input::GetKeyDown(sn::eKeyCode::S)) {
		//	renderer::constantBufferPos += Vector4(0.0f, -0.1f, 0.0f, 0.0f);
		//}
		//else if (sn::Input::GetKeyDown(sn::eKeyCode::A)) {
		//	renderer::constantBufferPos += Vector4(-0.1f, 0.0f, 0.0f, 0.0f);
		//}
		//else if (sn::Input::GetKeyDown(sn::eKeyCode::D)) {
		//	renderer::constantBufferPos += Vector4(0.1f, 0.0f, 0.0f, 0.0f);
		//}

		//이어서 움직임
		if (sn::Input::GetKey(sn::eKeyCode::W)) {
			renderer::constantBufferPos += Vector4(0.0f, 0.2f, 0.0f, 0.0f) * sn::Time::DeltaTime();
		}
		if (sn::Input::GetKey(sn::eKeyCode::S)) {
			renderer::constantBufferPos += Vector4(0.0f, -0.2f, 0.0f, 0.0f) * sn::Time::DeltaTime();
		}
		if (sn::Input::GetKey(sn::eKeyCode::A)) {
			renderer::constantBufferPos += Vector4(-0.2f, 0.0f, 0.0f, 0.0f) * sn::Time::DeltaTime();
		}
		if (sn::Input::GetKey(sn::eKeyCode::D)) {
			renderer::constantBufferPos += Vector4(0.2f, 0.0f, 0.0f, 0.0f) * sn::Time::DeltaTime();
		}
		
		SetConstantBuffer(renderer::triangleConstantBuffer, &renderer::constantBufferPos, sizeof(Vector4));
		BindConstantBuffer(eShaderStage::VS, eCBType::Transform, renderer::triangleConstantBuffer);

		// render target clear
		//ClearRenderTargetView()한다음에 DepthStencilView도 Clear를 해줘야 한다.
		//DepthStenciView도 텍스쳐를 들고오는거다! 그래서 이전꺼 Clear를 해줘야 한다!!
		FLOAT bgColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), bgColor);
		mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

		//우리 컴퓨터에 그리는 것임으로 뷰포트 변환작업을 한 번 거쳐줘야한다.
		//세팅된 뷰포트작업을 여기서 가져와서 실행해준다.		
		// viewport update
		HWND hWnd = application.GetHwnd();
		RECT winRect = {};
		GetClientRect(hWnd, &winRect);
		mViewPort =
		{
			0.0f, 0.0f
			, (float)(winRect.right - winRect.left)
			, (float)(winRect.bottom - winRect.top)
			, 0.0f, 1.0f
		};

		BindViewPort(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

		// input assembler 정점데이터 정보 지정
		UINT vertexsize = sizeof(renderer::Vertex);
		UINT offset = 0;

		//여기서 우리가 세팅해준 정점버퍼를 InputAssembler한테 넘겨준다.
		mContext->IASetVertexBuffers(0, 1, &renderer::triangleBuffer, &vertexsize, &offset);
		//우리가 만든 IndexBuffer를 InputAssembler할 때, Vertex Buffer랑 같이 전달해준다.
		mContext->IASetIndexBuffer(renderer::triangleIdxBuffer, DXGI_FORMAT_R32_UINT, 0);

		mContext->IASetInputLayout(renderer::triangleLayout);
		//밑의 함수는 삼각형을 어떻게 생성할것인지 지정한다.
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//이제 버텍스 셰이더랑 픽셀 셰이더를 묶어준다.
		//Bind VS, PS 
		mContext->VSSetShader(renderer::triangleVSShader, 0, 0);
		mContext->PSSetShader(renderer::trianglePSShader, 0, 0);

		int vertexesSize = renderer::getVertexesSize();

		//int vertexesSize = sizeof(renderer::vertexes) / sizeof(renderer::vertexes[0]);

		//이제 렌더타겟에 그려준다.
		//mContext->Draw(vertexesSize, 0);
		mContext->DrawIndexed(6, 0, 0);

		// 레더타겟에 있는 이미지를 화면에 그려준다
		mSwapChain->Present(0, 0);
	}

}
