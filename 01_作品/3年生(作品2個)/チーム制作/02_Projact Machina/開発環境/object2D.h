//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object.h
// Author : tutida ryousei
//			tanimoto kosuke
//
//==============================================================================================
#ifndef _OBJECT2D_H_
#define	_OBJECT2D_H_

#include <d3dx9.h>
#include"Object.h"
#include"texture.h"

class CObject2D : public CObject
{
	const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// ���_�t�H�[�}�b�g

public:
	CObject2D(const PRIORITY priority);
	~CObject2D() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw()  override;

	void Stencil();
	void UV();																		// UV�̏���

	// ��`�����̕`��̏����ݒ� = 3
	static CObject2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const PRIORITY priority = PRIORITY_CENTER);

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }								// �ʒu�̐ݒ�
	void AddPos(const D3DXVECTOR3 add) { m_pos += add; }							// �ʒu�̉��Z�ݒ�
	void SetMove(const D3DXVECTOR3 move);											// �ړ��ʂ̐ݒ�
	void SetSize(const D3DXVECTOR2 size) { m_size = size; }							// �T�C�Y�̐ݒ�
	void SetSubSize(const D3DXVECTOR2 halfsize) { m_Subsize = halfsize; }			// ��������T�C�Y
	void SetRot(const D3DXVECTOR2 rot) { m_rot = rot; }							// �p�x�̐ݒ�
	void AddRot(const D3DXVECTOR2 add) { m_rot += add; }							// �p�x�̐ݒ�
	void SetTexPos(float top, float row, float right, float left);					// �e�N�X�`�����W�̐ݒ�
	void AnimTexture(int nPattern, int nPatternMax);
	void SetAnimControl(const int nAnimColumn, const int nAnimLine);
	void SetAnimControl(const int nAnimCounter);
	void SetAnimDiv(const int nDivX, const int nDivY) { m_nAnimColumnDiv = nDivX; m_nAnimLineDiv = nDivY; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }				// �e�N�X�`���̐ݒ�
	void SetCol(D3DXCOLOR col) { m_col = col; }										// �F�̐ݒ�
	void SetAlpha(const float alpha) { m_col.a = alpha; }							// �A���t�@�l�̐ݒ�
	void AddAlpha(const float alpha) { m_col.a += alpha; }							// �A���t�@�l�̉��Z

	D3DXVECTOR3 GetPos() { return m_pos; }						// �ʒu�̎擾
	const D3DXVECTOR2 GetSize() { return m_size; }				// �T�C�Y�̎擾
	const D3DXVECTOR2 GetRot() { return m_rot; }				// �p�x�̎擾
	const D3DXCOLOR GetColor() { return m_col; }				// �F�̎擾

private:
	struct VERTEX_2D		// ���_�f�[�^
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@
	int m_nAnimColumn;						// �摜�A�j���[�V�����̗�
	int m_nAnimColumnDiv;					// �摜�A�j���[�V�����̗�敪
	int m_nAnimLineDiv;						// �摜�A�j���[�V�����̍s�敪
	D3DXVECTOR3 m_pos;						// �|���S���̈ʒu
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXVECTOR2 m_size;						// �T�C�Y
	D3DXVECTOR2 m_Subsize;					// ��������T�C�Y
	D3DXVECTOR2 m_rot;						// �|���S���̊p�x
	D3DXCOLOR m_col;						// �F
	CTexture::TEXTURE m_texture;			// �e�N�X�`��
};

#endif // !_OBJECT_H_
