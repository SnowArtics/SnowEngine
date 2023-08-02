#pragma once
#include "snEntity.h"
#include "snLayer.h"
#include "snCamera.h"

namespace sn
{
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
		virtual void Destroy();

		virtual void OnEnter();
		virtual void OnExit();

		void SetPlayer(GameObject* _player) { mPlayer = _player; }

		GameObject* GetPlayer() { return mPlayer; }

		void AddGameObject(eLayerType type, GameObject* gameObj);

		template <typename T>
		std::vector<T*> FindObjectsOfType()
		{
			std::vector<T*> findObjs = {};
			for (Layer* layer : mLayers)
			{
				auto gameObjs = layer->GetGameObjects();
				for (GameObject* obj : gameObjs)
				{
					T* buff = dynamic_cast<T*>(obj);
					if (buff != nullptr)
						findObjs.push_back(buff);
				}
			}

			return findObjs;
		}
		GameObject* Find(std::wstring _gameObjectName)
		{
			for (Layer& layer : mLayers) {
				const std::vector<GameObject*> gameObjs = layer.GetGameObjects();
				for (GameObject* obj : gameObjs) {
					if (obj->GetName() == _gameObjectName) {
						return obj;
					}
				}
			}
			return nullptr;
		}
		Layer& GetLayer(eLayerType type) { return mLayers[(UINT)type]; }

		bool GetFlag() { return flag; }
		void SetFlag(bool _flag) { flag = _flag; }

		void SetMainCamera(Camera* _mainCamera) { mainCamera = _mainCamera; }
		Camera* GetMainCamera() { return mainCamera; }

	private:
		std::vector<Layer> mLayers;
		bool flag;
		GameObject* mPlayer;
		Camera*		mainCamera;
	};
}
