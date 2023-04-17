//=========================================
// 
// プレイヤーのヘッダーファイル
// Author YudaKaito
// 
//=========================================
#ifndef _PLAYER_H_			// このマクロ定義がされてなかったら
#define _PLAYER_H_			// 2重インクルード防止のマクロ定義

//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "disk.h"

//------------------------------------
// マクロ定義
//------------------------------------

//------------------------------------
// プレイヤーの種別
//------------------------------------
typedef enum
{
	PLAYERTYPE_1,
	PLAYERTYPE_2,
	PLAYERTYPE_3,
	PLAYERTYPE_4,
	PLAYERTYPE_MAX,
}PLAYERTYPE;

//------------------------------------
// 跳躍状態
//------------------------------------
typedef enum
{
	JUMP_NONE = 0,
	JUMP_NOW,
	JUMP_MAX
}JUMPSTATE;

//------------------------------------
// プレイヤー構造体の定義
//------------------------------------
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 move;					// 移動量
	JUMPSTATE jumpstate;				// 跳躍の状態
	DISK_TYPE typeSpecialDisk;			// 必殺技
	float fSize;						// 大きさ
	float fHeight;						// 高さ
	float fVerticalSpeed = 5.0f;		// 上昇スピード
	float fThrowCurvePower;				// 曲げて投げる力
	float fMaxThrowPower;				// 投げる最大の力
	float fThrowPower;					// 投げる最大の力
	DISK_TYPE SpecialSkillType;			// 必殺技のタイプ
	int nSpecialSkillCnt;				// 必殺技までのカウント
	float fMoveSpeed;					// 移動量
	float fSlidingVolume;				// スライディング量
	int nSlidingRigorCnt;				// スライディングの硬直時間のカウント
	int nSlidingRigorMax;				// スライディングの硬直時間
	float fAttenuationSlidingSpead;		// スライディング時の減衰値
	float fAttenuationMoveSpead;		// 移動時の減衰値
	int nIdxShadow;						// 影の使用番号
	bool bHaveDisk;						// ディスクを所持しているか
	bool bUseSliding;					// スライディング状態
	bool bUse;							// 表示状態
}Player;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(const D3DXVECTOR3& pos, PLAYERTYPE type);
bool ResetPosPlayer();	// 開始位置に戻る
Player* GetPlayer(void);

#endif // !_PLAYER_H_
