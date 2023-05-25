#include "snRenderer.h"

namespace sn::renderer {
	Vertex vertexes[3] = {}; //이렇게 만든 정보를 GPU에 넘겨줘야한다. 이를 위해 버텍스 버퍼를 생성.
	//Vertex Buffer
	ID3D11Buffer* triangleBuffer = nullptr; //이걸 사용해서 전달을 한다. 그래서 일단 전달을 했어! 어떻게 처리할꺼야
	//위에서 만든 정보를 가공해줘야하는데, 이를 위해 버텍스 쉐이더 과정을 거친다.

	//Error Blob
	ID3DBlob* errorBlob = nullptr;//혹시 만들다 에러 뜨면 여기에 전달된다.

	//Vertex Shader code -> Binary Code로 바꿔서 우리가 사용할 수 있게 만들어준다.
	//HLSL코드가 컴파일된 결과물이 여기 들어간다.
	ID3DBlob* triangleVSBlob = nullptr;

	//Blob을 통햄 만들어진
	//Vertex Shader가 여기 저장된다.
	ID3D11VertexShader* triangleVSShader = nullptr;

	//Pixel Shader code -> Binary Code 위와 마찬가지
	ID3DBlob* trainglePSBlob = nullptr;

	//Pixel Shader
	ID3D11PixelShader* trianglePSShader = nullptr;


	void SetupState() {

	}

	void LoadBuffer()
	{
		//버퍼를 만들어줘야 하는데, 버퍼로 넘겨주기위해 들어가는 전체 Vertex의 크기를 지정해줘야한다.
		D3D11_BUFFER_DESC triangleDesc = {};
		triangleDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		triangleDesc.ByteWidth = sizeof(Vertex) * 3;;//도형의 전체 버텍스의 크기를 넘겨준다.
		triangleDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;//나는 버텍스 버퍼야
		//CPU가 써서 넘겨주기 때문에, 수정할 수 있게 설정해준다.	
		triangleDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

		//CreateTexture할때 data를 넘겨줄 수 있는데 그 때 넘겨줄려고 설정해놓는다.
		D3D11_SUBRESOURCE_DATA triangleData = {};
		triangleData.pSysMem = vertexes;
		//얘가 버퍼 생성을 호출해준다.
		sn::graphics::GetDevice()->CreateBuffer(&triangleBuffer, &triangleDesc, &triangleData);

	}

	void LoadShader()
	{
		sn::graphics::GetDevice()->CreateShader();
	}

	void Initialize()
	{
		vertexes[0].pos = Vector3(0.0f, 0.5f, 1.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[1].pos = Vector3(0.5f, -0.5f, 1.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(-0.5f, -0.5f, 1.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		SetupState();
		LoadBuffer();
		LoadShader();
	}
}