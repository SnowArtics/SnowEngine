#include "snMeshRenderer.h"
#include "snGameObject.h"
#include "snTransform.h"
#include "snRenderer.h"
#include "snAnimator.h"

namespace sn
{
	MeshRenderer::MeshRenderer()
		: Component(eComponentType::MeshRenderer)
		, monsterCB{}
	{
		monsterCB.state = 0;
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::Initialize()
	{
	}

	void MeshRenderer::Update()
	{
	}

	void MeshRenderer::LateUpdate()
	{
	}

	void MeshRenderer::Render()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		tr->BindConstantBuffer();

		BindConstantBuffers();

		mMesh->BindBuffer();
		mMaterial->Binds();

		Animator* animator = GetOwner()->GetComponent<Animator>();
		if (animator)
		{
			animator->Binds();
		}

		mMesh->Render();

		mMaterial->Clear();
	}

	void MeshRenderer::BindConstantBuffers()
	{
		//if (monsterCB.state == 1) {
		//	monsterCB.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		//}
		//else if (monsterCB.state == 2) {
		//	monsterCB.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		//}
		//else {
		//	monsterCB.color = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		//}

		ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Monster];
		cb->SetData(&monsterCB);

		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}
}
