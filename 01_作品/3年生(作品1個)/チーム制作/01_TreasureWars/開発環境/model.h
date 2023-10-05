//==============================================================================================
//
// ���f���@�@�@madel.h
// tutida ryousei
//
//==============================================================================================
#ifndef _MODEL_H_
#define	_MODEL_H_

#include"renderer.h"
#include <vector>

class CModel
{
public:
	CModel();
	~CModel();

	HRESULT Init();
	void Uninit();
	void Update();
	D3DXMATRIX Draw();

	void DrawShadow(const LPDIRECT3DDEVICE9 device);
	void Stencil();
	static void ModelReleaseAll();
	int ReadObject(const char* name);		// �I�u�W�F�N�g�̓ǂݍ���

	static CModel* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel *parent, char *Xfilename);

	void SetIndex(const int index) { m_nIndex = index; }
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetShadow(const bool shadow) { m_bShadow = shadow; }
	void SetParent(CModel *pModel) { m_pParent = pModel; }

	// �}�e���A���̐F���������}�e���A���F�ɖ߂�����
	void ResetColor() { for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] = m_Model[m_nIndex].MatColor[nCntMat]; } }
	void SetColor(const D3DXCOLOR col) { for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] = col; } }
	void AddColor(const D3DXCOLOR col) { for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] += col; } }
	void SetAlpha(const float alpha) { for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat].a = alpha; } }
	void AddAlpha(const float alpha) { for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat].a += alpha; } }

	D3DXVECTOR3 GetPos() { return m_Pos; }
	D3DXVECTOR3 GetRot() { return m_Rot; }
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }
	CModel *GetParent() { return m_pParent; }
	D3DXVECTOR3 GetMaxSize() { return m_vtxMaxModel; }
	D3DXVECTOR3 GetMinSize() { return m_vtxMinModel; }

private:

	struct Model
	{
		LPD3DXBUFFER pBuffMat;				// �}�e���A�����ւ̃|�C���^
		LPD3DXMESH pMesh;					// ���b�V�����ւ̃|�C���^
		DWORD nNumMat;						// �}�e���A�����̐�
		const char* fileName;				// �t�@�C����
		std::vector <D3DXCOLOR> MatColor;	// �����}�e���A���F���
		std::vector <LPDIRECT3DTEXTURE9> pTexture;	// ���f���̃e�N�X�`���ւ̃|�C���^�̔z��
	};
	static std::vector <Model> m_Model;		// ���f���\����

	D3DXVECTOR3 m_Pos;			// �ʒu
	D3DXVECTOR3 m_Rot;			// ���݂̊p�x
	D3DXVECTOR3 m_Size;			// �T�C�Y
	D3DXVECTOR3 m_vtxMinModel;	// ���_�̍ŏ��l
	D3DXVECTOR3 m_vtxMaxModel;	// ���_�̍ő�l
	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X
	std::vector <D3DXCOLOR> m_col;		// ���f���̐F
	CModel *m_pParent;			// �e���f���ւ̃|�C���^
	const char *m_XFileName;	// X�t�@�C���̖��O
	int m_nIndex;				// X�t�@�C���̔ԍ�
	bool m_bShadow;				// �e�̕`�揈��
};

#endif // !_MODEL_H_
