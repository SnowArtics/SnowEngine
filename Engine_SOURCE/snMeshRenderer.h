#pragma once
#include "snComponent.h"
#include "snMesh.h"
#include "snMaterial.h"
#include "snRenderer.h"

namespace sn
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void BindConstantBuffers();

		void SetMesh(std::shared_ptr<Mesh> mesh) { mMesh = mesh; }
		void SetMaterial(std::shared_ptr<Material> material) { mMaterial = material; }
		void SetMonsterCB(renderer::MonsterCB _monsterCB) { monsterCB = _monsterCB; }
		std::shared_ptr<Material> GetMaterial() { return mMaterial; }
		std::shared_ptr<Mesh> GetMesh() { return mMesh; }
		renderer::MonsterCB GetMonsterCB() { return monsterCB; }

	private:
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Material> mMaterial;

	private://�� ��ü���� ��� ���̴��� �޶�� �� ��� ������۸� �ξ ���⸦ �ٲٰ� �սô�.
		renderer::MonsterCB monsterCB;
	};
}