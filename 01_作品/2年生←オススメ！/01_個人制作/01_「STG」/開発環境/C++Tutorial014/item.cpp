//=============================================================================
//
// �A�C�e��
// Author : tanimoto kosuke
//
//=============================================================================
#include "item.h"
#include "application.h"
#include "texture.h"
#include "player.h"
#include "score.h"
#include "matrix.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define ITEM_WIDTH	(50)
#define ITEM_HEIGHT	(50)

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CItem::CItem() : CObject2D(OBJECT_PRIORITY_CENTER), m_nLife(0), m_move(D3DXVECTOR3(-2.0f, 0.0f, 0.0f))
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CItem::~CItem()
{

}

//=====================================
//����������
//=====================================
void CItem::Init()
{
	CObject2D::Init();
	m_fWidth = ITEM_WIDTH;
	m_fHeight = ITEM_HEIGHT;

	//�I�u�W�F�N�g�ݒ菈��
	SetObject2D(CObject::TYPE_ITEM, m_fWidth, m_fHeight);
}

//=====================================
//�I������
//=====================================
void CItem::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CItem::Update()
{
	CObject2D::Update();

	//UV�ݒ�
	CObject2D::SetUV();

	//�ʒu�擾����
	D3DXVECTOR3 pos = CObject2D::GetPos();

	//�����擾����
	float fLength = CObject2D::GetLength();

	//�ʒu�X�V
	pos += m_move;

	//�ʒu�ݒ菈��
	SetPos(pos);

	//�ʒu�ݒ菈��
	SetLength(fLength);

	//���������炷
	m_nLife--;

	//�����v�Z
	if (m_nLife <= 0)
	{
		Destroy();
	}
	else if (m_nLife == 300)
	{
		m_type = ENERGY_NONE;
	}
	else if (m_nLife < 300)
	{
		BlinkColor();
	}

	//�����蔻��ݒ菈��
	Collision(pos, fLength);
}

//=====================================
//�`�揈��
//=====================================
void CItem::Draw()
{
	//�摜�擾����
	CApplication::GetTexture()->TextureType(CTexture::TYPE_ITEM);
	CObject2D::Draw();
}


//=====================================
//��ʐݒ菈��
//=====================================
void CItem::TypeSet()
{
	switch (m_type)
	{
	case  ENERGY_01:
		CObject2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		m_nLife = 600;
		break;
	case  ENERGY_02:
		CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		m_nLife = 600;
		break;
	case  ENERGY_03:
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		m_nLife = 1000;
		break;
	case  ENERGY_04:
		CObject2D::SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_nLife = 800;
		break;
	default:
		break;
	}
}

//=====================================
//�_�ŐF����
//=====================================
void CItem::BlinkColor()
{
	if (m_bBlink)
	{
		m_fBlinkCounter--;
	}
	else
	{
		m_fBlinkCounter++;
	}

	if (m_fBlinkCounter >= 1.0f / 0.2f)
	{
		m_bBlink = true;
		m_fBlinkCounter = 1.0f / 0.2f;
		m_nBlinkLoop++;
	}
	else if (m_fBlinkCounter <= 0.0f)
	{
		m_bBlink = false;
		m_fBlinkCounter = 0.0f;
	}

	//�_�ł����镔���̐F���v�Z
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fBlinkCounter * 0.2f));

	if (m_nBlinkLoop >= 4)
	{
		m_nBlinkLoop = 0;
	}
}

//=====================================
//��������
//=====================================
void CItem::Destroy()
{
	Uninit();
}

//=====================================
//�����蔻��ݒ菈��
//=====================================
void CItem::Collision(D3DXVECTOR3 pos, float fLength)
{
	CObject *pCenter = (CObject*)CObject2D::GetTop(OBJECT_PRIORITY_CENTER);	//�I�u�W�F�N�g�̐擪�|�C���^
	if (pCenter != nullptr)
	{
		//���݈ʒu�|�C���^�ɐ擪�|�C���^����
		CObject *pObject = pCenter;

		//���݈ʒu�|�C���^���g�p���̏ꍇ
		while (pObject != nullptr)
		{
			CObject::Object_Type objtype = pObject->GetType();

			if (objtype == CObject::TYPE_PLAYER)
			{//�v���C���[�̏ꍇ
				if (CollisionCircle(pos, fLength, pObject->GetPos(), pObject->GetLength()))
				{//�d�Ȃ����ꍇ
					CSound::PlaySound(SOUND_LABEL_SE_ITEM_GET);

					CPlayer *pPlayer = (CPlayer*)pObject;
					CScore *pScore = CScore::GetScore();

					switch (m_type)
					{
					case  ENERGY_NONE:
						pScore->AddScore(500);
						break;
					case  ENERGY_01:
						//��e����
						pPlayer->AddLife(20);
						break;
					case  ENERGY_02:
						//���x�㏸����
						pPlayer->AddSpeed(0.1f);
						break;
					case  ENERGY_03:
						//�i����������
						pPlayer->AddBullet();
						break;
					case  ENERGY_04:
						//�i����������
						pPlayer->AddMaxLife(10);
						break;
					default:
						break;
					}
					//�A�C�e���j��
					Destroy();
				}
			}
			//���݈ʒu�|�C���^�Ɏ���|�C���^��������(�|�C���^��i�߂鏈��)
			pObject = pObject->GetNext();
		}
	}
}

////=====================================
////�~�^�����蔻��v�Z����
////=====================================
//bool CItem::CollisionCircle(D3DXVECTOR3 pos, float fLength, D3DXVECTOR3 posSub, float fLengthSub)
//{
//	float fSumLengthRad = (fLength + fLengthSub) * 0.5f;				//�~�̔��a���m�̘a
//
//	float fDiffX = pos.x - posSub.x;									//X���W�̍���
//	float fDiffY = pos.y - posSub.y;									//Y���W�̍���
//	float fDiffLength = sqrtf((fDiffX * fDiffX) + (fDiffY * fDiffY));	//���݂�2�_�̋���
//
//	return fSumLengthRad >= fDiffLength;
//}

//=====================================
//�G�L�����e��������
//=====================================
CItem *CItem::Create(D3DXVECTOR3 pos, Item_Type settype)
{
	CItem* pItem = new CItem;	//�e�̃|�C���^

	if (pItem != nullptr)
	{
		pItem->Init();
		pItem->SetPos(pos);
		pItem->m_type = settype;
		pItem->TypeSet();	//��ʏ���
	}
	return pItem;
}