#include "snScene.h"

namespace sn {
	sn::Scene::Scene()
	{
	}

	sn::Scene::~Scene()
	{
		for (Layer* layer : mLayers) {
			delete layer;
		}

		mLayers.clear();
	}

	void sn::Scene::Initialize()
	{
		for (Layer* layer : mLayers)
		{
			layer->Initialize();
		}
	}

	void sn::Scene::Update()
	{
		for (Layer* layer : mLayers)
		{
			layer->Update();
		}
	}

	void sn::Scene::LateUpdate()
	{
		for (Layer* layer : mLayers)
		{
			layer->LateUpdate();
		}
	}

	void sn::Scene::Render()
	{
		for (Layer* layer : mLayers)
		{
			layer->Render();
		}
	}
}
