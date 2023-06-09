#include "snPlayer.h"

#include "snMesh.h"

#include "snMath.h"

namespace sn {

	Player::Player()
		: mVertexes{}
		, mMesh(nullptr)
		, mShader(nullptr)
		, mConstantBuffer(nullptr)
	{
	}

	Player::~Player()
	{
	}

	void Player::Initialize()
	{
		//vertex 持失
		double angle = 0;

		mVertexes[0].pos = Vector3(0.0f, 0.0f, 0.0f);
		mVertexes[0].color = Vector4(0.3137f, 0.7373f, 0.8745f, 1.0f);

		for (int i = 1; i < (sizeof(mVertexes)); i++) {
			double sinValue = std::sin(angle * XM_PI / 180);
			double cosValue = std::cos(angle * XM_PI / 180);

			mVertexes[i].pos = Vector3(mVertexes[0].pos.x + 0.01 * sinValue, mVertexes[0].pos.y + 0.01 * cosValue, 0.0f);
			mVertexes[i].color = Vector4(0.3137f, 0.7373f, 0.8745f, 1.0f);

			angle += 10.f;
		}

		//Vertex Buffer 持失
		mMesh = new sn::Mesh();
		mMesh->CreateVertexBuffer(mVertexes, 37);

		//Index 持失
		std::vector<UINT> indexes = {};
		indexes.push_back(0);

		for(UINT i=1; i<sizeof(mVertexes)-1;i++){
			indexes.push_back(0);
			indexes.push_back(i);
			indexes.push_back(i+1);
		}

		indexes.push_back(0);
		indexes.push_back(sizeof(mVertexes)-1);
		indexes.push_back(1);

		//Index Buffer 持失
		mMesh->CreateIndexBuffer(indexes.data(), indexes.size());
	}

	void Player::Update()
	{
	}

	void Player::LateUpdate()
	{
	}

	void Player::Render()
	{
	}

}