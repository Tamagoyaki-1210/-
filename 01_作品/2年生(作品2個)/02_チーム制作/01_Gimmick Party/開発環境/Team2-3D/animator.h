//=============================================================================
//
// animator.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef ANIMATOR_H
#define ANIMATOR_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include <vector>

//=============================================================================
//前方宣言
//=============================================================================
class CModelPart;

class CAnimator
{
public:

	//アニメーションの種類
	enum AnimType
	{
		ANIM_TYPE_PLAYER = 0,					//プレイヤーアニメーション
		ANIM_TYPE_MAX
	};

	//キーの情報
	struct AnimKey
	{
		D3DXVECTOR3 RelativePos;				//キーの位置
		D3DXVECTOR3 RelativeRot;				//キーの回転
	};

	//キーセットの情報
	struct AnimKeySet
	{
		int nAnimFrames;						//アニメーションフレーム数
		std::vector <AnimKey> vAnimKey;			//アニメーションキー
	};

	//モーションセットの情報
	struct MotionSet
	{
		int nNextAnim;							//次のアニメーションの番号
		std::vector <AnimKeySet> vAnimKeySet;	//キーセット
	};

	//アニメーションの情報
	struct ModelAnimationSet
	{
		std::vector <MotionSet> vAnimSets;		//モーションセット
	};

	CAnimator();					//コンストラクタ
	~CAnimator();					//デストラクタ

	HRESULT Init(void);				//初期化処理
	void Uninit(void);				//終了処理
	void Update(void);				//更新処理

	void SetParts(std::vector <CModelPart*>* vParts);		//パーツの設定処理

	static void LoadAllAnimation(void);						//全部のアニメーションの読み込む処理
	static void DestroyLoadedAnimation(void);				//全部のアニメーションの破棄処理
	static CAnimator* Create(std::vector <CModelPart*>* vParts, AnimType type);				//生成処理

	void SetPresentAnim(int nPresentAnim);					//アニメーションの設定処理
	void SetLoopingAnim(int nPresentAnim);					//アニメーションの設定処理(アニメーションが終わった場合、元に戻す)
private:

	//アニメーションスピード
	struct animSpeed
	{
		D3DXVECTOR3 move;									//移動量
		D3DXVECTOR3 rotSpeed;								//回転スピード
	};

	static char* m_pAnimPass[ANIM_TYPE_MAX];				//アニメーションの外部ファイルの相対パス
															
	std::vector <CModelPart*>	 m_vParts;					//パーツのベクトル
	std::vector <animSpeed>		 m_vAnimSpeed;				//アニメーションスピードのベクトル
	int							 m_nPresentFrame;			//現在のフレーム
	int							 m_nPresentMotionSet;		//再生されるモーションセットの番号
	int							 m_nPresentAnim;			//再生されるアニメーションの番号
	AnimType					 m_type;					//アニメーションの種類
															
	static std::vector <ModelAnimationSet> m_vAnimAll;		//全部のアニメーションを持っているベクトル
};




#endif
