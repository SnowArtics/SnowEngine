#include "snRenderer.h"

#define M_PI 3.14159265358979323846

namespace sn::renderer {
	Vertex vertexes[135] = {}; //�̷��� ���� ������ GPU�� �Ѱ�����Ѵ�. �̸� ���� ���ؽ� ���۸� ����.

	// Input Layout (���� ����)
	ID3D11InputLayout* triangleLayout = nullptr;

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
		vertexes[0].pos = Vector3(-0.5f, 0.75f, 0.0f);
		vertexes[0].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[1].pos = Vector3(-0.25f, 0.25f, 0.0f);
		vertexes[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(-0.75f, 0.25f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		vertexes[3].pos = Vector3(0.5f, 0.75f, 0.0f);
		vertexes[3].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[4].pos = Vector3(0.75f, 0.5f, 0.0f);
		vertexes[4].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[5].pos = Vector3(0.25f, 0.5f, 0.0f);
		vertexes[5].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		vertexes[6].pos = Vector3(0.75f, 0.5f, 0.0f);
		vertexes[6].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[7].pos = Vector3(0.5f, 0.25f, 0.0f);
		vertexes[7].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[8].pos = Vector3(0.25f, 0.5f, 0.0f);
		vertexes[8].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		vertexes[9].pos = Vector3(-0.75f, -0.25f, 0.0f);
		vertexes[9].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[10].pos = Vector3(-0.25f, -0.25f, 0.0f);
		vertexes[10].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[11].pos = Vector3(-0.75f, -0.75f, 0.0f);
		vertexes[11].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		vertexes[12].pos = Vector3(-0.75f, -0.75f, 0.0f);
		vertexes[12].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[13].pos = Vector3(-0.25f, -0.25f, 0.0f);
		vertexes[13].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[14].pos = Vector3(-0.25f, -0.75f, 0.0f);
		vertexes[14].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		

		//double angle = 0;

		//double sinValue = std::sin(angle * M_PI / 180.0);
		//double cosValue = std::cos(angle * M_PI / 180.0);

		//Vector3 focus = Vector3(0.5f, -0.5f, 0.0f);

		//vertexes[15].pos = Vector3(focus.x + 0.25 * sinValue, focus.y + 0.25 * cosValue, 0.0f);
		//vertexes[15].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

		//angle += 9;
		//sinValue = std::sin(angle * M_PI / 180.0);
		//cosValue = std::cos(angle * M_PI / 180.0);

		//vertexes[16].pos = Vector3(focus.x + 0.25 * sinValue, focus.y + 0.25 * cosValue, 0.0f);
		//vertexes[16].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

		//vertexes[17].pos = focus;
		//vertexes[17].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

		double angle = 0;

		for (int i = 15; i < (sizeof(vertexes) / sizeof(vertexes[0])); i+=3) {
			double sinValue = std::sin(angle * M_PI / 180.0);
			double cosValue = std::cos(angle * M_PI / 180.0);
		
			Vector3 focus = Vector3(0.5f, -0.5f, 0.0f);
		
			vertexes[i].pos = Vector3(focus.x + 0.25*sinValue, focus.y+0.25*cosValue, 0.0f);
			vertexes[i].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		
			angle += 9;
			sinValue = std::sin(angle * M_PI / 180.0);
			cosValue = std::cos(angle * M_PI / 180.0);
		
			vertexes[i+1].pos = Vector3(focus.x + 0.25 * sinValue, focus.y + 0.25 * cosValue, 0.0f);
			vertexes[i+1].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		
			vertexes[i + 2].pos = focus;
			vertexes[i + 2].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		SetupState();
		LoadBuffer();
		LoadShader();
	}
}