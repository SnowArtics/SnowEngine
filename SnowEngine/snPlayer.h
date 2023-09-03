#pragma once
#include "snGameObject.h"

using namespace sn;

class snPlayer :
    public sn::GameObject
{
public:
    snPlayer();
    ~snPlayer();

public:
    virtual void Initialize();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();

public:
    virtual void OnCollisionEnter(sn::Collider2D* other);
    virtual void OnCollisionStay(sn::Collider2D* other);
    virtual void OnCollisionExit(sn::Collider2D* other);

public:
    float GetHP() { return HP; }
    float GetUnStiffness() { return UnStiffness; }

    void SetHP(float _HP) { HP = _HP; }
    void SetUnStiffness(float _UnStiffness) { UnStiffness = _UnStiffness; }

private:
    float HP;
    float UnStiffness;
};

