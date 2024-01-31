//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _OBJECT_H_
#define	_OBJECT_H_

#include <d3dx9.h>

class CObject
{
public:
	// ���_�t�H�[�}�b�g
	static const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);	//���W�A�@���A�J���[ �A�e�N�X�`��

	// �I�u�W�F�N�g�̎��
	enum EObjType
	{
		OBJTYPE_NONE = 0,
		//OBJTYPE_MODE,		// �Q�[�����[�h
		OBJTYPE_MAX
	};

	CObject(int nPriority = 3);
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void Release();
	void Death(int nPriority);
	float Distance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);		// ��̈ʒu�̋���

	bool CollisionCircle(const D3DXVECTOR3 pos, const float radius, const D3DXVECTOR3 otherPos, const float otherRadius);
	bool AttackCollision(const D3DXVECTOR3 pos, const float rot, const float length, const float angle, const D3DXVECTOR3 otherPos, const float otherRadius);
	bool CheckInAngle(const D3DXVECTOR3 pos, const float rot, const float angle, const D3DXVECTOR3 otherPos);

	CObject *GetNext() { return m_pNext; }
	static CObject *GetTop(int Pri) { return m_pTop[Pri]; }
	virtual EObjType GetObjType() { return OBJTYPE_NONE; }		// �I�u�W�F�N�g�^�C�v�̎擾
	bool GetDeathFlag() { return m_bDeath; }
	bool GetDrawFlag() { return m_bDraw; }
	void SetDrawFlag(bool flag) { m_bDraw = flag; }

private:
	static const int m_nMax_Pri = 6;
	static const float AttackLength_Decreace;	// �U���˒��̌���

	int m_nPriority;
	CObject *m_pPrev;						// ��O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pNext;						// ���̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject *m_pTop[m_nMax_Pri];		// �擪�̃I�u�W�F�N�g�̂ւ̃|�C���^
	static CObject *m_pCurrent[m_nMax_Pri];	// ����(��Ԍ��)�̃I�u�W�F�N�g�̂ւ̃|�C���^
	bool m_bDeath;							// ���S�t���O
	bool m_bDraw;
};

#endif // !_OBJECT_H_
