#include "snRenderer.h"
#include "snConstantBuffer.h"

#define M_PI 3.14159265358979323846

namespace renderer {
	using namespace sn;
	using namespace sn::graphics;

	Vertex vertexes[4] = {}; //�̷��� ���� ������ GPU�� �Ѱ�����Ѵ�. �̸� ���� ���ؽ� ���۸� ����.

	//Vertex Buffer
	////�̰� ����ؼ� ������ �Ѵ�. �׷��� �ϴ� ������ �߾�! ��� ó���Ҳ���
	////������ ���� ������ ����������ϴµ�, �̸� ���� ���ؽ� ���̴� ������ ��ģ��.
	//ID3D11Buffer* triangleBuffer = nullptr; 
	////�ε��� ���۸� ����ϱ� ���� �ε��� ���۸� �����.
	//ID3D11Buffer* triangleIdxBuffer = nullptr;
	
	//VertexBuffer
	sn::Mesh* mesh = nullptr;
	sn::Shader* shader = nullptr;
	//���̴��� �����͸� ������ �ֱ� ���� ��� ���۸� �����.
	sn::graphics::ConstantBuffer* constantBuffer = nullptr;

	Vector4 constantBufferPos;

	////Error Blob
	//ID3DBlob* errorBlob = nullptr;//Ȥ�� ����� ���� �߸� ���⿡ ���޵ȴ�.

	////Vertex Shader code -> Binary Code�� �ٲ㼭 �츮�� ����� �� �ְ� ������ش�.
	////HLSL�ڵ尡 �����ϵ� ������� ���� ����.
	//ID3DBlob* triangleVSBlob = nullptr;

	////Blob�� ���� �������
	////Vertex Shader�� ���� ����ȴ�.
	//ID3D11VertexShader* triangleVSShader = nullptr;

	////Pixel Shader code -> Binary Code ���� ��������
	//ID3DBlob* trianglePSBlob = nullptr;

	void SetupState() {
		// Input layout ���� ���� ������ �Ѱ�����Ѵ�.
		D3D11_INPUT_ELEMENT_DESC arrLayout[2] = {};

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


		sn::graphics::GetDevice()->CreateInputLayout(arrLayout, 2
			, shader->GetVSCode()
			, shader->GetInputLayoutAddressOf());
	}

	void LoadBuffer()
	{
		// Vertex Buffer
		mesh = new sn::Mesh();
		mesh->CreateVertexBuffer(vertexes, 4);

		//���⼭ �ε��� ���۸� �ʱ�ȭ���ش�.
		//�ε��� ���۴� �׸��� ������ ������ �ִ� ���̹Ƿ�, ���� �����͸� ������ �ȴ�.
		//���ؽ� ���۴� �Ӽ��� ���������ϱ� ����ü�� ������
		std::vector<UINT> indexes = {};
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		// Index Buffer
		mesh->CreateIndexBuffer(indexes.data(), indexes.size());

		// Constant Buffer
		//������۸� ����������.
		D3D11_BUFFER_DESC triangleCSDesc = {};
		triangleCSDesc.ByteWidth = sizeof(Vector4);//������۴� �������ٶ� ��Ģ�� �־ ������ Vector4 ������ �����ؾ� �Ѵ�.
		triangleCSDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;//��� ���۴�.
		triangleCSDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;//�ٲ� �� �������� DYNAMIC���� �������ش�.
		triangleCSDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//CPU�� �����ؼ� GPU�� �Ѱ��ֹǷ� �̷��� �ؾ���.

		//������۴� ���긮�ҽ� �����͸� �������� �ʴ´�.
		
		constantBuffer = new sn::graphics::ConstantBuffer(sn::graphics::eCBType::Transform);
		constantBuffer->Create(sizeof(Vector4));

		//��������� ���۸� ����⸸ �ߴ�!
		//��� ���۴� GPU�� �����͸� �Ѱ��־�� ������ �߰����� �۾��� �� ����� �Ѵ�.
		//1. ����� �����͸� ������ְ� cpu�� �ִ� �����͸� GPU�� ������� �ϸ�(MAP, UNMAP)
		//2. ������ �����͸� ������ ���������� ���̴��� ������� �Ѵ�.
		//Vector4 pos(0.3f, 0.0f, 0.0f, 1.0f);
		constantBufferPos = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		constantBuffer->SetData(&constantBufferPos);
		constantBuffer->Bind(sn::graphics::eShaderStage::VS);
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

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

		LoadBuffer();
		LoadShader();
		SetupState();
	}

	void Release()
	{
		//����, Blob, Shader ���� ��ü �����͵��� ���̷�ƮX�� �������ִ� Release() �Լ��� �ִ�. �װ� ����.		
		delete mesh;
		delete shader;
		delete constantBuffer;
	}
}