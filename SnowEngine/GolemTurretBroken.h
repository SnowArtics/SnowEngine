#pragma once
#include "Monster.h"

enum class GolemTurretBrokenDir {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

class GolemTurretBroken :
    public Monster
{
public:
    GolemTurretBroken();
    ~GolemTurretBroken();

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

public:
    float shootTime;
};

