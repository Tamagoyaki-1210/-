//==============================================================================================
//
// �Q�[���@�@�@game.h
// tutida ryousei
//
//==============================================================================================
#ifndef _GAME_H_
#define	_GAME_H_

#include"mode.h"
#include"charmanager.h"

// �O���錾
class CMap;
class CCharacter;
class CScore;
class CTime;
class CShop;
class CObject2D;
class COreManager;
class CEnemyManager;
class CEnemySpawner;
class CScoreItemManager;
class CItemManager;
class CArea;
class CFever_Time;

class CGame : public CMode
{
public:
	static const int FeverStart_Count = 30;	// �t�B�[�o�[�^�C�����n�܂鎞��

	CGame();
	~CGame();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void CharaRanking();		// �L�����̏��ʏ���
	void EnemySpawn();			// �G�L�����̐�������
	void SetFeverTime();		// �t�B�[�o�[�^�C���̐ݒ�
	static void EnemySpawnerDeath();	// �G�L�����X�|�i�[����������

	static CGame *Create();

	static CTime *GetTime() { return m_pTime; }							// ����
	static CShop *GetShop(const int index) { return m_pShop[index]; }	// ����
	static COreManager *GetOreManager() { return m_pOreManager; }		// �z��
	static CEnemyManager *GetEnemyManager() { return m_pEnemyManager; }	// �G�L�����}�l�[�W���[
	static CEnemySpawner *GetEnemySpawner() { return m_pEnemySpawner; }	// �G�L�����X�|�i�[
	static CScoreItemManager *GetScoreItemManager() { return m_pScoreItemManager; }	// �X�R�A�A�C�e��
	static CItemManager *GetItemManager() { return m_pItemManager; }	// �A�C�e��
	static CMap *GetMap() { return m_pMap; }							// �}�b�v
	static CFever_Time *GetFever_Time() { return m_pFever_Time; }		// �t�B�[�o�[�^�C��
	static CArea *GetArea() { return m_pArea; }							// �G���A

private:
	static const int EnemySpawner_Timer = 360;	// �G�L�����X�|�i�[�̍Đ�������
	static const int GemCreate_Count = 5;	// ��΂𐶐�����Ԋu

	int m_nTop_Score;			// ��ԍ����X�R�A
	int m_nEnemySpawnerCounter;	// �G�L�����̍Đ����J�E���^�[
	static CArea *m_pArea;
	static CMap *m_pMap;
	static CTime *m_pTime;
	static CShop *m_pShop[CCharManager::Max_Character];
	static COreManager *m_pOreManager;
	static CEnemyManager *m_pEnemyManager;
	static CEnemySpawner *m_pEnemySpawner;
	static CScoreItemManager *m_pScoreItemManager;
	static CItemManager *m_pItemManager;
	static CFever_Time *m_pFever_Time;

	int m_nStage_Number;
	int m_nTopCameraNum;
};

#endif // !_GAME_H_