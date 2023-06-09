#pragma once
#include "snGameObject.h"
#include "snComponent.h"

#include "snRenderer.h"
#include "snMesh.h"
#include "snShader.h"
#include "snConstantBuffer.h"

namespace sn {
	class Player : public GameObject {
	public:
		Player();
		~Player();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
	private:
		renderer::Vertex mVertexes[37];
		sn::Mesh* mMesh;
		sn::Shader* mShader;
		sn::graphics::ConstantBuffer* mConstantBuffer;
	};
}

