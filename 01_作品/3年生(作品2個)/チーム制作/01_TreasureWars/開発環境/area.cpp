//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@area.cpp
// koduna hirohito
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include "area.h"
#include "texture.h"
#include "application.h"
#include <assert.h>
#include "input.h"

//==============================================================================================
// �ÓI�����o�ϐ��錾
//==============================================================================================
const D3DXVECTOR3 CArea::AREA_POS = D3DXVECTOR3(800.0f, 2.0f, 800.0f);
const D3DXVECTOR2 CArea::AREA_UV_SIZE_INIT = D3DXVECTOR2(0.25f, 0.25f);
const D3DXVECTOR2 CArea::EACH_PLAYER_DIRECTION[] = { 
	D3DXVECTOR2(-0.5f, -0.5f) ,
	D3DXVECTOR2(0.5f, -0.5f) ,
	D3DXVECTOR2(-0.5f, 0.5f) ,
	D3DXVECTOR2(0.5f, 0.5f) };
const float CArea::AREA_UV_MOVE = AREA_UV_SIZE_INIT.x / 4.0f;
const float CArea::AREA_UV_MOVE_OBLIQUE = sqrtf(AREA_UV_SIZE_INIT.x * AREA_UV_SIZE_INIT.x + AREA_UV_SIZE_INIT.y * AREA_UV_SIZE_INIT.y) / 4.0f;
const float CArea::AREA_SIZE = 800.0f;
const float CArea::AREA_FLOATING_DAMPING_RATE = 0.04f;
const float CArea::ANGLE_EACH_EACH_CHARACTER[] = {
	D3DXToRadian(45.0f),
	D3DXToRadian(315.0f),
	D3DXToRadian(135.0f),
	D3DXToRadian(225.0f)
};

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CArea::CArea()
{
	
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CArea::~CArea()
{
}

//==============================================================================================
// ������
//==============================================================================================
HRESULT CArea::Init()
{

	//�e�̏�����
	if (FAILED(C3DPolygon::Init()))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̐ݒ�
	SetTextIndex(CTexture::TEXTURE_AREA);

	//�G���A�g���̉�
	m_nAreaExtensionX = 0;
	m_nAreaExtensionZ = 0;
	m_nOldAreaExtensionX = 0;
	m_nOldAreaExtensionZ = 0;

	//�\���̈ʒu
	m_CrossPos = AREA_POS;
	m_CrossPos.y = 0.0f;

	//�G���A�ϓ��J�E���g�̏�����
	m_nAreaFloatingTime = 0;

	return S_OK;
}

//==============================================================================================
// �I��
//==============================================================================================
//void CArea::Uninit()
//{
//}

//==============================================================================================
// �X�V
//==============================================================================================
void CArea::Update()
{
	//�G���A�ϓ��J�E���g
	m_nAreaFloatingTime++;

	//�J�E���g�`�F�b�N
	if (m_nAreaFloatingTime > AREA_FLOATING_TIME)
	{
		//�v���C���[�̏��ʊm�F
		PlayerRankConfirmation();
		//�^�C���̃��Z�b�g
		m_nAreaFloatingTime = 0;
	}
	

	//���݂�UV�̈ʒu�̎擾
	D3DXVECTOR2 UV = GetUV();

	if (UV.x < AREA_UV_SIZE_INIT.x)
	{
		m_DestUVPos = D3DXVECTOR2(0.5f, 0.5f);
	}

	//�ړI�̂Ƃ̋����̌v�Z
	UV = m_DestUVPos - UV;

	//�����v�Z
	UV *= AREA_FLOATING_DAMPING_RATE;

	//���Z
	AddUVPos(UV);

	//�e���̔��f
	SetPolygon();

	//�X�V���ꂽ��UV�̈ʒu�̍Ď擾
	UV = GetUV();

	//UV�����̌v�Z
	D3DXVECTOR2 Ratio;

	//X
	Ratio.x = (UV.x - AREA_UV_SIZE_INIT.x) / (AREA_UV_SIZE_INIT.x * 2.0f);
	//Y
	Ratio.y = (UV.y - AREA_UV_SIZE_INIT.y) / (AREA_UV_SIZE_INIT.y * 2.0f);

	//�\���̈ʒu�̌v�Z(3�����Ή��̂���Y��Z�Ƃ���)
	m_CrossPos.x = AREA_POS.x * 2.0f - AREA_POS.x * 2.0f * Ratio.x;
	m_CrossPos.z = AREA_POS.z * 2.0f * Ratio.y;

}

//==============================================================================================
// �`��
//==============================================================================================
void CArea::Draw()
{
	
	// �f�o�C�X�̃|�C���^���擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �����_�����O�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//�e�̕`��
	C3DPolygon::Draw();

	// ���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�o�b�t�@�̐ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// �ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==============================================================================================
//�v���C���[�̏��ʊm�F
//==============================================================================================
void CArea::PlayerRankConfirmation()
{
	//�G���A�ϓ����s�����ǂ���
	if (m_nTopCharaNum >= 0)
	{
		AreaFluctuation(m_nTopCharaNum);
	}
}

//==============================================================================================
//�G���A�ϓ�
//==============================================================================================
void CArea::AreaFluctuation(int nPLNum)
{
	//�G���A�g���J�E���g
	if (nPLNum <= 1)
	{
		m_nAreaExtensionZ--;
	}
	else
	{
		m_nAreaExtensionZ++;
	}

	if (nPLNum % 2 == 0)
	{
		m_nAreaExtensionX--;
	}
	else
	{
		m_nAreaExtensionX++;
	}

	//X�̌��E�l�̔���
	if (m_nAreaExtensionX > AREA_EXTENSION)
	{
		m_nAreaExtensionX = AREA_EXTENSION;
	}
	else if (m_nAreaExtensionX < -AREA_EXTENSION)
	{
		m_nAreaExtensionX = -AREA_EXTENSION;
	}

	//Z�̌��E�l�̔���
	if (m_nAreaExtensionZ > AREA_EXTENSION)
	{
		m_nAreaExtensionZ = AREA_EXTENSION;
	}
	else if (m_nAreaExtensionZ < -AREA_EXTENSION)
	{
		m_nAreaExtensionZ = -AREA_EXTENSION;
	}

	//�O��Ƃ̍���
	int nAreaExtensionDifferenceX, nAreaExtensionDifferenceZ;

	//X�̍���
	nAreaExtensionDifferenceX = m_nAreaExtensionX - m_nOldAreaExtensionX;

	//Z�̍���
	nAreaExtensionDifferenceZ = m_nAreaExtensionZ - m_nOldAreaExtensionZ;

	//�X�V�Ώۂ��ǂ���
	if (nAreaExtensionDifferenceX == 0 && nAreaExtensionDifferenceZ == 0)
	{
		return;
	}

	//����
	float fUVMove = AREA_UV_MOVE;

	//�����ɂƂ��Ē�����ς���
	if (nAreaExtensionDifferenceX != 0 && nAreaExtensionDifferenceZ != 0)
	{
		fUVMove = AREA_UV_MOVE_OBLIQUE;
	}

	D3DXVECTOR2 ExtendedDirection = D3DXVECTOR2((float)nAreaExtensionDifferenceX,(float)nAreaExtensionDifferenceZ);

	//������1�ɂ���
	D3DXVec2Normalize(&ExtendedDirection, &ExtendedDirection);

	//�ړI�ʒu�̐ݒ�
	m_DestUVPos += ExtendedDirection * fUVMove;

	//Old�̍X�V
	m_nOldAreaExtensionX = m_nAreaExtensionX;
	m_nOldAreaExtensionZ = m_nAreaExtensionZ;

}

//==============================================================================================
// �w�n�����ǂ���
//==============================================================================================
bool CArea::InCheckTerritory(D3DXVECTOR3 Pos, int nCharacterNum)
{
	return CheckInAngle(Pos, ANGLE_EACH_EACH_CHARACTER[nCharacterNum],90.0f, m_CrossPos);
}

//==============================================================================================
// ��������
//==============================================================================================
CArea *CArea::Create()
{
	//�|�C���^�ϐ��錾
	CArea *pArea = nullptr;

	//���̊m��
	pArea = new CArea;

	//NULL�`�F�b�N
	if (pArea == nullptr)
	{
		assert(false);
	}

	//������
	if (FAILED(pArea->Init()))
	{
		assert(false);
	}

	//�e���̓o�^
	pArea->SetPos(AREA_POS);
	pArea->SetRot(D3DXVECTOR3(0.0f,0.0f,0.0f));
	pArea->SetDiagonalLine(AREA_SIZE, AREA_SIZE);
	pArea->SetColor(D3DXCOLOR(1.0f,1.0f,1.0f,0.3f));
	pArea->SetUVSize(AREA_UV_SIZE_INIT);
	pArea->SetPolygon();
	pArea->SetDestUVPos(pArea->GetUV());

	return pArea;
}