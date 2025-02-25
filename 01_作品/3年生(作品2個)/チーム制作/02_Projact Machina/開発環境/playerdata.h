//=============================================================================
//
// PlayerData.h (プレイヤーデータ.h)
// Author : Saito Shian
//
//=============================================================================
#ifndef  _PLAYERDATA_H_				//このマクロ定義がされてなかったら
#define  _PLAYERDATA_H_				//二重インクルード防止のマクロ定義

//=============================================================================
// インクルード
//=============================================================================
#include "main.h"
#include "object.h"
#include "d3dx9.h"
#include <vector>

//=============================================================================
// 前方宣言
//=============================================================================
class CPlayerManager;

//=============================================================================
// 確認ウィンドウクラス
//=============================================================================
class CPlayerData
{
public:
	CPlayerData();					// プレイヤーデータのコンストラクタ
	~CPlayerData();					// プレイヤーデータのデストラクタ

	HRESULT Init();					// 初期化処理
	void Uninit();					// 終了処理
	void Update();					// 更新処理

	int GetPlayerIndex() { return m_nPlayerIndex; }

private:
	int m_nPlayerIndex;				// プレイヤー番号
};
#endif