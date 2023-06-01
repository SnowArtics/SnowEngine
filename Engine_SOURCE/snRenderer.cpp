#include "snRenderer.h"

#define M_PI 3.14159265358979323846

namespace sn::renderer {
	Vertex vertexes[3] = {}; //�̷��� ���� ������ GPU�� �Ѱ�����Ѵ�. �̸� ���� ���ؽ� ���۸� ����.

	// Input Layout (���� ����)
	ID3D11InputLayout* triangleLayout = nullptr;

	//Vertex Buffer
	//�̰� ����ؼ� ������ �Ѵ�. �׷��� �ϴ� ������ �߾�! ��� ó���Ҳ���
	//������ ���� ������ ����������ϴµ�, �̸� ���� ���ؽ� ���̴� ������ ��ģ��.
	ID3D11Buffer* triangleBuffer = nullptr; 
	//�ε��� ���۸� ����ϱ� ���� �ε��� ���۸� �����.
	ID3D11Buffer* triangleIdxBuffer = nullptr;
	//���̴��� �����͸� ������ �ֱ� ���� ��� ���۸� �����.
	ID3D11Buffer* triangleConstantBuffer = nullptr;

	//Error Blob
	ID3DBlob* errorBlob = nullptr;//Ȥ�� ����� ���� �߸� ���⿡ ���޵ȴ�.

	//Vertex Shader code -> Binary Code�� �ٲ㼭 �츮�� ����� �� �ְ� ������ش�.
	//HLSL�ڵ尡 �����ϵ� ������� ���� ����.
	ID3DBlob* triangleVSBlob = nullptr;

	//Blob�� ���� �������
	//Vertex Shader�� ���� ����ȴ�.
	ID3D11VertexShader* triangleVSShader = nullptr;

	//Pixel Shader code -> Binary Code ���� ��������
	ID3DBlob* trianglePSBlob = nullptr;

	//Pixel Shader
	ID3D11PixelShader* trianglePSShader = nullptr;


	void SetupState() {

	}

	void LoadBuffer()
	{
		//���۸� �������� �ϴµ�, ���۷� �Ѱ��ֱ����� ���� ��ü Vertex�� ũ�⸦ ����������Ѵ�.
		D3D11_BUFFER_DESC triangleDesc = {};
		triangleDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		triangleDesc.ByteWidth = sizeof(Vertex) * (sizeof(vertexes)/sizeof(vertexes[0]));//������ ��ü ���ؽ��� ũ�⸦ �Ѱ��ش�.
		triangleDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;//���� ���ؽ� ���۾�
		//CPU�� �Ἥ �Ѱ��ֱ� ������, ������ �� �ְ� �������ش�.	
		triangleDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

		//CreateTexture�Ҷ� data�� �Ѱ��� �� �ִµ� �� �� �Ѱ��ٷ��� �����س��´�.
		D3D11_SUBRESOURCE_DATA triangleData = {};
		triangleData.pSysMem = vertexes;
		//�갡 ���� ������ ȣ�����ش�.
		sn::graphics::GetDevice()->CreateBuffer(&triangleBuffer, &triangleDesc, &triangleData);


		//���⼭ �ε��� ���۸� �ʱ�ȭ���ش�.
		//�ε��� ���۴� �׸��� ������ ������ �ִ� ���̹Ƿ�, ���� �����͸� ������ �ȴ�.
		//���ؽ� ���۴� �Ӽ��� ���������ϱ� ����ü�� ������
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

		//�׳� �ε��� ���۸� �������� �� ���� ���긮�ҽ� �����Ϳ� �߰����༭ �Ѱ����
		D3D11_SUBRESOURCE_DATA triangleIdxData = {};
		triangleIdxData.pSysMem = indexes.data();
		sn::graphics::GetDevice()->CreateBuffer(&triangleIdxBuffer, &triangleIdxDesc, &triangleIdxData);

		// Constant Buffer
		//������۸� ����������.
		D3D11_BUFFER_DESC triangleCSDesc = {};
		triangleCSDesc.ByteWidth = sizeof(Vector4);//������۴� �������ٶ� ��Ģ�� �־ ������ Vector4 ������ �����ؾ� �Ѵ�.
		triangleCSDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;//��� ���۴�.
		triangleCSDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;//�ٲ� �� �������� DYNAMIC���� �������ش�.
		triangleCSDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//CPU�� �����ؼ� GPU�� �Ѱ��ֹǷ� �̷��� �ؾ���.

		//������۴� ���긮�ҽ� �����͸� �������� �ʴ´�.

		sn::graphics::GetDevice()->CreateBuffer(&triangleConstantBuffer, &triangleCSDesc, nullptr);

		//��������� ���۸� ����⸸ �ߴ�!
		//��� ���۴� GPU�� �����͸� �Ѱ��־�� ������ �߰����� �۾��� �� ����� �Ѵ�.
		//1. ����� �����͸� ������ְ� cpu�� �ִ� �����͸� GPU�� ������� �ϸ�(MAP, UNMAP)
		//2. ������ �����͸� ������ ���������� ���̴��� ������� �Ѵ�.
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