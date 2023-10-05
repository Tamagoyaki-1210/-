//==============================================================================
//
// title_obj.h	(タイトルのオブジェクト.h)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2重インクルードガード
//==============================================================================
#ifndef _TITLE_OBJ_H_
#define	_TITLE_OBJ_H_

//==============================================================================
// インクルード
//==============================================================================
#include "main.h"
#include "objectX.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//座標,法線,カラー	※テクスチャも使う場合はここに追加(構造体にも追加)

//==============================================================================
// 前方宣言
//==============================================================================
class CScore;
class CResult;

//==============================================================================
// マップクラス
//==============================================================================
class CTitleObj : public CObjectX
{
	static const float MOVING_ORESPEED;
	static const float MOVING_PILEOFBILLSSPEED;
	static const int CREATE_TIME;

public:
	enum ObjType
	{
		ObjType_None = 0,
		ObjType_Ore,				// 鉱石
		ObjType_Pileofbills,		// 札束
		ObjType_Max
	};
	CTitleObj();					// コンストラクタ
	~CTitleObj();					// デストラクタ

	HRESULT Init()override;			// 初期化処理
	void Uninit()override;			// 終了処理
	void Update()override;			// 更新処理

	void SetObjType(ObjType type) { m_ObjType = type; }	// オブジェクトタイプの設定 }

	static CTitleObj *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, char *Xfilename, ObjType type);

private:
	D3DXVECTOR3 m_OreMove;				// 鉱石の移動量
	D3DXVECTOR3 m_PileofbillsMove;		// 札束の移動量
	D3DXVECTOR3 m_rot;					// 回転
	D3DXVECTOR3 m_pos;					// 位置

	ObjType m_ObjType;					// オブジェクトタイプ

	int m_PileofbillsTime;				// 札束が出てくるまでの時間

	float m_RandX;						// ランダムのx値
	float m_RandY;						// ランダムのy値
	float m_RandRotX;					// 回転のランダムx値
};

#endif

