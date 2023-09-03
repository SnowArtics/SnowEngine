#pragma once
#include "Monster.h"

using namespace sn;

class FylingRepairGolem :
    public Monster
{
public:
    FylingRepairGolem();
    ~FylingRepairGolem();

public:
    virtual void Initialize();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();

public:
    virtual void OnCollisionEnter(sn::Collider2D* other, sn::Collider2D* me);
    virtual void OnCollisionStay(sn::Collider2D* other, sn::Collider2D* me);
    virtual void OnCollisionExit(sn::Collider2D* other, sn::Collider2D* me);
};

