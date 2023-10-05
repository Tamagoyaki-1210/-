//=============================================================================
//
// model.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _MODEL_H
#define _MODEL_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object.h"
#include <vector>

class CModel : public CObject
{
public:

	//マテリアルの色
	struct ModelColor
	{
		int nMatNumber;
		D3DXCOLOR col;
	};

	//モデルの種類
	enum ModelType
	{
		MODEL_BODY = 0,
		MODEL_HEAD,
		MODEL_LEFT_ARM,
		MODEL_LEFT_HAND,
		MODEL_RIGHT_ARM,
		MODEL_RIGHT_HAND,
		MODEL_LEFT_LEG,
		MODEL_LEFT_FOOT,
		MODEL_RIGHT_LEG,
		MODEL_RIGHT_FOOT,

		MODEL_HEAD_SANTA,
		MODEL_HEAD_PUMPKIN,
		MODEL_HEAD_TOPHAT,
		MODEL_HEAD_KATANA,
		MODEL_HEAD_DRAGON,
		MODEL_HEAD_OCTOPUS,
		MODEL_HEAD_SNOWMAN,
		MODEL_HEAD_TOYBOX,
		MODEL_HEAD_SUMMER,
		MODEL_HEAD_TREE,

		MODEL_COIN_0,
		MODEL_COIN_1,
		MODEL_COIN_2,
		MODEL_COIN_3,

		MODEL_MOUNT_FUJI,
		MODEL_MOUNT_FUJI_YUKI,
		MODEL_BALLOON,
		MODEL_TREE1,
		MODEL_TREE2,
		MODEL_WINDMILL,
		MODEL_AIRSHIP,
		MODEL_STONE,

		MODEL_SPIKE_BALL,
		MODEL_LAVA_FLOOR,
		MODEL_BOUNCE_POLE,

		MODEL_TRAMPOLINE,
		MODEL_FALLING_STONE,
		MODEL_ICE_PILLAR,

		MODEL_CAUTION_SIGNAL,

		MODEL_GOAL,

		MODEL_MAX
	};

	CModel();																			//コンストラクタ
	CModel(const int nPriority);														//コンストラクタ
	virtual ~CModel() override;															//デストラクタ
													
	virtual HRESULT Init(void) override;												//初期化処理
	virtual void Uninit(void) override;													//終了処理
	virtual void Update(void) override;													//更新処理
	virtual void Draw(void) override;													//描画処理

	void SetPos(const D3DXVECTOR3 pos) override;										//位置の設定処理
	const D3DXVECTOR3 GetPos(void) override;											//位置の取得処理

	void SetRot(const D3DXVECTOR3 ros);													//向きの設定処理
	const D3DXVECTOR3 GetRot(void);														//向きの取得処理

	const D3DXVECTOR2 GetSize(void) override;											//サイズの取得処理
	void SetModel(const ModelType type);												//モデルの設定処理

	void StartRotation(const D3DXVECTOR3 frameRot);										//回転開始処理
	void StopRotating(void);															//回転停止処理
																						
	void SetModelColor(const int nNumMat, const D3DXCOLOR col);							//モデルの色の設定処理
	void SetShadowDraw(const bool bDraw);												//影の描画フラグの設定処理
	void SetShadowHeight(const float fHeight);											//影の高さの設定処理

	static void GetModel(ModelType type, LPD3DXMESH* pMesh, LPD3DXBUFFER* pBuffMat, DWORD* numMat);		//モデルのメッシュ情報の取得処理
	static void GetTextures(std::vector <LPDIRECT3DTEXTURE9>& vTex, CModel::ModelType type);			//マテリアルのテクスチャの取得処理
	static void LoadAllModels(void);													//全部のモデルのロード処理
	static void DestroyAllModels(void);													//全部のモデルの破棄処理

	static CModel* Create(ModelType type, D3DXVECTOR3 pos);								//生成処理
	static CModel* Create(ModelType type, D3DXVECTOR3 pos, const int nPriority);		//生成処理

private:

	LPD3DXMESH		m_pMesh;												//メッシュ情報へのポインタ
	LPD3DXBUFFER	m_pBuffMat;												//マテリアル情報へのポインタ
	DWORD			m_nNumMat;												//マテリアル情報の数
	D3DXVECTOR3		m_pos;													//現在の位置
	D3DXVECTOR3		m_LastPos;												//前回の位置
	D3DXVECTOR3		m_move;													//モデルの移動量
	D3DXVECTOR3		m_rot;													//向き
	D3DXVECTOR3		m_frameRot;												//1フレームの回転角度
	D3DXMATRIX		m_mtxWorld;												//ワールドマトリックス
	ModelType		m_type;													//モデルの種類
	std::vector <ModelColor> m_vCol;										//モデルの色
	bool			m_bShadow;												//影があるかどうか
	float			m_fShadowHeight;										//影の高さ

	static LPD3DXMESH		m_pMeshAll[MODEL_MAX];							//メッシュ情報へのポインタ
	static LPD3DXBUFFER		m_pBuffMatAll[MODEL_MAX];						//マテリアル情報へのポインタ
	static DWORD			m_nNumMatAll[MODEL_MAX];						//マテリアル情報の数
	static char*			m_pModelPass[MODEL_MAX];						//モデルの相対パス
	static std::vector <LPDIRECT3DTEXTURE9> m_vModelTexture[MODEL_MAX];		//モデルのテクスチャへのポインタのベクトル
};

#endif