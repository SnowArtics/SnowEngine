#pragma once
#include "snComponent.h"

class GameObject;

class MonsterHPBar :
	public sn::Component
{
public:
	MonsterHPBar();
	~MonsterHPBar();

	virtual void Initialize();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();

public:
	void CreateHpBar();
	void PlayDamage(float _damage);
	void SetEnable(bool _enable);

private:
	sn::GameObject* hpBackBar;
	sn::GameObject* hpMiddleBar;
	sn::GameObject* hpFrontBar;

	float fakeHP;
	float realHP;
	float fullHP;
};

