#pragma once
#include "snComponent.h"

using namespace sn;

class GameObject;

class PlayerHP :
    public Component
{
public:
	PlayerHP();
	~PlayerHP();

	virtual void Initialize();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();

public:
	void CreateHpBar();
	void PlayDamage(float _damage);

private:
	sn::GameObject* playerHPBarRed;
	sn::GameObject* playerHPBarWhite;
	sn::GameObject* playerHitScreen;

	float fakeHP;
	float realHP;
	float fullHP;

	float hitTime;
	float absoluteTime;
};

