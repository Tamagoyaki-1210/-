//==============================================================================
//
// result_obj.cpp	(���U���g�̃I�u�W�F�N�g.cpp)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// �C���N���[�h
//==============================================================================
#include <time.h>
#include "application.h"
#include "renderer.h"
#include "result_obj.h"
#include "objectX.h"
#include "ore.h"
#include "particle_emitter.h"
#include "charmanager.h"

//==============================================================================
// �ÓI�����o�ϐ��錾
//==============================================================================
const float CResultObj::Moving_OreSpeed = 10.0f;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CResultObj::CResultObj()
{

}


//==============================================================================
// �f�X�g���N�^
//==============================================================================
CResultObj::~CResultObj()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CResultObj::Init()
{
	//�N�����Ɉ�񂾂��s�����ߏ������ɏ���	
	srand((unsigned int)time(nullptr));

	//objectX�̏���������
	CObjectX::Init();

	//==================================================
	// �����o�ϐ��̏�����
	//==================================================
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_OreMove = D3DXVECTOR3(0.0f, Moving_OreSpeed, 0.0f);
	m_LogoMove = 3.0f;

	return S_OK;
}

//==============================================================================
// �X�V����
//==============================================================================
void CResultObj::Update()
{
	//�z�΂̋���
	ObjectBehavior();

	//objectX�̍X�V����
	CObjectX::Update();
}

//==============================================================================
// �I�u�W�F�N�g�̋�������
//==============================================================================
void CResultObj::ObjectBehavior()
{
	//�ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();

	if (pos.y <= -500.0f)
	{
		m_RandX = (float)(rand() % 1500 - 750);		//x�͈̔͂Ƀ����_��
		m_RandY = (float)(rand() % 1000 + 500);		//y�͈̔͂Ƀ����_��
		m_RandRotX = (float)(rand() % 30 - 15);		//��]�����_��

		pos = D3DXVECTOR3(m_RandX, m_RandY, 0.0f);
	}
	if (pos.y == -50.0f)
	{
		CParticleEmitter::Create("Coin", pos);
	}

	//��]
	m_rot.x += m_RandRotX / 100;

	//�ړ���
	pos -= m_OreMove;

	//�ݒ�
	SetRot(m_rot);
	SetPos(pos);
}

//==============================================================================
// ��������
//==============================================================================
CResultObj *CResultObj::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, int index)
{
	CResultObj *pResultObj = nullptr;

	pResultObj = new CResultObj;

	if (pResultObj != nullptr)
	{
		pResultObj->SetPos(pos);
		pResultObj->SetRot(rot);
		pResultObj->SetSize(size);
		pResultObj->SetIndex(index);
		pResultObj->Init();
	}

	return pResultObj;
}
