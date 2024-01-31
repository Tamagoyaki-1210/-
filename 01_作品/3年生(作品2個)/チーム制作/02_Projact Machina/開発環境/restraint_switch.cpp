//==============================================================================================
//
// �S���X�C�b�`�@�@�@restraint_switch.cpp
// tutida ryousei
//
//==============================================================================================
#include"restraint_switch.h"
#include"objectX.h"
#include"debugProc.h"
#include"cannon.h"
#include"map_object.h"
#include"object3D.h"
#include"application.h"
#include"camera.h"

int CRestraint_Switch::m_nCount_Switch = 0;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CRestraint_Switch::CRestraint_Switch(const PRIORITY priority) : CMove_Object(priority)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CRestraint_Switch::~CRestraint_Switch()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CRestraint_Switch::Init()
{
	CMove_Object::Init();

	// �����蔻��̐ݒ�
	SetCollision();

	// �����蔻��̃^�C�v
	SetCollision_Type(CMove_Object::COLLISION_TYPE_BLOCK);

	// �^�O�̐ݒ�
	SetTag(TAG_MAP_OBJECT);

	D3DXVECTOR3 Max = GetObjectX()->GetMaxSize();
	D3DXVECTOR3 Min = GetObjectX()->GetMinSize();

	SetIndex({ Min.x,0.0f,Min.z }, 0);
	SetIndex({ Min.x,0.0f,Max.z }, 1);
	SetIndex({ Max.x,0.0f,Max.z }, 2);
	SetIndex({ Max.x,0.0f,Min.z }, 3);

	SetCenterPos({ 0.0f,Max.y / 2,0.0f });
	SetSize(GetObjectX()->GetMaxSize());

	// �C��̈ʒu
	D3DXVECTOR3 Cannon_Pos = { GetPos().x,GetPos().y,GetPos().z };

	// �C��̐���
	CCannon::Create({ Cannon_Pos.x,Cannon_Pos.y + 30.0f,Cannon_Pos.z }, { 0.0f,0.0f,0.0f }, nullptr, "Data/model/Cannon_Up.x", m_nIndex);

	m_Display_Key = CObject3D::Create({ GetPos().x, GetPos().y + 120.0f, GetPos().z }, { 70.0f,70.0f }, PRIORITY_CENTER, { 1.0f,1.0f,1.0f,1.0f }, true);
	m_Display_Key->SetTexture(CTexture::TEXTURE_KEY_E);
	m_Display_Key->SetDrawFlag(false);

	m_nCount_Switch = 0;

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CRestraint_Switch::Uninit()
{
	CMove_Object::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CRestraint_Switch::Update()
{
	CMove_Object::Update();

	if (!m_bPush)
		m_Display_Key->SetDrawFlag(m_bHit);
	else
		m_Display_Key->SetDrawFlag(false);

	m_bHit = false;

	if (CApplication::GetCamera()->GetOpening())
		m_bBoss = true;
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CRestraint_Switch::Draw()
{
}

//==============================================================================================
// �q�b�g����
//==============================================================================================
void CRestraint_Switch::Hit(CMove_Object* pHit)
{
	if (!m_bPush && pHit->GetTag() == TAG_CHARACTER && pHit->GetPlayerSide()
		&& m_bBoss)
		m_bHit = true;
}

//==============================================================================================
// ��������
//==============================================================================================
CRestraint_Switch *CRestraint_Switch::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, int index, const PRIORITY priority)
{
	CRestraint_Switch *pRestraint = new	CRestraint_Switch(priority);

	if (pRestraint != nullptr)
	{
		pRestraint->SetObjectX(CObjectX::Create(pos, rot, parent, Xfilename, priority));
		pRestraint->SetPos(pos);
		pRestraint->m_nIndex = index;
		pRestraint->Init();
	}

	return pRestraint;
}