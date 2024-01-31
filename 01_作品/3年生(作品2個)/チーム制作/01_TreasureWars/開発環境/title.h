//==============================================================================================
//
// �^�C�g���@�@�@title.h
// tutida ryousei
//
//==============================================================================================
#ifndef _TITLE_H_
#define	_TITLE_H_

#include"mode.h"

class CTitleObj;

class CTitle : public CMode
{
	static const int TITLEORE_MAX = 20;	//�����������̍ő吔 
public:
	CTitle();
	~CTitle();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CTitle *Create();

private:
	// ���_�f�[�^
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;				//���_���W
		D3DXVECTOR3 nor;				//�@���x�N�g��
		D3DCOLOR col;					//���_�J���[
		D3DXVECTOR2 tex;				//�e�N�X�`�����W
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXVECTOR3 m_rot;					// ����
	D3DXMATRIX m_mtxRot;				// ��]�}�g���b�N�X
	D3DXMATRIX m_mtxWorld;				// ���[���h�}�g���b�N�X
	LPD3DXMESH m_pMesh;					// ���b�V�����ւ̃|�C���^
	DWORD m_nNumMat;					// �}�e���A�����̐�	

	int m_TitleObjModel;				// �z�΂̃��f��
	int m_ModelIdx;						// �D���̃��f��

	float m_RandX;						// �����_����x�l
	float m_RandY;						// �����_����y�l
};

#endif // !_GAME_H_
