#include "snRigidBody.h"

#include "snTime.h"
#include "snGameObject.h"
#include "snTransform.h"

namespace sn {
	RigidBody::RigidBody()
		:Component(eComponentType::RigidBody)
		, m_fMass(1.f)
		, m_vForce(Vector3(0.f, 0.f, 0.f))
		, m_fFricCoeff(100.f)
		, m_vMaxVelocity(Vector3(200.f, 600.f, 0.f))
	{
	}

	RigidBody::~RigidBody()
	{
	}

	void RigidBody::LateUpdate()
	{
		float fForce = m_vForce.Length();//������ ���� ũ��

		if (fForce != 0.f) {
			m_vForce.Normalize();//������ ���� ����

			float m_fAccel = fForce / m_fMass; //���ӵ��� ũ��

			//���⼭ m_vForce�� ��ֶ������ ���Ⱚ��
			m_vAccel = m_vForce * m_fAccel; //���ӵ� = �� * ���ӵ��� ũ��
			// m_vAccel = m_vForce*m_fAccel;
		}

		m_vAccel += m_vAccelA; // �߷°� ���� ���ӵ��� ���� ���ӵ��� �߰��Ǵ� ���̶� ��¥��¥ ������
		//�߷��� �����̳� ���� ������� �׳� ���ӵ��� �������� ���̶� ��¥��¥ ������

		//���� �ӵ�
		//���� �ֵ� ���� �ӵ��� ���
		m_vVelocity += m_vAccel * Time::DeltaTime();

		//�ӵ��� 0�� �ƴҶ��� 
		if (!(m_vVelocity.x == 0.f && m_vVelocity.y == 0.f && m_vVelocity.z == 0.f)) {
			//�����¿� ���� �ݴ���������� ���ӵ�
			Vector3 vFricDir = -m_vVelocity;
			vFricDir.Normalize();
			Vector3 vFriction = vFricDir * m_fFricCoeff * Time::DeltaTime();
			if (m_vVelocity.Length() <= vFriction.Length()) {
				//�������ӵ��� ���� �ӵ����� �� ū ���
				m_vVelocity = Vector3(0.f, 0.f, 0.f);
			}
			else {
				m_vVelocity += vFriction;
			}
		}


		//�ӵ� ���� �˻�
		if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x)) {
			m_vVelocity.x = (m_vVelocity.x / abs(m_vVelocity.x)) * abs(m_vMaxVelocity.x);
		}

		if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y)) {
			m_vVelocity.y = (m_vVelocity.y / abs(m_vVelocity.y)) * abs(m_vMaxVelocity.y);
		}

		// �ӵ��� ���� �̵�
		Move();

		//������ �� ���� ���Ӱ� ���� ������ ������Ʈ �ϱ� ���ؼ� �ʱ�ȭ
		m_vForce = Vector3(0.f, 0.f, 0.f);

		//���ӵ� �ʱ�ȭ
		m_vAccel = Vector3(0.f, 0.f, 0.f);
		m_vAccelA = Vector3(0.f, 0.f, 0.f);
	}

	void RigidBody::Move()
	{
		float fSpeed = m_vVelocity.Length();

		if (fSpeed != 0.f) {
			//�̵� ����
			// �����ӵ��� �����Ǿ�� ���ؼ� Normalize�ϸ� �ȵ�
			Vector3 vDir = m_vVelocity;
			vDir.Normalize();

			//�̵� �ӷ�
			Vector3 vPos = this->GetOwner()->GetComponent<Transform>()->GetPosition();

			//��� �׳� �����ӵ��� �ð� ���ϸ� ��
			vPos += m_vVelocity * Time::DeltaTime();
			this->GetOwner()->GetComponent<Transform>()->SetPosition(vPos);
		}
	}
}