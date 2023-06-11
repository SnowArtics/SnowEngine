#pragma once
#include "snGameObject.h"
#include "snComponent.h"

#include "snRenderer.h"
#include "snMesh.h"
#include "snShader.h"
#include "snConstantBuffer.h"
#include "snScene.h"

using namespace std;

namespace sn {
	class Player : public GameObject {
	public:
		Player();
		~Player();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		void OnCollision(wstring _objectName);

	public:
		void SetPos(Vector3 _pos) { mPos = _pos; }
		void SetColor(Vector4 _color) { mColor = _color; }
		void SetSize(Vector3 _scale) { mScale = _scale; }
		void SetScene(Scene* _scene) { mScene = _scene; }

		Vector3 GetPos() { return mPos; }
		Vector4 GetColor() { return mColor; }
		Vector3 GetSize() { return mScale; }
		Scene* GetScene() { return mScene; }

	private:
		renderer::Vertex mVertexes[37];
		Mesh* mMesh;
		Shader* mShader;
		graphics::ConstantBuffer* mConstantBuffer;
		Vector4 mConstantBufferPos;
	private:
		Vector3 mPos;
		Vector4 mColor;
		Vector3 mScale;

		Scene* mScene;
	};
}

