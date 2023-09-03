#pragma once
#include "snGameObject.h"

using namespace sn;

enum class HitState {
    SWORD,
    BOW,
};

class HitEffect :
    public sn::GameObject
{
public:
    HitEffect();
    ~HitEffect();

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
    void PlaySwordEffect();
    void PlayBowEffect();

private:
    HitState hitState;
    float playTime;
};

