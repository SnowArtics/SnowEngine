#pragma once
#include "snComponent.h"

using namespace sn;

class GameObject;

class MiniBossHPBar :
    public Component
{
public:
	MiniBossHPBar();
	~MiniBossHPBar();

	virtual void Initialize();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();

public:
	void CreateHpBar();
	void PlayDamage(float _damage);

private:
	sn::GameObject* bossHPBracket;
	sn::GameObject* bossHPBar;
	sn::GameObject* bossEnterUI;

	float fakeHP;
	float realHP;
	float fullHP;

	float time;
};

