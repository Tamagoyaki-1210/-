//==============================================================================================
//
// �O�Ձ@�@�@locus.h
// Author : koduna hirohito
//
//==============================================================================================
#ifndef _LOCUS_H_
#define	_LOCUS_H_

#include <d3dx9.h>
#include"object.h"
#include"texture.h"
#include"move_object.h"

class CLocus : public CObject
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
		bool broken;			//���b�V�������
	};

	CLocus(const PRIORITY priority = PRIORITY_CENTER);
	~CLocus() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	void UV();

	static CLocus* Create(const D3DXVECTOR3 pos, const float fsize, int nAnchorPoints, const PRIORITY priority = PRIORITY_CENTER, const D3DXCOLOR col = { 1.0f,1.0f, 1.0f, 1.0f }, const bool billboard = true);

	void SetTexPos(const float top, const float row, const float right, const float left);			// �e�N�X�`���̕���

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(const float fsize) { m_fsize = fsize; }
	void SetCol(const D3DXCOLOR col) { m_col = col; }
	void SetAlpha(const float alpha) { m_col.a = alpha; }
	void AddAlpha(const float alpha) { m_col.a += alpha; }
	void Setbillboard(const bool billboard) { m_bBillboard = billboard; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }
	void SetAnchorPoints(int nanchorpoints);

	const D3DXVECTOR3 GetMove() { return m_move; }
	const D3DXCOLOR GetCol() { return m_col; }
	const D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }

private:

	D3DXVECTOR3 m_pos;
	// ���_�t�H�[�}�b�g
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	float m_fsize;							// �T�C�Y
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXVECTOR3 m_rot;						// �p�x
	D3DXCOLOR m_col;						// �F
	CTexture::TEXTURE m_texture;			// �e�N�X�`��
	bool m_bBillboard;						// �r���{�[�h��
	int m_nAnchorPoint;						// �O�Ղ̋敪�̐�
	D3DXVECTOR3 *m_pAnchorPoints;			// �O�Ղ̋敪�̊�ʒu
};

#endif // !_LOCUS_H_