//==============================================================================================
// 
// �L�����N�^�[�@�@�@character.h
// tutida ryousei
// 
//==============================================================================================
#ifndef _CHARACTER_H_
#define	_CHARACTER_H_

#include"object.h"
#include"model.h"

class CGauge;
class CScore;
class CScoreItemStack;
class CObject3D;
class CObjectX;
class CShop_Direction;

class CCharacter : public CObject
{
public:
	static const int Max_GemStack = 20;				// �L�����̕�Ώ������̍ő吔
	static const float Attack_Angle;				// �L�����̍U���͈�(��^)

	CCharacter();
	~CCharacter();

	// ���[�V�����̃^�C�v
	enum Motion_Type
	{
		MOTION_NEUTRAL = 0,		// �j���[�g����
		MOTION_WALK,			// ����
		MOTION_MINE,			// �@��
		MOTION_KNOCKBACK,		// �m�b�N�o�b�N
		MOTION_MAX,
		MOTION_NONE
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	virtual void Move();	// �ړ�
	void SetMotionData(int maxkey, int key, int parts, int motion,
	int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop);				// ���[�V�����l�̓ǂݍ���
	void Motion();							// ���[�V�����̐ݒ�
	void ChangeMotion();		// ���[�V�����̐؂�ւ�
	CModel *SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename);	// ���f���p�[�c�̐ݒ�
	void ResetCharaColor();								// �L�������̐F�̏�����
	void SetCrown(const bool crown);	// �����̕\���ݒ�
	void NormalizeRot();	// �p�x�̐��K��
	void Walk();			// ����
	void Item_Effect(const int type);		// �A�C�e���擾���̌���
	void Attack();			// �U������
	void CMine();			// �̌@
	void KnockBack_Set(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, int knockback_power, int attackpower);	// �m�b�N�o�b�N�̐ݒ�
	void KnockBack();		// �m�b�N�o�b�N
	void Flash();			// ���G����
	void Damage(const int power);	// ��e����
	void SetStackScoreItem();	// �X�R�A�A�C�e��������
	void CollObject();		// �I�u�W�F�N�g�Ƃ̓����蔻��
	void Before_Pos();		// �ߋ��̈ʒu��ۑ�
	void TimeSlip();		// �^�C���X���b�v
	void TimeSlip_CoolTime();
	void ShopCollision();	// �����Ƃ̓����蔻��
	void EnemyCollision();	// �G�L�����Ƃ̓����蔻��
	void Shop_Direction();	// �����̕���
	void CharUx();			// �L������UX��ݒ肷��
	void Create_ScoreItem();	// �w�����X�R�A�A�C�e���̐�������

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// �ʒu
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }	// �p�x
	void SetRotDest(D3DXVECTOR3 rotdest) { m_rotDest = rotdest; }	// �ړI�̊p�x
	void SetSizeMin(D3DXVECTOR3 min) { m_SizeMin = min; }				// �T�C�Y�̍ŏ��l
	void SetSizeMax(D3DXVECTOR3 max) { m_SizeMax = max; }				// �T�C�Y�̍ő�l
	void SetMotion(Motion_Type motion) { m_Motion = motion; }			// ���[�V����
	void SetMoveSpeed_Level(int speedlevel) { m_MoveSpeed_Level = speedlevel; }			// �ړ����x�̃��x��
	void SetArmor_Level(int armorlevel) { m_nArmor_Level = armorlevel; }
	void SetGemStack_Level(int stacklevel) { m_nGemStack_Level = stacklevel; }			// ��x�Ɏ��Ă�z�΂̗ʂ̃��x��
	void SetNext(CCharacter *next) { m_pNextChar = next; }
	void SetMove(bool move) { m_bMove = move; }
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	void SetKnockBack(bool knodkback) { m_bKnockBack = knodkback; }
	void SetKnockBackMove(D3DXVECTOR3 knockbackmove) { m_KnockBack = knockbackmove; }
	void SetKnockBack_Level(int knockbacklevel) { m_nKnockBack_Level = knockbacklevel; }
	void SetAttack(bool attack) { m_bAttack = attack; }
	void SetFlashing(bool flashing) { m_bFlashing = flashing; }
	void SetDraw(bool draw) { m_bDraw = draw; }
	void SetMoveSpeed(float speed) { m_fMoveSpeed = speed; }
	void SetMinePower(int power) { m_nMinePower = power; }
	void SetIndex(const int index) { m_nCharacterIndex = index; }
	void SetTimeSlip(bool timeslip) { m_bUseTimeSlip = timeslip; }
	void SetTimeSlip_SaveTime(int save) { m_nTimeSlip_Count = save; }
	void SetPlayTimeSlip(bool playtimeslip) { m_bPlayTimeSlip = playtimeslip; }
	void SetTime_Elapsed(bool timeelapsed) { m_bTime_Elapsed = timeelapsed; }
	void SetZeroOver(bool zeroover) { m_bZeroOver = zeroover; }
	void SetInvocation(bool invocation) { Invocation_TimeSlip = invocation; }

	D3DXVECTOR3 GetPos() { return m_pos; }			// �ʒu
	D3DXVECTOR3 GetRot() { return m_rot; }			// �p�x
	D3DXVECTOR3 GetRotDest() { return m_rotDest; }	// �ړI�̊p�x
	D3DXVECTOR3 GetSizeMin() { return m_SizeMin; }						// �T�C�Y�̍ŏ��l
	D3DXVECTOR3 GetSizeMax() { return m_SizeMax; }						// �T�C�Y�̍ő�l
	Motion_Type GetMotion_Type() { return m_Motion; }					// ���[�V����
	const float GetRadius() { return m_fRadius; }						// �T�C�Y(���a)
	const float GetAttackLength() { return m_fAttackLength; }			// �U���˒�
	const float GetAttackSize() { return m_fAttackSize; }				// �U���͈�(�~�`)
	float GetMoveSpeed() { return m_fMoveSpeed; }						// �ړ����x
	int GetMoveSpeed_Level() { return m_MoveSpeed_Level; }				// �ړ����x�̃��x��
	int GetArmor_Level() { return m_nArmor_Level; }						// �A�[�}�[�̃��x��
	int GetNumGemStack() { return m_nGemStack; }						// ��x�Ɏ��Ă�z�΂̗�
	int GetGemStack_Level() { return m_nGemStack_Level; }				// ��x�Ɏ��Ă�z�΂̗ʂ̃��x��
	int GetNamStack_ScoreItem() { return m_nNum_StackScoreItem; }		// �w�����Ă�X�R�A�A�C�e���̐�
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }						// ���[���h�}�g���b�N�X

	CCharacter *GetNext() { return m_pNextChar; }						// ���̃L����
	bool GetKnockBack() { return m_bKnockBack; }						// �m�b�N�o�b�N
	int GetKnockBack_Level() { return m_nKnockBack_Level;}				// �m�b�N�o�b�N�̏���
	bool GetFlashing() { return m_bFlashing; }							// ���G
	bool GetMove() { return m_bMove; }									// �ړ�������
	bool GetAttack() { return m_bAttack; }								// �U��������
	int GetMinePower() { return m_nMinePower; }							// �̌@�U����
	int GetNum_StackScoreItem() { return m_nNum_StackScoreItem; }				// �����Ă���X�R�A�A�C�e���̐�
	const int GetIndex() { return m_nCharacterIndex; }					// �L�����N�^�[�̔ԍ�
	bool GetPlayTimeSlip() { return m_bPlayTimeSlip; }					// �^�C���X���b�v����
	bool GetInvocation() { return Invocation_TimeSlip; }
	const bool GetSafe() { return m_bSafe; }							// ���S�n�т̔���
	CScore *GetScore() { return m_pScore; }								// �X�R�A�̏��
	CScoreItemStack *GetGemStack() { return m_pScoreItemStack; }		// �X�R�A�A�C�e���X�^�b�N�̏��
	bool GetInTerritory() { return m_bInTerritory; }					// �w�n�����ǂ�����Get
	void SetInTerritory(bool bbInTerritory) { m_bInTerritory = bbInTerritory; }	// �w�n�����ǂ�����Set
	bool GetEntry() { return m_bEntry; }

private:
	static const int m_nNumParts = 14;		// �p�[�c��
	static const int m_nNumKey = 5;			// �L�[�̍ő吔
	static const int m_nNumMotion = MOTION_MAX;		// ���[�V������

	static const int First_Attack_Power = 3;		// �v���C���[�ւ̏����U����
	static const int KnockBack_MaxFrame = 60;		// �m�b�N�o�b�N���鎞��
	static const int Flashing_MaxFrame = 120;		// ���G����
	static const int Flashing_SlowDown = 4;			// �_�ł̌���
	static const int Attack_MaxFrame = 55;			// �U���̑S�̃t���[��
	static const int Attack_Swing = 10;				// �U���̐U�艺�낷�^�C�~���O
	static const int Attack_Hit = 15;				// �U���̔���
	static const int Sale_Start_Timer = 60;			// ���p�J�n�^�C�~���O
	static const int Sale_Next_Timer = 10;			// ���̔��p�^�C�~���O
	static const int m_nBefor_Frame = 300;			// �^�C���X���b�v�Ŗ߂鎞��
	static const int m_nTimeSlip_Speed = 3;			// �^�C���X���b�v�̑��x

	static const float Attack_Length;				// �L�����̏����U���˒�
	static const float Attack_Size;					// �L�����̏����U���͈�(�~�`)
	static const float Attack_LevelUp;				// �U���͏㏸�ɔ������p���[�A�b�v
	static const float Move_Rot_Speed;				// �ړ��̉�]���x
	static const float Character_Radius;			// �L�����̓����蔻��̃T�C�Y(���a)
	static const float Shop_Blink_Alpha_Speed;		// �����̃A���t�@�l�̓_�ł̑��x
	static const float Shop_Blink_Max_Alpha;		// �����̃A���t�@�l�̓_�ł̍ő�l
	static const float Shop_Col_Radius;				// �����̓����蔻��̑傫��

	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXVECTOR3 m_posOld;				// �ߋ��̈ʒu
	D3DXVECTOR3 m_rot;				// ���݂̊p�x
	D3DXVECTOR3 m_rotDest;			// �ړI�̊p�x
	D3DXVECTOR3 InitPos[m_nNumParts];			// �p�[�c�̏����ʒu
	D3DXVECTOR3 InitRot[m_nNumParts];			// �p�[�c�̏����p�x
	D3DXVECTOR3 m_SizeMin;						// ���_�̍ŏ��l
	D3DXVECTOR3 m_SizeMax;						// ���_�̍ő�l���[�V����
	D3DXVECTOR3 m_nBefore_Pos[m_nBefor_Frame];	// ���b�O�̈ʒu
	D3DXVECTOR3 m_nBefore_Rot[m_nBefor_Frame];	// ���b�O�̊p�x
	int m_nCurrentKey;							// ���݂̃L�[�ԍ�
	int m_nCurrentMotion;						// ���݂̃��[�V�����ԍ�
	int m_nCountMotion;							// ���[�V�����J�E���^�[
	int m_nTimeSlip_Count;						// �^�C���X���b�v�̃J�E���g
	static int m_nTimeSlip_CoolTime;			// �^�C���X���b�v�̃N�[���^�C��
	static int m_nTimeSlip_CoolTime_Count;		// �^�C���X���b�v�̃N�[���^�C���̃J�E���g
	int m_nTimeSlip_Frame;
	int m_nGemStack;							// ��x�Ɏ��Ă�z�΂̗�
	int m_nSaleCount;							// �W�F���𔄂�Ԋu
	int m_nShopBlinkAlphaCount;					// �����̃A���t�@�l�̓_�ł̊Ԋu
	float m_fRadius;							// �L�����̓����蔻��̑傫���̔��a
	float m_fAttackLength;						// �L�����̍U���˒�
	float m_fAttackSize;						// �L�����̍U���͈�(�~�`)
	float m_fMoveSpeed;							// �ړ����x
	float m_fRotSpeed;							// ��]�̃X�s�[�h
	int m_nNum_StackScoreItem;					// �������Ă���X�R�A�A�C�e���̐�
	float m_fCollisionRadius;					// �L�����̓����蔻��̑傫��
	float m_fSpeed;								// �ړ����x
	char *m_XFileName[m_nNumParts];				// X�t�@�C���l�[��
	bool m_bNullModel;							// ���f����null
	bool m_bMove;								// �����Ă��邩
	bool m_bUseTimeSlip;						// �^�C���X���b�v���邩
	bool m_bPlayTimeSlip;						// �^�C���X���b�v��
	bool m_bTime_Elapsed;						// ��莞�Ԍo��
	bool m_bZeroOver;							// 0�𒴂���
	static int m_nTimeSlip_Charindex;			// �^�C���X���b�v�𔭓������L�����̔ԍ�
	static bool Invocation_TimeSlip;			// �N�����^�C���X���b�v�𔭓�������
	Motion_Type m_Motion;						// ���[�V����
	CModel *m_pModel[m_nNumParts];				// ���f���̏��
	CCharacter *m_pNextChar;					// ���̃L�����N�^�[�̏��
	CObject3D *m_pCharacter_Type;				// �L�����N�^�[�̃^�C�v�̕\��

	bool m_bAttack;									// �U��������
	bool m_bKnockBack;								// �m�b�N�o�b�N������
	bool m_bFlashing;								// �_�ł��Ă邩
	bool m_bDraw;									// �`�悷�邩
	bool m_bSafe;									// ���S�n�т̒��ɂ��邩�ǂ���
	int m_nKnockBack_Power;							// �m�b�N�o�b�N�������
	int m_nWalkCounter;								// �����J�E���^�[
	int m_nFlashCount;								// �_�ł̊Ԋu
	int m_nKnockBack_Frame;							// �m�b�N�o�b�N���鎞�Ԃ̃J�E���g
	int m_nAttack_HitCount;							// �U�����I���܂ł̎���
	int m_nMinePower;								// �̌@�U����
	int m_nAttackPower;								// �v���C���[�ւ̍U����
	D3DXVECTOR3 m_Move;								// �ړ��ʂ̋���
	D3DXVECTOR3 m_KnockBack;						// �m�b�N�o�b�N�̋���
	D3DXCOLOR m_charaCol;							// �L�������̐F
	int m_nCharacterIndex;							// �L�����N�^�[�̔ԍ�
	int m_nProbability;								// �A�C�e�����h���b�v����m��
	CObjectX *m_Crown;								// ����
	CScore *m_pScore;								// �X�R�A�̏��
	CScoreItemStack *m_pScoreItemStack;				// �X�R�A�A�C�e���X�^�b�N�̏��
	CModel *m_pOreStock[Max_GemStack];
	CShop_Direction *m_Shop_Direction;
	bool m_bEntry;

	int m_MoveSpeed_Level;			// �ړ����x�̃��x��
	int m_nKnockBack_Level;			// �m�b�N�o�b�N�̃��x��
	int m_nArmor_Level;				// �A�[�}�[���x��
	int m_nGemStack_Level;			// ��x�Ɏ��Ă�z�Ηʂ̃��x��
	bool m_bInTerritory;			// �w�n�����ǂ���

	// �L�[
	struct KEY
	{
		D3DXVECTOR3 KeyPos;
		D3DXVECTOR3 KeyRot;
	};

	// �L�[�Z�b�g
	struct KEY_SET
	{
		int nFrame;
		KEY aKey[m_nNumParts];
	};

	// ���[�V�����Z�b�g
	struct MOTION_SET
	{
		int nMaxKey;			// �L�[�̑���
		bool bLoop;				// ���[�v���邩
		bool bStop;				// ���[�V�������~�܂�����
		KEY_SET aKeySet[m_nNumKey];
	};
	MOTION_SET m_MotionSet[m_nNumMotion];
};

#endif // !_CHARACTER_H_