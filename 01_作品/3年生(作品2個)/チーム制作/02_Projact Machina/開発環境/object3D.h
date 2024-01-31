//==============================================================================================
//
// 3D�|���S���@�@�@Object3D.h
// Author : tutida ryousei
// Author : tanimoto kosuke
//
//==============================================================================================
#ifndef _OBJECT3D_H_
#define	_OBJECT3D_H_

#include <d3dx9.h>
#include"object.h"
#include"texture.h"

//==============================================
// �I�u�W�F�N�g3D�N���X
//==============================================
class CObject3D : public CObject
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

	CObject3D(const PRIORITY priority = PRIORITY_CENTER);
	~CObject3D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	void UV();

	static CObject3D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const PRIORITY priority = PRIORITY_CENTER, const D3DXCOLOR col = {1.0f,1.0f, 1.0f, 1.0f}, const bool billboard = false);

	void SetTexPos(const float top, const float row, const float right, const float left);			// �e�N�X�`���̕���

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }					// �ʒu�̐ݒ�
	void AddPos(const D3DXVECTOR3 pos) { m_pos += pos; }				// �ʒu�̉��Z
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(const D3DXVECTOR2 size) { m_size = size; }
	void SetSubSize(const D3DXVECTOR2 halfsize) { m_Subsize = halfsize; }			// ��������T�C�Y
	void SetCol(const D3DXCOLOR col) { m_col = col; }
	void SetAlpha(const float alpha) { m_col.a = alpha; }
	void AddAlpha(const float alpha) { m_col.a += alpha; }
	void Setbillboard(const bool billboard) { m_bBillboard = billboard; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }
	void SetVtxPos(D3DXVECTOR3 pos, int index) { VERTEX_3D *pVtx; m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0); pVtx[index].pos = pos; m_pVtxBuff->Unlock(); }

	const D3DXVECTOR3 GetPos() { return m_pos; }						// �ʒu�̎擾
	const D3DXVECTOR3 GetMove() { return m_move; }
	const D3DXVECTOR2 GetScale() { return m_size; }
	const D3DXCOLOR GetCol() { return m_col; }
	const D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }
	const D3DXVECTOR3 GetVtxPos(int index) { VERTEX_3D *pVtx; m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0); return pVtx[index].pos; }
	void VtxBuffUnlock() { m_pVtxBuff->Unlock(); }

private:

	// ���_�t�H�[�}�b�g
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR2 m_size;						// �T�C�Y
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXVECTOR3 m_rot;						// �p�x
	D3DXVECTOR2 m_Subsize;					// ��������T�C�Y
	D3DXCOLOR m_col;						// �F
	CTexture::TEXTURE m_texture;			// �e�N�X�`��
	bool m_bBillboard;						// �r���{�[�h��
};

#endif // !_OBJECT3D_H_