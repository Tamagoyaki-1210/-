//=============================================================================
//
// environment.h
// Author : Kajita Hiromu
//
//=============================================================================

#ifndef _ENVIROMENT_H_
#define _ENVIROMENT_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "model.h"

//=============================================================================
//前方宣言
//=============================================================================

class CEnvironment : public CModel
{
public:
	//自然・環境の種類
	enum EnvironmentType
	{
		ENVIRONMENT_MOUNT = 0,
		ENVIRONMENT_MOUNT_YUKI,
		ENVIRONMENT_BALLOON,
		ENVIRONMENT_TREE1,
		ENVIRONMENT_TREE2,
		ENVIRONMENT_WINDMILL,
		ENVIRONMENT_AIRSHIP,
		ENVIRONMENT_STONE,
		ENVIRONMENT_TYPE_MAX
	};

	CEnvironment();										//コンストラクタ
	~CEnvironment() override;								//デストラクタ

	HRESULT Init(void) override;					//初期化処理
	void Uninit(void) override;						//終了処理
	void Update(void) override;						//更新処理
	void Draw(void) override;						//描画処理

	static CEnvironment* Create(D3DXVECTOR3 pos, EnvironmentType type);	//生成処理

private:
};

#endif //!_ENVIROMENT_H_