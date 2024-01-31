//==============================================================================================
//
// プレイヤー　　　player.h
// tutida ryousei
//
//==============================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"character.h"

class CModel;
class CGauge;

class CPlayer : public CCharacter
{
public:
	CPlayer();
	~CPlayer() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CPlayer *Create(D3DXVECTOR3 pos, int index);

	void Move() override;		// 移動処理
	void Action();		// アクション

private:
	static const int First_Mine_Power = 10;	// 採掘攻撃力の初期値

	//D3DXVECTOR3 m_Pos;			// 位置
	D3DXVECTOR3 m_RotDest;		// 目的の角度
	D3DXVECTOR3 m_SizeMin;		// 頂点の最小値
	D3DXVECTOR3 m_SizeMax;		// 頂点の最大値
	D3DXVECTOR3 m_CPosV;		// カメラの視点の格納先
	bool m_bSell;				// 売却状態の判定
	bool m_bEntry;				// エントリーしたか
	int m_nDamage;				// 攻撃力
};

#endif // !_PLAYER_H_
