#pragma once
#include "../Engine_SOURCE/snGameObject.h"
#include "snRenderer.h"

struct tMonInfo
{
    float      fSpeed;           // �ӵ�
    float      fHP;              // ü��
    float      fRecogRange;      // ���� ����
    float      fAttRange;        // ���� ����
    float      fAtt;             // ���ݷ�
    float      fAttTime;         // ���ݼӵ�
    float      fAttDelay;        // ���� ������
    float      fUnStiffness;       // ������ �������� �߹и�
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