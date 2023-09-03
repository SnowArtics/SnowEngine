#pragma once
#include "Monster.h"

using namespace sn;

class GolemTurret :
    public Monster
{
public:
    GolemTurret();
    ~GolemTurret();

public:
    virtual void Initialize();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();

public:
    virtual void OnCollisionEnter(sn::Collider2D* other, sn::Collider2D* me);
    virtual void OnCollisionStay(sn::Collider2D* other, sn::Collider2D* me);
    virtual void OnCollisionExit(sn::Collider2D* other, sn::Collider2D* me);

public:
    void CreateProjectile();

private:
    float shootTime;
};

