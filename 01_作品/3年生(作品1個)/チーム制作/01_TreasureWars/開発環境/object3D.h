//==============================================================================================
//
// 3D�|���S���@�@�@Object3D.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _OBJECT3D_H_
#define	_OBJECT3D_H_

#include <d3dx9.h>
#include"object.h"
#include"texture.h"

class CInputkeyboard;
class CTexture;


class CObject3D : public CObject
{
public:
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;		//���_���W
		D3DXVECTOR3 nor;		//�@���x�N�g��
		D3DCOLOR col;			//���_�J���[
		D3DXVECTOR2 tex;		//�e�N�X�`�����W
	};

	CObject3D(int nPriority = 3);
	~CObject3D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col = {1.0f,1.0f, 1.0f, 1.0f}, int priority = 3, bool billboard = false);

	void SetTexPos(float top, float row, float right, float left);			// �e�N�X�`���̕���

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; m_SizeX = size.x; }
	void SetSizeX(float sizeX) { m_SizeX = sizeX; }
	void SetCol(D3DXCOLOR col) { m_Col = col; }
	void SetAlpha(float alpha) { m_Col.a = alpha; }
	void AddAlpha(float alpha) { m_Col.a += alpha; }
	void Setbillboard(bool billboard) { m_bBillboard = billboard; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }

	D3DXVECTOR3 GetPos() { return m_Pos; }
	D3DXVECTOR3 GetScale() { return m_Size; }
	D3DXCOLOR GetCol() { return m_Col; }
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }

protected:
	const float SCREEN_WIDTH = 1280;		// �X�N���[���̕�
	const float SCREEN_HEIGHT = 720;		// �X�N���[���̍���

private:
	// ���_�t�H�[�}�b�g
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_Size;						// �T�C�Y
	float m_SizeX;							// �����ς���p
	float m_SizeZ;							// ���s
	D3DXVECTOR3 m_Pos;						// �|���S���̈ʒu
	D3DXVECTOR3 m_Rot;						// �p�x
	D3DXCOLOR m_Col;						// �F
	CTexture::TEXTURE m_texture;			// �e�N�X�`��
	bool m_bBillboard;						// �r���{�[�h��
	EObjType m_ObjType;						// �I�u�W�F�N�g�̃^�C�v
};

#endif // !_OBJECT3D_H_