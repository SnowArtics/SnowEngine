#include "snGameObject.h"
#include "snRenderer.h"
#include "snGraphicDevice_Dx11.h"

namespace sn {
	sn::GameObject::GameObject()
		: mState(eState::Active)
	{
	}

	sn::GameObject::~GameObject()
	{
	}

	void sn::GameObject::Initialize()
	{
	}

	void sn::GameObject::Update()
	{
		// 공 움직임 구현

		// 렌더
	}

	void sn::GameObject::LateUpdate()
	{
	}

	void sn::GameObject::Render()
	{
		//상수버퍼로 위치정보 크기정보, 색깔, 업데이트 해줘야한다.

		renderer::mesh->BindBuffer();
		renderer::shader->Binds();
		graphics::GetDevice()->DrawIndexed(renderer::mesh->GetIndexCount(), 0, 0);
	}
}