#include "snRenderer.h"

#define M_PI 3.14159265358979323846

namespace sn::renderer {
	Vertex vertexes[3] = {}; //이렇게 만든 정보를 GPU에 넘겨줘야한다. 이를 위해 버텍스 버퍼를 생성.

	// Input Layout (정점 정보)
	ID3D11InputLayout* triangleLayout = nullptr;

	//Vertex Buffer
	//이걸 사용해서 전달을 한다. 그래서 일단 전달을 했어! 어떻게 처리할꺼야
	//위에서 만든 정보를 가공해줘야하는데, 이를 위해 버텍스 쉐이더 과정을 거친다.
	ID3D11Buffer* triangleBuffer = nullptr; 
	//인덱스 버퍼를 사용하기 위한 인덱스 버퍼를 만든다.
	ID3D11Buffer* triangleIdxBuffer = nullptr;
	//셰이더에 데이터를 전달해 주기 위한 상수 버퍼를 만든다.
	ID3D11Buffer* triangleConstantBuffer = nullptr;

	//Error Blob
	ID3DBlob* errorBlob = nullptr;//혹시 만들다 에러 뜨면 여기에 전달된다.

	//Vertex Shader code -> Binary Code로 바꿔서 우리가 사용할 수 있게 만들어준다.
	//HLSL코드가 컴파일된 결과물이 여기 들어간다.
	ID3DBlob* triangleVSBlob = nullptr;

	//Blob을 통햄 만들어진
	//Vertex Shader가 여기 저장된다.
	ID3D11VertexShader* triangleVSShader = nullptr;

	//Pixel Shader code -> Binary Code 위와 마찬가지
	ID3DBlob* trianglePSBlob = nullptr;

	//Pixel Shader
	ID3D11PixelShader* trianglePSShader = nullptr;


	void SetupState() {

	}

	void LoadBuffer()
	{
		//버퍼를 만들어줘야 하는데, 버퍼로 넘겨주기위해 들어가는 전체 Vertex의 크기를 지정해줘야한다.
		D3D11_BUFFER_DESC triangleDesc = {};
		triangleDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		triangleDesc.ByteWidth = sizeof(Vertex) * (sizeof(vertexes)/sizeof(vertexes[0]));//도형의 전체 버텍스의 크기를 넘겨준다.
		triangleDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;//나는 버텍스 버퍼야
		//CPU가 써서 넘겨주기 때문에, 수정할 수 있게 설정해준다.	
		triangleDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

		//CreateTexture할때 data를 넘겨줄 수 있는데 그 때 넘겨줄려고 설정해놓는다.
		D3D11_SUBRESOURCE_DATA triangleData = {};
		triangleData.pSysMem = vertexes;
		//얘가 버퍼 생성을 호출해준다.
		sn::graphics::GetDevice()->CreateBuffer(&triangleBuffer, &triangleDesc, &triangleData);


		//여기서 인덱스 버퍼를 초기화해준다.
		//인덱스 버퍼는 그리는 순서를 전달해 주는 것이므로, 정수 데이터만 보내면 된다.
		//버텍스 버퍼는 속성이 여러가지니까 구조체를 보내줌
		std::vector<UINT> indexes = {};
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		// Index Buffer
		D3D11_BUFFER_DESC triangleIdxDesc = {};
		triangleIdxDesc.ByteWidth = sizeof(UINT) * indexes.size();
		triangleIdxDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		triangleIdxDesc.Usage = D3D11_USAGE_DEFAULT;
		triangleIdxDesc.CPUAccessFlags = 0;

		//그냥 인덱스 버퍼를 전달해줄 수 없고 서브리소스 데이터에 추가해줘서 넘겨줬다
		D3D11_SUBRESOURCE_DATA triangleIdxData = {};
		triangleIdxData.pSysMem = indexes.data();
		sn::graphics::GetDevice()->CreateBuffer(&triangleIdxBuffer, &triangleIdxDesc, &triangleIdxData);

		// Constant Buffer
		//상수버퍼를 생성해주자.
		D3D11_BUFFER_DESC triangleCSDesc = {};
		triangleCSDesc.ByteWidth = sizeof(Vector4);//상수버퍼는 생성해줄때 규칙이 있어서 무조건 Vector4 단위로 생성해야 한다.
		triangleCSDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;//상수 버퍼다.
		triangleCSDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;//바뀔 수 있음으로 DYNAMIC으로 설정해준다.
		triangleCSDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//CPU가 수정해서 GPU에 넘겨주므로 이렇게 해야함.

		//상수버퍼는 서브리소스 데이터를 생성하지 않는다.

		sn::graphics::GetDevice()->CreateBuffer(&triangleConstantBuffer, &triangleCSDesc, nullptr);

		//여기까지는 버퍼를 만들기만 했다!
		//상수 버퍼는 GPU상에 데이터를 넘겨주어야 함으로 추가적인 작업을 더 해줘야 한다.
		//1. 상수의 데이터를 만들어주고 cpu에 있는 데이터를 GPU에 묶어줘야 하며(MAP, UNMAP)
		//2. 묶어준 데이터를 렌더링 파이프라인 셰이더에 보내줘야 한다.
		Vector4 pos(0.3f, 0.0f, 0.0f, 1.0f);
		sn::graphics::GetDevice()->SetConstantBuffer(triangleConstantBuffer, &pos, sizeof(Vector4));
		sn::graphics::GetDevice()->BindConstantBuffer(eShaderStage::VS, eCBType::Transform, triangleConstantBuffer);

	}

	void LoadShader()
	{
		sn::graphics::GetDevice()->CreateShader();
	}

	int getVertexesSize()
	{
		return  (sizeof(vertexes) / sizeof(vertexes[0]));
	}

	void Initialize()
	{
		vertexes[0].pos = Vector3(0.0f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[1].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		SetupState();
		LoadBuffer();
		LoadShader();
	}
}