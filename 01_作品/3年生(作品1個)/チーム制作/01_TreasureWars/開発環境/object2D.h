//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _OBJECT2D_H_
#define	_OBJECT2D_H_

#include <d3dx9.h>
#include"Object.h"
#include"texture.h"

class CInputkeyboard;
class CTexture;

class CObject2D : public CObject
{
public:
	CObject2D(int nPriority);
	~CObject2D() override;

	virtual HRESULT Init() override;
	void Uninit() override;
	virtual void Update() override;
	void Draw()  override;

	void Stencil();
	void UV();																		// UV�̏���

	// ��`�����̕`��̏����ݒ� = 3
	static CObject2D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nPriority = 3);

	void Setposition(D3DXVECTOR3 pos);												// �ʒu�̐ݒ�
	void Addposition(D3DXVECTOR3 add) { m_Pos += add; }								// �ʒu�̉��Z�ݒ�
	D3DXVECTOR3 Getposition() { return m_Pos; }										// �ʒu�̐ݒ�
	void SetMove(D3DXVECTOR3 move);													// �ړ��ʂ̐ݒ�
	void SetSize(float width, float height);										// �T�C�Y�̐ݒ�
	void SetGaugeSize(float width, float width2);									// �Q�[�W�̕�
	void Setrot(float rot);															// �p�x�̐ݒ�
	void Addrot(float fAdd) { m_fRot += fAdd; }										// �p�x�̐ݒ�
	float GetRot() { return m_fRot; }
	void SetTexPos(float top, float row, float right, float left);					// �e�N�X�`�����W�̐ݒ�
	void AnimTexture(int nPattern, int nPatternMax);
	void SetAnimControl(const int nAnimColumn, const int nAnimLine);
	void SetAnimControl(const int nAnimCounter);
	void SetAnimDiv(const int nDivX, const int nDivY) { m_nAnimColumnDiv = nDivX; m_nAnimLineDiv = nDivY; }
	void SetTexture(CTexture::TEXTURE texture);										// �e�N�X�`���̐ݒ�
	bool SetDisappear(float size, float alpha, float sizespeed, float alphaspeed);	// ������ݒ�
	void SetCol(D3DXCOLOR col) { m_Col = col; }										// �F�̐ݒ�
	void SetAlpha(const float alpha) { m_Col.a = alpha; }							// �A���t�@�l�̐ݒ�
	void AddAlpha(const float alpha) { m_Col.a += alpha; }							// �A���t�@�l�̉��Z

	const D3DXVECTOR3 GetSize() { return{ m_fWidth, m_fHeight, 0.0f }; }
	const D3DXCOLOR GetColor() { return m_Col; }

protected:
	const float SCREEN_WIDTH = 1280;		// �X�N���[���̕�
	const float SCREEN_HEIGHT = 720;		// �X�N���[���̍���

private:
	struct VERTEX_2D		// ���_�f�[�^
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	};

	const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// ���_�t�H�[�}�b�g
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@
	float m_fWidth, m_fWidth2;				// ��
	float m_fHeight, m_fHeight2;			// ����
	float m_fInitWidth, m_fInitWidth2;		// ��(�����l)
	float m_fInitHeight, m_fInitHeight2;	// ����(�����l)
	float m_fAngle;							// �Ίp���̊p�x���Z�o����
	float m_fRot;							// �|���S���̊p�x
	int m_nAnimColumn;						// �摜�A�j���[�V�����̗�
	int m_nAnimColumnDiv;					// �摜�A�j���[�V�����̗�敪
	int m_nAnimLineDiv;						// �摜�A�j���[�V�����̍s�敪
	D3DXVECTOR3 m_Pos;						// �|���S���̈ʒu
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXCOLOR m_Col;						// �F
	CTexture::TEXTURE m_texture;			// �e�N�X�`��
	EObjType m_ObjType;						//�I�u�W�F�N�g�̃^�C�v
};

#endif // !_OBJECT_H_
