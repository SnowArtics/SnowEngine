#pragma once
#include "Monster.h"

using namespace sn;

class GolemSmashWave :
    public sn::GameObject
{
public:
    GolemSmashWave();
    ~GolemSmashWave();

public:
    virtual void Initialize();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();

public:
    virtual void OnCollisionEnter(sn::Collider2D* other);
    virtual void OnCollisionStay(sn::Collider2D* other);
    virtual void OnCollisionExit(sn::Collider2D* other);

private:
    float time;
};

