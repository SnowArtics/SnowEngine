#pragma once
#include "snComponent.h"

namespace sn {
    class RigidBody :
        public Component
    {
	public:
		RigidBody();
		~RigidBody();

	private:
		Vector3		m_vForce; // ũ��, ����
		Vector3		m_vAccel; //���ӵ�
		Vector3		m_vAccelA;	// �߰� ���ӵ�
		Vector3		m_vVelocity; // �ӵ� (ũ��: �ӷ�, ����)
		Vector3		m_vMaxVelocity; // �ִ� �ӵ�

		float		m_fMass; // ����
		float		m_fFricCoeff; // ���� ���(�������� ������ ���� �ݴ����)

		// F = M * A; //��
		// V += A * DT; // �ӵ��� ���ӵ� * �ð�

	public:
		virtual void LateUpdate();

	public:
		void AddForce(Vector3 _vF) { m_vForce += _vF; }

		void SetMass(float _fMass) { m_fMass = _fMass; }
		float GetMass() { return m_fMass; }
		float GetSpeed() { return m_vVelocity.Length(); }
		Vector3 GetVelocity() { return m_vVelocity; }
		
		void SetVelocity(Vector3 _v) { m_vVelocity = _v; }
		void AddVelocity(Vector3 _v) { m_vVelocity += _v; }
		void SetMAXVelocity(Vector3 _v) { m_vMaxVelocity = _v; }
		void SetAccelAlpha(Vector3 _vAccelA) { m_vAccelA = _vAccelA; }

	private:
		void Move();
	
    };
}