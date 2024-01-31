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

	// �`�揇�̎��
	enum PRIORITY
	{
		PRIORITY_NONE = 0,		// �`�揇_��ԉ�
		PRIORITY_BACK_GROUND,	// �`�揇_�w�i
		PRIORITY_BACK,			// �`�揇_��둤
		PRIORITY_MODEL,			// �`�揇_���f��
		PRIORITY_CENTER,		// �`�揇_����
		PRIORITY_FRONT,			// �`�揇_�O��
		PRIORITY_SCREEN,		// �`�揇_��ʎ�O
		PRIORITY_MAX,			// �`�揇�̍ő吔
	};

	// �񋓌^�ł̂ݕ`�揇�̎w�肪�\
	explicit CObject(const PRIORITY priority = PRIORITY_CENTER);
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void Release() { m_bDeath = true; }	// ���S�t���O�����Ă�
	void Death(const int nPriority);

	bool Sphere_Collision(const D3DXVECTOR3 pos, const float radius, const D3DXVECTOR3 otherPos, const float otherRadius);	// �~�̓����蔻��

	CObject* GetNext() { return m_pNext; }
	static CObject* GetTop(const int Pri) { return m_pTop[Pri]; }
	bool GetDeathFlag() { return m_bDeath; }
	bool GetDrawFlag() { return m_bDraw; }
	void SetDrawFlag(bool flag) { m_bDraw = flag; }

private:

	int m_nPriority;
	CObject* m_pPrev;							// ��O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNext;							// ���̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject* m_pTop[PRIORITY_MAX];		// �擪�̃I�u�W�F�N�g�̂ւ̃|�C���^
	static CObject* m_pCurrent[PRIORITY_MAX];	// ����(��Ԍ��)�̃I�u�W�F�N�g�̂ւ̃|�C���^
	bool m_bDeath;								// ���S�t���O
	bool m_bDraw;
};

#endif // !_OBJECT_H_
