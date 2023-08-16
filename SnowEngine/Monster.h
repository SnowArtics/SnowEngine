#pragma once
#include "../Engine_SOURCE/snGameObject.h"

struct tMonInfo
{
    float      fSpeed;           // 속도
    float      fHP;              // 체력
    float      fRecogRange;      // 인지 범위
    float      fAttRange;        // 공격 범위
    float      fAtt;             // 공격력
};

class AI;
class Collider2D;

class Monster : public sn::GameObject
{
public:
    Monster();
    ~Monster();

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

    friend class MonFactory;
};