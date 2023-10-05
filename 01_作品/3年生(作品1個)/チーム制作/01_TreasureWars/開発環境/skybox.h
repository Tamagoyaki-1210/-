//==============================================================================
//
// skybox.h
// Author : saito shian
//
//==============================================================================

//==============================================================================
// 2�d�C���N���[�h�K�[�h
//==============================================================================
#ifndef _SKYBOX_H_
#define	_SKYBOX_H_

//==============================================================================
// �C���N���[�h
//==============================================================================
#include"Object.h"
#include"texture.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_SIZE				(1400.0f)	//�T�C�Y�̍ő�l
#define MAX_SIDE_SIZE			(30)		//���̍ő�l
#define MIN_SIDE_SIZE			(50)		//���̍ŏ��l
#define START_SIDE				(16)		//���̍ŏ��̒l(16�p�`)
#define MAX_VERTICAL_SIZE		(30)		//�c�̍ő�l
#define MIN_VERTICAL_SIZE		(5)			//�c�̍ŏ��l
#define START_VERTICAL			(15)		//�c�̍ŏ��̒l
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//���W,�@��,�J���[	���e�N�X�`�����g���ꍇ�͂����ɒǉ�(�\���̂ɂ��ǉ�)

//==============================================================================
// �O���錾
//====================================== ========================================

//==============================================================================
// �X�J�C�{�b�N�X�N���X
//==============================================================================
class CSkyBox : public CObject
{
public:
	CSkyBox(int nPriority);
	~CSkyBox() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CSkyBox *Create(D3DXVECTOR3 pos,int nStageNum = -1);

	void Setposition(D3DXVECTOR3 pos);												// �ʒu�̐ݒ�
	void SetSize(D3DXVECTOR3 size);													// �T�C�Y�̐ݒ�

	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }				// �e�N�X�`���̐ݒ�
	void SetStageTexture(int nStageNum);											//�X�e�[�W���ƂɎg�p����w�i��ς���

private:
	// ���_�f�[�^
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;					//���_���W
		D3DXVECTOR3 nor;					//�@���x�N�g��
		D3DCOLOR col;						//���_�J���[
		D3DXVECTOR2 tex;					//�e�N�X�`�����W
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				//���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;				//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^											
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffCone;			//�~���̒��_�o�b�t�@�ւ̃|�C���^
	CTexture::TEXTURE m_texture;					//�e�N�X�`��
	D3DXMATRIX m_mtxWorld;							//���[���h�}�g���b�N�X
	D3DXVECTOR3 IdxPosNor[3];						//Idx��pos
	D3DXVECTOR3 Calculation3DNor[2];				//3�����O�ς̌v�Z����
	D3DXVECTOR3 VecAnswer;							//�O�ς̌v�Z����
	D3DXVECTOR3	m_pos;								//�ʒu
	D3DXVECTOR3 m_size;								//�T�C�Y
	D3DXVECTOR3	m_move;								//�ړ�
	D3DXVECTOR3 m_rot;								//��]
	D3DXCOLOR m_col;								//�F

	int	m_nHorizontal;								// ��
	int	m_nVertical;								// �c
	int	m_nVtx;										// ���_��
	int	m_nIdx;										// �C���f�b�N�X��
	int	m_nPolygon;									// �|���S����
};

#endif // !_OBJECT_H_
