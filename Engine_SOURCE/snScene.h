#pragma once
#include "snEntity.h"
#include "snGameObject.h"

#include <queue>

namespace sn
{
	struct Respawn {
	public:
		std::wstring name;
		float time;
		math::Vector3 Pos;
	};
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		void ColliderCheck();

		int FindObjectIndex(std::wstring _objectName);
		bool DeleteObject(UINT _index);

		void RespawnItem();

	private:
		std::vector<GameObject*> mGameObjects;
		std::queue<Respawn*> mRespawn;

		LONGLONG mItemNum;
		float mTime;
	};
}
