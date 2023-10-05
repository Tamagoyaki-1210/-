//==============================================================================================
//
//���f���̏���[model.h]
//Author�Ftutida ryousei
//
//==============================================================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include"Object.h"
#include <vector>

class CObjectX : public CObject
{
public:
	CObjectX(int nPriority = 3);
	~CObjectX();

	virtual HRESULT Init() override;
	void Uninit() override;
	virtual void Update() override;
	void Draw() override;

	static void UninitObject();
	void DrawShadow();		// �e
	static int ReadObject(const char* name);		// �I�u�W�F�N�g�̓ǂݍ���
	static CObjectX *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const char* Xfilename);

	void SetIndex(const int index);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void AddPos(D3DXVECTOR3 pos) { m_pos += pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void AddRot(D3DXVECTOR3 rot) { m_rot += rot; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }

	// �}�e���A���̐F���������}�e���A���F�ɖ߂�����
	void ResetColor() { for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] = m_ModelPattern[m_nIndex].MatColor[nCntMat]; } }
	void SetColor(const D3DXCOLOR col) { for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] = col; } }
	void AddColor(const D3DXCOLOR col) { for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] += col; } }
	void SetAlpha(const float alpha) { for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat].a = alpha; } }
	void AddAlpha(const float alpha) { for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat].a += alpha; } }

	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	D3DXVECTOR3 GetSize() { return m_Size; }
	D3DXVECTOR3 GetMax() { return m_vtxMaxModel; }
	D3DXVECTOR3 GetMin() { return m_vtxMinModel; }
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }

private:

	struct ModelPattern
	{
		LPD3DXMESH pMesh;					// ���b�V�����ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;				// �}�e���A�����ւ̃|�C���^
		DWORD nNumMat;						// �}�e���A�����̐�
		std::string fileName;				// �t�@�C����
		std::vector <D3DXCOLOR> MatColor;	// �����}�e���A���F���
		std::vector <LPDIRECT3DTEXTURE9> pTexture;		// �e�N�X�`���ւ̃|�C���^
	};
	static std::vector<ModelPattern> m_ModelPattern;

	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXVECTOR3 m_posOld;			// �O��̈ʒu
	D3DXVECTOR3 m_rot;				// ����
	D3DXVECTOR3 m_Initrot;			// �ŏ��̌���
	D3DXVECTOR3 m_Size;				// �T�C�Y
	D3DXVECTOR3 m_vtxMinModel;		// ���_�̍ŏ��l
	D3DXVECTOR3 m_vtxMaxModel;		// ���_�̍ő�l
	D3DXVECTOR3 m_vecDir;			// �v���C���[�̕����x�N�g��
	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxRot;			// ��]�}�g���b�N�X
	std::vector <D3DXCOLOR> m_col;	// ���f���̐F
	const char *m_FileName;			// �t�@�C���̖��O
	int m_nIndex;					// �I�u�W�F�N�g�̔ԍ�
	static bool m_bRelease;
};

#endif // !_OBJECTX_H_