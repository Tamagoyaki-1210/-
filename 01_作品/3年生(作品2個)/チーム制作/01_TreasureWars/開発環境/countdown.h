//==============================================================================================
//
// カウントダウン　　　countdown.h
// tutida ryousei
//
//==============================================================================================
#ifndef _COUNTDOWN_H_
#define	_COUNTDOWN_H_

class CNumber;
class CObject2D;

#include"object.h"

class CCountdown : public CObject
{
public:
	CCountdown();
	~CCountdown();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CCountdown *Create();

	void SetCountdown(int countTime);

private:
	CNumber *m_pNumber;			// ナンバー
	int m_nCountTime;			// 始まるまでの時間
	int m_nCountFrame;			// 時間のカウント
	int m_nStart_Count;			// スタートを表示する時間
	CObject2D *m_pObject2D;		// スタート
	D3DXVECTOR3 m_Start_Size;	// スタートのサイズ
	bool m_bDisplay_Start;		// スタートを表示したか
	bool m_bStartScale;			// スタートの拡大判定
};

#endif // !_COUNTDOWN_H_
