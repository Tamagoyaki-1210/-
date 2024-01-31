//==============================================================================
//
// oremanager.h	(鉱石マネージャー.h)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2重インクルードガード
//==============================================================================
#ifndef _OREMANAGER_H_
#define	_OREMANAGER_H_

//==============================================================================
// インクルード
//==============================================================================
#include <list>
#include "main.h"
#include "object.h"

//==============================================================================
// マクロ定義
//==============================================================================

//==============================================================================
// 前方宣言
//==============================================================================
class COre;

//==============================================================================
// マップクラス
//==============================================================================
class COreManager : public CObject
{
public:
	static const float Ore_Radius;		// 鉱石の当たり判定の大きさ(半径)

	COreManager();						// コンストラクタ
	~COreManager()override;				// デストラクタ

	HRESULT Init()override;				// 初期化処理
	void Uninit()override;				// 終了処理
	void Update()override;				// 更新処理
	void Draw()override;				// 描画処理

	COre* OreCollision(const D3DXVECTOR3 pos, const float radius);
	void RandCreate();					// ランダムで生成する処理
	void InitCreate();					// 初期化時に生成する処理

	void OreClear();					// 鉱石のリストを整理する処理
	void OreDeath(COre* ore);			// 鉱石削除処理
	void SetStageNum(int nStageNum) { m_nStageNum = nStageNum; }  //ステージ番号の保存

	std::list <COre*> GetOreList() { return m_pOreList; }	// 鉱石リストの取得

	static COreManager *Create(int nStageNum);		// 生成処理

private:
	static const int RANDCREATE_MAX = 10;	//初期生成時の最大数
	static const int REAPPEARANCE_MAX;		//鉱石の再出現数
	static const int CREATE_TIME;			//鉱石が生成されるまでの時間

	std::list <COre*> m_pOreList;		// 鉱石のリスト

	int m_CreateTime;					// 生成されるまでの時間
	int m_UninitTime;					// 消すまでの時間

	float m_RandX;						// ランダムのx値
	float m_RandY;						// ランダムのy値
	float m_RandZ;						// ランダムのz値
	int m_nStageNum;					//ステージ番号
};

#endif