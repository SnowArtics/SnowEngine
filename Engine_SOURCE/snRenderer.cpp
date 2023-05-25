#include "snRenderer.h"

namespace sn::renderer {
	Vertex vertexes[3] = {}; //�̷��� ���� ������ GPU�� �Ѱ�����Ѵ�. �̸� ���� ���ؽ� ���۸� ����.
	//Vertex Buffer
	ID3D11Buffer* triangleBuffer = nullptr; //�̰� ����ؼ� ������ �Ѵ�. �׷��� �ϴ� ������ �߾�! ��� ó���Ҳ���
	//������ ���� ������ ����������ϴµ�, �̸� ���� ���ؽ� ���̴� ������ ��ģ��.

	//Error Blob
	ID3DBlob* errorBlob = nullptr;//Ȥ�� ����� ���� �߸� ���⿡ ���޵ȴ�.

	//Vertex Shader code -> Binary Code�� �ٲ㼭 �츮�� ����� �� �ְ� ������ش�.
	//HLSL�ڵ尡 �����ϵ� ������� ���� ����.
	ID3DBlob* triangleVSBlob = nullptr;

	//Blob�� ���� �������
	//Vertex Shader�� ���� ����ȴ�.
	ID3D11VertexShader* triangleVSShader = nullptr;

	//Pixel Shader code -> Binary Code ���� ��������
	ID3DBlob* trainglePSBlob = nullptr;

	//Pixel Shader
	ID3D11PixelShader* trianglePSShader = nullptr;


	void SetupState() {

	}

	void LoadBuffer()
	{
		//���۸� �������� �ϴµ�, ���۷� �Ѱ��ֱ����� ���� ��ü Vertex�� ũ�⸦ ����������Ѵ�.
		D3D11_BUFFER_DESC triangleDesc = {};
		triangleDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		triangleDesc.ByteWidth = sizeof(Vertex) * 3;;//������ ��ü ���ؽ��� ũ�⸦ �Ѱ��ش�.
		triangleDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;//���� ���ؽ� ���۾�
		//CPU�� �Ἥ �Ѱ��ֱ� ������, ������ �� �ְ� �������ش�.	
		triangleDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

		//CreateTexture�Ҷ� data�� �Ѱ��� �� �ִµ� �� �� �Ѱ��ٷ��� �����س��´�.
		D3D11_SUBRESOURCE_DATA triangleData = {};
		triangleData.pSysMem = vertexes;
		//�갡 ���� ������ ȣ�����ش�.
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