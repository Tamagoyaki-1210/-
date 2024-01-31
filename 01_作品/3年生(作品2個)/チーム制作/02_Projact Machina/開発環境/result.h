//==============================================================================================
//
// リザルト　　　result.h
// tutida ryousei
// tanimoto kosuke
// Saito shian
//
//==============================================================================================
#ifndef _RESULT_H_
#define	_RESULT_H_

//==============================================================================================
// インクルード
//==============================================================================================
#include"mode.h"

//==============================================================================================
// 前方宣言
//==============================================================================================
class CHalfSphere;
class CObjectX;
class CPlayerManager;
class CPlayer_Parameter;
class CWeapon_Parameter;
class CObject2D;
class CParts;
class CFontString;
class CScore;
class CTime;
class CNumber;

//==============================================================================================
// リザルトクラス
//==============================================================================================
class CResult : public CMode
{
	const float SizeXScaleSpeed = 26.8f;			// xサイズ拡大縮小スピード
	const float SizeYScaleSpeed = 22.5f;			// yサイズ拡大縮小スピード
public:

	CResult();										// コンストラクタ
	~CResult();										// デストラクタ

	HRESULT Init() override;						// 初期化処理
	void Uninit() override;							// 終了処理
	void Update() override;							// 更新処理

	void SetTotalDamage(int num, D3DXVECTOR3 pos, D3DXVECTOR2 size);			// 受けたダメージの設定処理
	void SetDeathCount(int num, D3DXVECTOR3 pos, D3DXVECTOR2 size);				// 倒した敵の設定処理
	void ScaleExpansion();							// サイズ拡大処理
	void InformationUninit();						// 情報の破棄
	void ZoroDamageCount();
	void ZoroDeathCount();
			
	static CResult* Create();						// 生成処理
	//static CPlayer_Parameter *GetPlayerParameter() { return m_pPlayerParameter; }	// プレイヤーパラメータ取得処理
	static CPlayerManager* GetPlayerManager() { return m_pPlayerManager; }

private:
	//static CPlayer_Parameter *m_pPlayerParameter;	// プレイヤーパラメーター
	static CPlayerManager* m_pPlayerManager;		// プレイヤーマネージャー
	CWeapon_Parameter *m_pWeaponParameter;			// 武器パラメーター
	CParts* m_pParts[3];							// パーツ
	CHalfSphere* m_pHalfSphere;						// ハーフスフィア
	CObject2D* m_pObject2D;							// オブジェクト2D
	CObject2D* m_pFontTexture[3];					// フォントテクスチャ
	CScore* m_pScore;								// スコア
	CTime* m_pTime;									// タイム
	CNumber* m_apTotalDamage[6];					// ナンバー
	CNumber* m_apDeathCount[3];						// ナンバー
	D3DXVECTOR3 m_pos;								// 位置
	D3DXVECTOR3 m_move;								// 移動量
	D3DXVECTOR2 m_size;								// サイズ
	int m_aPosTexU[6];								//各桁のスコアを格納
	int m_nZeroCountTotalDamage;					// 0の数
	int m_nZeroCountDeathCount;					// 0の数
	int m_nNum;										// 値
	int m_MoveCount;								// 移動時間
	int m_PlayerIndex;								// プレイヤーの番号
	bool m_bGetFlag;								// 取得フラグ
	bool m_bCreateFlag;								// 生成フラグ
};

#endif // !_GAME_H_
