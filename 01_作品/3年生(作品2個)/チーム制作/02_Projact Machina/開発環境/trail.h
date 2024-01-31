#ifndef _ORBIT_H_
#define _ORBIT_H_

#include "object.h"

class CTrail : public CObject
{
public:
	// 3D���_�t�H�[�}�b�g
	const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;		//���_���W
		D3DXVECTOR3 nor;		//�@���x�N�g��
		D3DCOLOR col;			//���_�J���[
		D3DXVECTOR2 tex;		//�e�N�X�`�����W
	};

	CTrail();	//�R���X�g���N�^
	~CTrail();	//�f�X�g���N�^

	HRESULT Init() override { return S_OK; }
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetTrail(D3DXMATRIX mtx, D3DXCOLOR col);
	void SetMtxParent(D3DXMATRIX pMtx) { m_pMtxParent = pMtx; };	//�e�̃}�g���b�N�X�̐ݒ�
	void SetPos(D3DXVECTOR3 offset) { m_offsetPos = offset; }	//�I�t�Z�b�g���W�̐ݒ�
	void SetColor(D3DXCOLOR col, D3DXCOLOR destcol);			//�F�̐ݒ�

	D3DXVECTOR3 GetPos() { return m_offsetPos; }	//�ʒu�̎擾

	static CTrail* Create(D3DXVECTOR3 offset, D3DXMATRIX mtx);

private:
	static const int nMaxVtx = 1024;	//���_�̍ő吔

	D3DXVECTOR3 m_offsetPos;			//�I�t�Z�b�g���W
	D3DXVECTOR3 m_worldPos;				//���[���h���W
	D3DXMATRIX	m_mtxWorld;				//���[���h�}�g���b�N�X
	D3DXMATRIX m_pMtxParent;			//�O�Ղ̐e���f���̃}�g���b�N�X
	D3DXCOLOR m_beginCol;				//�F�̊J�n�l
	D3DXCOLOR m_endCol;					//�F�̏I���l
	int m_nNumVtx;						//���_��

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
};

#endif