//==============================================================================================
//
// トロッコマネージャー　　　trolley_manager.h
// tutida ryousei
//
//==============================================================================================
#ifndef _TROLLEY_MANAGER_H_
#define	_TROLLEY_MANAGER_H_

#include"object.h"

class CGimmick_Model;

class CTrolley_Manager : public CObject
{
public:
	static const int Max_TurnPos = 10;	// 巡回地点の数
	static const int Max_StartPos = 5;		// 初期位置の数
	static const int MaxLevel = 4;			// トロッコの最大レベル

	CTrolley_Manager();
	~CTrolley_Manager();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void TrolleyDeath();	// トロッコの削除

	static CTrolley_Manager *Create(int type, int index, const char *Xfilename);

	void SetMove(bool move) { m_bMove = move; }
	void SetEnd(bool end) { m_bEnd = end; }
	void SetFileName(const char *Xfilename) { m_FileName = Xfilename; }

	bool GetMove() { return m_bMove; }
	D3DXVECTOR3 GetTurn_Pos(int cnt) { return m_Turn_Pos[cnt]; }
	D3DXVECTOR3 GetStart_Pos(int cnt) { return m_Start_Pos[cnt]; }
	int GetStartTurn_Index() { return m_nStartTurn_Index; }
	int GetTurnNum() { return m_nTurnNum; }
	int GetStartNum() { return m_nStartNum; }
	int GetRandStart() { return m_nRand_StartPos; }
	float GetSpeed(int cnt) { return m_fSpeed[cnt]; }
	float GetRadius() { return m_fRadius; }
	CGimmick_Model *GetGimmick_Model() { return m_pGimmick_Model; }

private:
	void SetTrolley();		// トロッコの設定
	void SetStartPos();		// 初期位置の設定

	void SetType(int type) { m_nType = type; }
	void SetIndex(int index) { m_nIndex = index; }

	D3DXVECTOR3 m_Pos;						// 位置
	D3DXVECTOR3 m_Start_Pos[Max_StartPos];	// 初期位置
	D3DXVECTOR3 m_Turn_Pos[Max_TurnPos];	// 巡回地点
	int m_nFirst_Index[Max_StartPos][2];	// 初期位置ごとの最初の巡回地点
	int m_nStartTurn_Index;		// 確定した初期位置
	int m_nType;				// タイプ
	int m_nStartNum;			// 初期位置の数
	int m_nTurnNum;				// 巡回地点の数
	int m_nFirstNum;			// 最初の目的地
	int m_nRand_StartPos;		// 最初のスタート地点
	int m_nCoolTime;			// クールタイム
	int m_nCoolTime_Count;		// クールタイムのカウント
	int m_nIndex;				// トロッコの番号
	float m_fSpeed[MaxLevel];			// 移動速度
	float m_fRadius;			// 当たり判定の半径
	bool m_bMove;				// 移動してるか
	bool m_bEnd;				// 終わったか
	CGimmick_Model *m_pGimmick_Model;			// トロッコの情報
	const char *m_FileName;
};

#endif // !_TROLLEY_H_