//=============================================================================
//
// 3DPolygon.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _3DPOLGON_H_			//���̃}�N����`������Ȃ�������
#define _3DPOLGON_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"
#include"texture.h"


//*****************************************************************************
// �N���X��`
//*****************************************************************************
class C3DPolygon : public CObject
{
private:
	static const float FLASHING_AMOUNT;

public:

	// ���_���[3D]�̍\���̂��`
	typedef struct
	{
		D3DXVECTOR3 pos;//���_���W
		D3DXVECTOR3 nor;//�@���x�N�g��
		D3DCOLOR col;//���_�J���[
		D3DXVECTOR2 tex;		//�e�N�X�`�����W
	}VERTEX_3D;

	C3DPolygon();//�R���X�g���N�^
	virtual ~C3DPolygon() override;//�f�X�g���N�^

	virtual HRESULT Init() override;//������
	virtual void Uninit() override;//�I������
	virtual void Update() override;//�X�V����
	virtual void Draw() override;//�`�揈��

	virtual bool IsUnused() { return true; }//�I����������
	virtual void SetUp(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move);//�Z�b�g�A�b�v

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	D3DXVECTOR3 GetPos() { return m_Pos; }
	void AddPos(D3DXVECTOR3 pos) { m_Pos += pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void AddRot(D3DXVECTOR3 addrot) { m_Rot += addrot; }
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	void SetUV(D3DXVECTOR2 uv) { m_UV = uv; }
	D3DXVECTOR2 GetUV() { return m_UV; }
	void AddUVPos(D3DXVECTOR2 uvmove) { m_UV += uvmove; }
	void SetUVMove(D3DXVECTOR2 uvmove) { m_UVMove = uvmove; }
	void SetUVSize(D3DXVECTOR2 uvsize) { m_UVSize = uvsize; }
	void SetColor(D3DXCOLOR color) { m_Color = color; }
	void AddColor(D3DXCOLOR color) { m_Color += color; }
	D3DXCOLOR GetColor() { return m_Color; }
	D3DXVECTOR3 GetRot() { return m_Rot; }
	void SetFlashing(int nflashing) { m_nFlashingCnt = nflashing; }

	virtual void SetDiagonalLine(float fwidth, float fheight);//�\������|���S���̏c�����ݒ�
	void AddDiagonalLine(float faddwidth, float faddheight);//�\������|���S���̏c����Add

	virtual void SetPolygon();//�X�V�����ȊO�ł̃|���S���̃Z�b�g
	void SetTextIndex(CTexture::TEXTURE Index) { m_TextIndex = Index; }//���̃|���S�����g�p����e�N�X�`���ԍ��̃Z�b�g
	CTexture::TEXTURE GetTextIndex() { return m_TextIndex; }//���̃|���S�����g�p����e�N�X�`���ԍ��̎擾

	static C3DPolygon *C3DPolygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXCOLOR col);
private:

	D3DXMATRIX m_MtxWorld;				//���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^
	float m_fWidth;//����
	float m_fHeight;//�c��
	D3DXVECTOR2 m_UV;//UV�̍��W
	D3DXVECTOR2 m_UVSize;//UV�̃T�C�Y
	D3DXCOLOR m_Color;//�F
	CTexture::TEXTURE m_TextIndex;//�e�N�X�`���ԍ�
	D3DXVECTOR3 m_Pos;//�ʒu
	D3DXVECTOR3 m_Rot;//����
	D3DXVECTOR3 m_Move;//�ړ���
	D3DXVECTOR2 m_UVMove;//UV�̈ړ���
	int m_nFlashingCnt;//�_�ŃJ�E���g
	bool m_bFadeInOut;//�t�F�[�h���(true = in,false = out)


};

#endif