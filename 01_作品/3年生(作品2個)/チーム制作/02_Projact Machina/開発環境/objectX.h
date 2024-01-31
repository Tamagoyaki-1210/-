//==============================================================================================
//
// �I�u�W�F�N�gX�@�@�@objectX.h
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================
#ifndef _OBJECTX_H_
#define	_OBJECTX_H_

#include"object.h"
#include "model.h"
#include <vector>

class CObjectX : public CObject
{
public:
	CObjectX(const PRIORITY priority);
	virtual ~CObjectX() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void DrawShadow(const LPDIRECT3DDEVICE9 device, CModel::ModelPattern model);
	void Stencil();

	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, const PRIORITY priority = PRIORITY_MODEL);
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const int index, const PRIORITY priority = PRIORITY_MODEL);

	void SetShadow(const bool shadow) { m_bShadow = shadow; }
	void SetParts(const bool parts) { m_bParts = parts; }
	void SetIndex(const int index) { m_nIndex = index; }
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void AddPos(const D3DXVECTOR3 pos) { m_pos += pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void AddRot(const D3DXVECTOR3 rot) { m_rot += rot; }
	void SetSize(const D3DXVECTOR3 size) { m_size = size; }
	void SetParent(CObjectX* pModel) { m_pParent = pModel; }
	void SetModel(const int index);
	void SetWireFrame(bool bwireFrame) { m_bWireFrame = bwireFrame; }

	// �}�e���A���̐F���������}�e���A���F�ɖ߂�����
	void SetColor(const D3DXCOLOR col) { m_col = col; }
	void AddColor(const D3DXCOLOR col) { m_col += col; }
	void SetAlpha(const float alpha) { m_col.a = alpha; }
	void AddAlpha(const float alpha) { m_col.a += alpha; }

	const D3DXVECTOR3 GetPos() { return m_pos; }
	const D3DXVECTOR3 GetMove() { return m_move; }
	const D3DXVECTOR3 GetRot() { return m_rot; }
	const D3DXMATRIX GetMatrix() { return m_mtxWorld; }
	CObjectX* GetParent() { return m_pParent; }
	const D3DXVECTOR3 GetMaxSize() { return m_vtxMaxModel; }
	const D3DXVECTOR3 GetMinSize() { return m_vtxMinModel; }

private:

	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_rot;			// ���݂̊p�x
	D3DXVECTOR3 m_move;			// �ړ���
	D3DXVECTOR3 m_size;			// �T�C�Y
	D3DXVECTOR3 m_vtxMinModel;	// ���_�̍ŏ��l
	D3DXVECTOR3 m_vtxMaxModel;	// ���_�̍ő�l
	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXCOLOR m_col;				// ���f���̐F
	std::vector <D3DXCOLOR> m_Original_col;		// ���f���̌��̐F
	CObjectX* m_pParent;			// �e���f���ւ̃|�C���^
	int m_nIndex;				// X�t�@�C���̔ԍ�
	int m_nNumMat;				// ���݂̃��f���̃}�e���A����
	bool m_bShadow;				// �e�̕`�揈��
	bool m_bParts;				// �p�[�c�p�̃��f���ł��邩�ǂ���
	bool m_bWireFrame;			// ���C���[�t���[��
};

#endif // !_MODEL_H_
