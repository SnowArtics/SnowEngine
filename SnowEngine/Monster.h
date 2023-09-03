#pragma once
#include "../Engine_SOURCE/snGameObject.h"
#include "snRenderer.h"

struct tMonInfo
{
    float      fSpeed;           // 속도
    float      fHP;              // 체력
    float      fRecogRange;      // 인지 범위
    float      fAttRange;        // 공격 범위
    float      fAtt;             // 공격력
    float      fAttTime;         // 공격속도
    float      fAttDelay;        // 공격 딜레이
    float      fUnStiffness;       // 강인함 높을수록 잘밀림
};

class AI;
class Collider2D;
class HitEffect;

class Monster : public sn::GameObject
{
public:
    Monster();
    virtual ~Monster();

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
    void AddHitEffect(Vector3 size);

public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    const tMonInfo& GetInfo() { return m_tInfo; }
    std::pair<int, int> GetMonsterMapPos() { return monsterMapPos; }
    tMonInfo GetMonsterInfo() { return m_tInfo; }
    
    void SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
    void SetMonsterMapPos(int x, int y) { monsterMapPos.second = y, monsterMapPos.first = x; }
    void SetMonsterInfo(tMonInfo _MonInfo) { m_tInfo = _MonInfo; }

private:
    void SetMonInfo(const tMonInfo& _info) { m_tInfo = _info; }

protected:
    tMonInfo    m_tInfo;
    std::pair<int, int>				monsterMapPos;
    float time;
    float hitTime;
    float deadTime;

    HitEffect* hitEffect;

    friend class MonFactory;
};