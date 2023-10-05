//=============================================================================
//
// ����
// Author : tanimoto kosuke
//
//=============================================================================
#include "explosion.h"
#include "application.h"
#include "texture.h"
#include "stage.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define EXPLOSION_ANIM_X		(8)
#define EXPLOSION_ANIM_Y		(1)

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CExplosion::CExplosion() : CObject2D(EFFECT_PRIORITY_BACK), m_nLife(0)
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CExplosion::~CExplosion()
{

}

//=====================================
//����������
//=====================================
void CExplosion::Init()
{
	CObject2D::Init();
	m_nLife = 30;

	SetColor(D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.0f));
	//�I�u�W�F�N�g�ݒ菈��
	CObject2D::SetObject2D(TYPE_EFFECT, m_fWidth, m_fHeight);
} 

//=====================================
//�I������
//=====================================
void CExplosion::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CExplosion::Update()
{
	CObject2D::Update();

	//UV�ݒ�
	CObject2D::SetUV();

	CObject2D::SetAnim(EXPLOSION_ANIM_X, EXPLOSION_ANIM_Y,1);

	//�e�ړ�
	m_move.x = -2.0f;

	//����
	m_move = m_move - (m_move * 0.05f);

	//�ʒu�擾
	D3DXVECTOR3 pos = GetPos();

	//�ʒu�X�V
	pos += m_move;

	//�ʒu�ݒ菈��
	SetPos(pos);

	//�����v�Z
	if (m_nLife <= 0)
	{
		Uninit();
	}
	else
	{//���������炷
		m_nLife--;
	}
}

//=====================================
//�`�揈��
//=====================================
void CExplosion::Draw()
{
	//�摜�擾����
	CApplication::GetTexture()->TextureType(CTexture::TYPE_EXPLOSION);
	CObject2D::Draw();
}

//=====================================
//������������
//=====================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CExplosion* pExplosion;	//�����|�C���^

	pExplosion = new CExplosion;

	if (pExplosion != nullptr)
	{
		pExplosion->m_fWidth = fWidth;
		pExplosion->m_fHeight = fHeight;
		pExplosion->Init();
		pExplosion->SetPos(pos);
		if (CStage::GetBomm() == false)
		{
			CSound::PlaySound(SOUND_LABEL_SE_BULLET_LASER_DAMAGE);
		}
	}
	return pExplosion;
}