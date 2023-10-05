//=============================================================================
//
// �摜
// Author : tanimoto kosuke
//
//=============================================================================
#include "texture.h"
#include "renderer.h"
#include "application.h"

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CTexture::CTexture()
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CTexture::~CTexture()
{

}

//=====================================
//�I������
//=====================================
void CTexture::Uninit()
{
	for (int nInd = 0; nInd < TYPE_MAX; nInd++)
	{
		if (m_pTextureType[nInd] == nullptr)
		{
			continue;
		}
		//m_pTexture[nInd]->Release();
		m_pTextureType[nInd] = nullptr;
	}
}

//=====================================
//�ǂݍ��ݏ���
//=====================================
void CTexture::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//����
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\BackGround\\white_screen.jpg", &m_pTextureType[TYPE_NONE]);

	//�v���C���[
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Player\\PlayerAnim.png", &m_pTextureType[TYPE_PLAYER]);

	//�G�L����
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Enemy\\Enemy01Anim.png", &m_pTextureType[TYPE_ENEMY_01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Enemy\\Enemy02Anim.png", &m_pTextureType[TYPE_ENEMY_02]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Enemy\\Enemy03Anim.png", &m_pTextureType[TYPE_ENEMY_03]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Enemy\\Enemy04Anim.png", &m_pTextureType[TYPE_ENEMY_04]);

	//��Q��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Obstacle\\Obstacle01.png", &m_pTextureType[TYPE_OBSTACLE_01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Obstacle\\Obstacle02.png", &m_pTextureType[TYPE_OBSTACLE_02]);

	//�e
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Bullet\\Bullet01.png", &m_pTextureType[TYPE_BULLET_01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Bullet\\white_screen.jpg", &m_pTextureType[TYPE_BULLET_02]);

	//�����G�t�F�N�g
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Ray\\effect105.png", &m_pTextureType[TYPE_RAY_01]);

	//����
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Explosion\\Explosion01.png", &m_pTextureType[TYPE_EXPLOSION]);

	//�A�C�e��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Item\\item001.png", &m_pTextureType[TYPE_ITEM]);

	//�w�i
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\BackGround\\white_screen.jpg", &m_pTextureType[TYPE_BG]);

	//�w�i(�`���[�g���A��)
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\BackGround\\Tutorial01.png", &m_pTextureType[TYPE_BG_Tutorial01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\BackGround\\Tutorial02.png", &m_pTextureType[TYPE_BG_Tutorial02]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\BackGround\\Tutorial03.png", &m_pTextureType[TYPE_BG_Tutorial03]);

	//�X�R�A
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Score\\frame_score.png", &m_pTextureType[TYPE_SCORE]);

	//�����L���O
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Ranking\\ranking.png", &m_pTextureType[TYPE_RANKING]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Message\\ThisNumber.png", &m_pTextureType[TYPE_THISNUMBER]);

	//�^�C�g�����S
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Message\\Title.png", &m_pTextureType[TYPE_TITLE]);

	//�|�[�Y���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Message\\Pause.png", &m_pTextureType[TYPE_PAUSE]);

	//�R���}
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Number\\comma.png", &m_pTextureType[TYPE_COMMA]);

	//�i���o�[
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Number\\number000.png", &m_pTextureType[TYPE_NUMBER]);

	//�̗�
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Life\\white_screen.jpg", &m_pTextureType[TYPE_LIFE]);

	//�Q�[�W
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Gauge\\white_screen.jpg", &m_pTextureType[TYPE_GAUGE]);

	//�I����
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Choice\\Choice_TitleStart.png", &m_pTextureType[TYPE_CHOICE_01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Choice\\Choice_TitleTutrial.png", &m_pTextureType[TYPE_CHOICE_02]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Choice\\Choice_TitleExit.png", &m_pTextureType[TYPE_CHOICE_03]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Choice\\Choice_GameContinue.png", &m_pTextureType[TYPE_CHOICE_04]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Choice\\Choice_GameExit.png", &m_pTextureType[TYPE_CHOICE_05]);
}

//=====================================
//�摜��ޏ���
//=====================================
void CTexture::TextureType(Texture_Type textype)
{
	//�e�N�X�`���̐ݒ�
	CApplication::GetRenderer()->GetDevice()->SetTexture(0, m_pTextureType[textype]);
}