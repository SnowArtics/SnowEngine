#include "snRenderer.h"
#include "snConstantBuffer.h"
#include "snResources.h"
#include "snTexture.h"

namespace renderer {
	using namespace sn;
	using namespace sn::graphics;

	Vertex vertexes[4] = {}; //이렇게 만든 정보를 GPU에 넘겨줘야한다. 이를 위해 버텍스 버퍼를 생성.

	//Vertex Buffer
	////이걸 사용해서 전달을 한다. 그래서 일단 전달을 했어! 어떻게 처리할꺼야
	////위에서 만든 정보를 가공해줘야하는데, 이를 위해 버텍스 쉐이더 과정을 거친다.
	//ID3D11Buffer* triangleBuffer = nullptr; 
	////인덱스 버퍼를 사용하기 위한 인덱스 버퍼를 만든다.
	//ID3D11Buffer* triangleIdxBuffer = nullptr;
	
	//VertexBuffer
	sn::Mesh* mesh = nullptr;
	sn::Shader* shader = nullptr;
	//셰이더에 데이터를 전달해 주기 위한 상수 버퍼를 만든다.
	sn::graphics::ConstantBuffer* constantBuffer = nullptr;

	Vector4 constantBufferPos;

	////Error Blob
	//ID3DBlob* errorBlob = nullptr;//혹시 만들다 에러 뜨면 여기에 전달된다.

	////Vertex Shader code -> Binary Code로 바꿔서 우리가 사용할 수 있게 만들어준다.
	////HLSL코드가 컴파일된 결과물이 여기 들어간다.
	//ID3DBlob* triangleVSBlob = nullptr;

	////Blob을 통햄 만들어진
	////Vertex Shader가 여기 저장된다.
	//ID3D11VertexShader* triangleVSShader = nullptr;

	////Pixel Shader code -> Binary Code 위와 마찬가지
	//ID3DBlob* trianglePSBlob = nullptr;

	void SetupState() {
		// Input layout 정점 구조 정보를 넘겨줘야한다.
		D3D11_INPUT_ELEMENT_DESC arrLayout[3] = {};

		arrLayout[0].AlignedByteOffset = 0;
		arrLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[0].InputSlot = 0;
		arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[0].SemanticName = "POSITION";
		arrLayout[0].SemanticIndex = 0;

		arrLayout[1].AlignedByteOffset = 12;
		arrLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayout[1].InputSlot = 0;
		arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[1].SemanticName = "COLOR";
		arrLayout[1].SemanticIndex = 0;

		arrLayout[2].AlignedByteOffset = 28;
		arrLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayout[2].InputSlot = 0;
		arrLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[2].SemanticName = "TEXCOORD";
		arrLayout[2].SemanticIndex = 0;

		sn::graphics::GetDevice()->CreateInputLayout(arrLayout, 3
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());
	}

	void LoadBuffer()
	{
		// Vertex Buffer
		mesh = new sn::Mesh();
		mesh->CreateVertexBuffer(vertexes, 4);

		//여기서 인덱스 버퍼를 초기화해준다.
		//인덱스 버퍼는 그리는 순서를 전달해 주는 것이므로, 정수 데이터만 보내면 된다.
		//버텍스 버퍼는 속성이 여러가지니까 구조체를 보내줌
		std::vector<UINT> indexes = {};
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		// Index Buffer
		mesh->CreateIndexBuffer(indexes.data(), indexes.size());

		//상수버퍼는 서브리소스 데이터를 생성하지 않는다.
		
		constantBuffer = new sn::graphics::ConstantBuffer(sn::graphics::eCBType::Transform);
		constantBuffer->Create(sizeof(Vector4));
				
	}

	void LoadShader()
	{
		shader = new sn::Shader();
		shader->Create(sn::graphics::eShaderStage::VS, L"TriangleVS.hlsl", "main");
		shader->Create(sn::graphics::eShaderStage::PS, L"TrianglePS.hlsl", "main");
	}

	void Initialize()
	{
		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		vertexes[0].uv = Vector2(0.0f, 0.0f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		LoadBuffer();
		LoadShader();
		SetupState();

		Texture* texture
			= Resources::Load<Texture>(L"Smile", L"..\\Resources\\Texture\\Smile.png");

		texture->BindShader(eShaderStage::PS, 0);
	}

	void Release()
	{
		//버퍼, Blob, Shader 관련 객체 포인터들은 다이렉트X가 제공해주는 Release() 함수가 있다. 그걸 쓴다.		
		delete mesh;
		delete shader;
		delete constantBuffer;
	}
}