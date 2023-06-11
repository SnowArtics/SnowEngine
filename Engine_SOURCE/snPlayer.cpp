#include "snPlayer.h"

#include "snApplication.h"
#include "snGraphicDevice_Dx11.h"
#include "snScene.h"

#include "snMath.h"
#include "snInput.h"
#include "snTime.h"

namespace sn {

	Player::Player()
		: mVertexes{}
		, mMesh(nullptr)
		, mShader(nullptr)
		, mConstantBuffer(nullptr)
		, mConstantBufferPos(Vector4(0.0f, 0.0f, 0.0f, 0.0f))
		, mPos(Vector3(0.0f, 0.0f, 0.0f))
		, mColor(Vector4(0.3137f, 0.7373f, 0.8745f, 1.0f))
		, mScale(Vector3(0.1f, 0.1f, 0.0f))
		, mScene(nullptr)
	{
	}

	Player::~Player()
	{
		delete mMesh;
		delete mShader;
		delete mConstantBuffer;
	}

	void Player::Initialize()
	{
		//vertex 持失
		double angle = 0;

		mVertexes[0].pos = mPos;
		mVertexes[0].color = mColor;

		for (int i = 1; i < (sizeof(mVertexes)/sizeof(mVertexes[0])); i++) {
			double sinValue = std::sin(angle * XM_PI / 180);
			double cosValue = std::cos(angle * XM_PI / 180);

			mVertexes[i].pos = Vector3(mVertexes[0].pos.x + mScale.x*0.9 * sinValue, mVertexes[0].pos.y + mScale.x * 1.6 * cosValue, 0.0f);
			mVertexes[i].color = mColor;

			angle += 10.f;
		}

		//Vertex Buffer 持失
		mMesh = new Mesh();
		mMesh->CreateVertexBuffer(mVertexes, 37);

		//Index 持失
		std::vector<UINT> indexes = {};

		for(UINT i=1; i< (sizeof(mVertexes) / sizeof(mVertexes[0])) -1;i++){
			indexes.push_back(0);
			indexes.push_back(i);
			indexes.push_back(i+1);
		}

		indexes.push_back(0);
		indexes.push_back((sizeof(mVertexes) / sizeof(mVertexes[0])) -1);
		indexes.push_back(1);

		//Index Buffer 持失
		mMesh->CreateIndexBuffer(indexes.data(), indexes.size());

		//Constant Buffer 持失
		mConstantBuffer = new graphics::ConstantBuffer(graphics::eCBType::PlayerTransform);
		mConstantBuffer->Create(sizeof(Vector4));

		//shader create
		mShader = new Shader();
		mShader->Create(graphics::eShaderStage::VS, L"PlayerVS.hlsl", "main");
		mShader->Create(graphics::eShaderStage::PS, L"PlayerPS.hlsl", "main");

		//InputLayout
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


		graphics::GetDevice()->CreateInputLayout(arrLayout, 2
			, mShader->GetVSCode()
			, mShader->GetInputLayoutAddressOf());
	}

	void Player::Update()
	{
		if (sn::Input::GetKey(sn::eKeyCode::W)) {
			mConstantBufferPos += Vector4(0.0f, 0.2f, 0.0f, 1.0f) * sn::Time::DeltaTime();
			mPos.x = mConstantBufferPos.x;
			mPos.y = mConstantBufferPos.y;
			mPos.z = mConstantBufferPos.z;
		}
		if (sn::Input::GetKey(sn::eKeyCode::S)) {
			mConstantBufferPos += Vector4(0.0f, -0.2f, 0.0f, 1.0f) * sn::Time::DeltaTime();
			mPos.x = mConstantBufferPos.x;
			mPos.y = mConstantBufferPos.y;
			mPos.z = mConstantBufferPos.z;
		}
		if (sn::Input::GetKey(sn::eKeyCode::A)) {
			mConstantBufferPos += Vector4(-0.2f, 0.0f, 0.0f, 1.0f) * sn::Time::DeltaTime();
			mPos.x = mConstantBufferPos.x;
			mPos.y = mConstantBufferPos.y;
			mPos.z = mConstantBufferPos.z;
		}
		if (sn::Input::GetKey(sn::eKeyCode::D)) {
			mConstantBufferPos += Vector4(0.2f, 0.0f, 0.0f, 1.0f) * sn::Time::DeltaTime();
			mPos.x = mConstantBufferPos.x;
			mPos.y = mConstantBufferPos.y;
			mPos.z = mConstantBufferPos.z;
		}

		mConstantBuffer->SetData(&mConstantBufferPos);
		mConstantBuffer->Bind(graphics::eShaderStage::VS);
	}

	void Player::LateUpdate()
	{
	}

	void Player::Render()
	{
		mMesh->BindBuffer();
		mShader->Binds();
		graphics::GetDevice()->DrawIndexed(mMesh->GetIndexCount(), 0, 0);
	}

	void Player::OnCollision(wstring _objectName)
	{
		int objectNum = mScene->FindObjectIndex(_objectName);
		mScene->DeleteObject(objectNum);
	}
}