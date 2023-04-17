//=============================================================================
//
// model.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "model.h"
#include "application.h"
#include "rendering.h"
#include "directionalLight.h"


//=============================================================================
//							静的変数の初期化
//=============================================================================

LPD3DXMESH		CModel::m_pMeshAll[MODEL_MAX] = {};					//メッシュ情報へのポインタ
LPD3DXBUFFER	CModel::m_pBuffMatAll[MODEL_MAX] = {};				//マテリアル情報へのポインタ
DWORD			CModel::m_nNumMatAll[MODEL_MAX] = {};				//マテリアル情報の数
std::vector <LPDIRECT3DTEXTURE9>     CModel::m_vModelTexture[CModel::MODEL_MAX];		//モデルのテクスチャへのポインタのベクトル

//モデルのXファイルの相対パス
char*			CModel::m_pModelPass[MODEL_MAX] =
{
	{ "data\\MODELS\\Player02\\Player02_Body.x" },						//MODEL_BODY,				
	{ "data\\MODELS\\Player02\\Player02_Head.x" },						//MODEL_HEAD,
	{ "data\\MODELS\\Player02\\Player02_Arm_Left.x" },					//MODEL_LEFT_ARM,
	{ "data\\MODELS\\Player02\\Player02_Hand_Left.x" },					//MODEL_LEFT_HAND,
	{ "data\\MODELS\\Player02\\Player02_Arm_Right.x" },					//MODEL_RIGHT_ARM,
	{ "data\\MODELS\\Player02\\Player02_Hand_Right.x" },				//MODEL_RIGHT_HAND,
	{ "data\\MODELS\\Player02\\Player02_Leg_Left.x" },					//MODEL_LEFT_LEG,
	{ "data\\MODELS\\Player02\\Player02_Foot_Left.x" },					//MODEL_LEFT_FOOT,
	{ "data\\MODELS\\Player02\\Player02_Leg_Right.x" },					//MODEL_RIGHT_LEG,
	{ "data\\MODELS\\Player02\\Player02_Foot_Right.x" },				//MODEL_RIGHT_FOOT,
																		
	{ "data\\MODELS\\Player02\\PlayerParts\\SantaHead.x" },				//MODEL_HEAD_SANTA,
	{ "data\\MODELS\\Player02\\PlayerParts\\PumpHead.x" },				//MODEL_HEAD_PUMPKIN,
	{ "data\\MODELS\\Player02\\PlayerParts\\TopHatHead.x" },			//MODEL_HEAD_TOPHAT,
	{ "data\\MODELS\\Player02\\PlayerParts\\KatanaHead.x" },			//MODEL_HEAD_KATANA,
	{ "data\\MODELS\\Player02\\PlayerParts\\DragonHead.x" },			//MODEL_HEAD_DRAGON,
	{ "data\\MODELS\\Player02\\PlayerParts\\oqtoHead.x" },				//MODEL_HEAD_OCTOPUS,
	{ "data\\MODELS\\Player02\\PlayerParts\\snowman.x" },				//MODEL_HEAD_SNOWMAN,
	{ "data\\MODELS\\Player02\\PlayerParts\\ToyBoxHead.x" },			//MODEL_HEAD_TOYBOX,
	{ "data\\MODELS\\Player02\\PlayerParts\\SummerTimeHead.x" },		//MODEL_HEAD_SUMMER,
	{ "data\\MODELS\\Player02\\PlayerParts\\TreeHead.x" },				//MODEL_HEAD_TREE,
																		
	{ "data\\MODELS\\Coin\\Coin00.x" },									//MODEL_COIN_0,
	{ "data\\MODELS\\Coin\\Coin01.x" },									//MODEL_COIN_1,
	{ "data\\MODELS\\Coin\\Coin02.x" },									//MODEL_COIN_2,
	{ "data\\MODELS\\Coin\\Coin03.x" },									//MODEL_COIN_3,
																		
	{ "data\\MODELS\\Environment\\Mountains\\Fuji.x" },					//MODEL_MOUNT_FUJI,
	{ "data\\MODELS\\Environment\\Mountains\\FujiYuki.x" },				//MODEL_MOUNT_FUJI_YUKI,
	{ "data\\MODELS\\Environment\\Balloon\\Balloon.x" },				//MODEL_BALLOON,
	{ "data\\MODELS\\Environment\\tree\\tree1.x" },						//MODEL_TREE1,
	{ "data\\MODELS\\Environment\\tree\\tree2.x" },						//MODEL_TREE2,
	{ "data\\MODELS\\Environment\\windmill\\windmill.x" },				//MODEL_WINDMILL,
	{ "data\\MODELS\\Environment\\airship\\airship.x" },				//MODEL_AIRSHIP,
	{ "data\\MODELS\\Environment\\stone\\stone.x" },					//MODEL_STONE,
																		
	{ "data\\MODELS\\Stage_gimmick\\NeedleBall\\NeedleBall.x" },		//MODEL_SPIKE_BALL,
	{ "data\\MODELS\\Stage_gimmick\\LavaFloor\\Lava.x" },				//MODEL_LAVA_FLOOR,
	{ "data\\MODELS\\Stage_gimmick\\BoundPole\\BoundPole.x" },			//MODEL_BOUNCE_POLE,
																		
	{ "data\\MODELS\\traps\\jump\\jump.x" },							//MODEL_TRAMPOLINE,
	{ "data\\MODELS\\traps\\fallstone\\fallstone.x" },					//MODEL_FALLING_STONE,
	{ "data\\MODELS\\traps\\IcePillar\\IcePillar_TrapType.x" },			//MODEL_ICE_PILLAR,
																		
	{ "data\\MODELS\\sign\\fallstonesign.x" },							//MODEL_CAUTION_SIGNAL,
																		
	{ "data\\MODELS\\Goal\\Goal01.x" },									//MODEL_GOAL,
};

//コンストラクタ
CModel::CModel()
{
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_frameRot = Vec3Null;							//1フレームの回転角度
	D3DXMatrixIdentity(&m_mtxWorld);				//ワールドマトリックス
	m_type = CModel::MODEL_BODY;					//モデルの種類
	m_vCol.clear();									//マテリアルの色
	m_bShadow = true;								//影を描画するかどうか
	m_fShadowHeight = 0.0f;							//影の高さ
}

CModel::CModel(const int nPriority) : CObject::CObject(nPriority)
{
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_frameRot = Vec3Null;							//1フレームの回転角度
	D3DXMatrixIdentity(&m_mtxWorld);				//ワールドマトリックス
	m_type = CModel::MODEL_BODY;					//モデルの種類
	m_vCol.clear();									//マテリアルの色
	m_bShadow = true;								//影を描画するかどうか
	m_fShadowHeight = 0.0f;							//影の高さ
}

//デストラクタ
CModel::~CModel()
{

}

//初期化処理
HRESULT CModel::Init(void)
{
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_frameRot = Vec3Null;							//1フレームの回転角度
	D3DXMatrixIdentity(&m_mtxWorld);				//ワールドマトリックス
	m_vCol.clear();									//マテリアルの色
	m_bShadow = true;								//影を描画するかどうか
	m_fShadowHeight = -149.9f;						//影の高さ

	return S_OK;
}

//終了処理
void CModel::Uninit(void)
{
	m_vCol.clear();				//マテリアルの色をクリアする
}

//更新処理
void CModel::Update(void)
{
	m_LastPos = m_pos;			//前回の位置の更新

	if (m_frameRot != nullptr)
	{//回転速度が0ではなかったら
		m_rot += m_frameRot;			//回転角度を更新する
	}
}

//描画処理
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();				//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxShadow;			//計算用マトリックス
	D3DMATERIAL9 matDef;							//現在のマテリアル保存用
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ
	D3DXVECTOR4 vecLight;							//ライトの向き
	D3DXVECTOR3 pos, Normal;						//投影用の位置と法線
	D3DXPLANE planeField;							//面

		//ライトの向きを設定する
		D3DXVECTOR3 dir = CDirectionalLight::GetPrincipalLightDir();
		D3DXVec3Normalize(&dir, &dir);
		vecLight = D3DXVECTOR4(-dir.x, -dir.y, -dir.z, 0.0f);

		pos = D3DXVECTOR3(0.0f, m_fShadowHeight, 0.0f);				//面の高さ
		Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						//面の法線

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);
		D3DXMatrixIdentity(&mtxShadow);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		if (m_bShadow)
		{//影の描画のフラグがtrueだったら

		//影の描画用のマトリックスを作る
		D3DXPlaneFromPointNormal(&planeField, &pos, &Normal);
		D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

		D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタの取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			D3DXCOLOR col = pMat[nCntMat].MatD3D.Diffuse;			//マテリアルの色を保存する

			//マテリアルの色を真っ黒にする
			pMat[nCntMat].MatD3D.Diffuse.r = 0.0f;
			pMat[nCntMat].MatD3D.Diffuse.g = 0.0f;
			pMat[nCntMat].MatD3D.Diffuse.b = 0.0f;
			pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, m_vModelTexture[m_type].data()[nCntMat]);

			//モデルパーツの描画
			m_pMesh->DrawSubset(nCntMat);

			//マテリアルの色を元に戻す
			pMat[nCntMat].MatD3D.Diffuse.r = col.r;
			pMat[nCntMat].MatD3D.Diffuse.g = col.g;
			pMat[nCntMat].MatD3D.Diffuse.b = col.b;
			pMat[nCntMat].MatD3D.Diffuse.a = col.a;
		}


		//ステンシルバッファを有効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		//ステンシルバッファと比較する参照値設定
		pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

		//ステンシルバッファの値に対してのマスク設定
		pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

		//ステンシルテストの比較方法の設定
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

		//ステンシルテストの結果に対しての反映設定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

		pMat = nullptr;

		//保持しいたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}


	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタの取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_vModelTexture[m_type].data()[nCntMat]);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保持しいたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	//ステンシルバッファを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	pMat = nullptr;
	
	//保持しいたマテリアルを戻す
	pDevice->SetMaterial(&matDef);


	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタの取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//マテリアルの色が設定されていたら、そのマテリアルの色を変えたら、描画して、元に戻す
		D3DXCOLOR c = {};
		bool bCol = false;

		for (int i = 0; i < (int)m_vCol.size(); i++)
		{
			if (m_vCol.data()[i].nMatNumber == nCntMat)
			{
				bCol = true;
				c = pMat[nCntMat].MatD3D.Diffuse;
				pMat[nCntMat].MatD3D.Diffuse = m_vCol.data()[i].col;
				break;
			}
		}

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_vModelTexture[m_type].data()[nCntMat]);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);

		if (bCol)
		{
			pMat[nCntMat].MatD3D.Diffuse = c;
		}
	}

	//保持しいたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//位置の設定処理
void CModel::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//位置の取得処理
const D3DXVECTOR3 CModel::GetPos(void)
{
	return m_pos;
}

//モデルの向き設定処理
void CModel::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//モデルの向きの取得処理
const D3DXVECTOR3 CModel::GetRot(void)
{
	return m_rot;
}

//ポリゴンのサイズの取得処理
const D3DXVECTOR2 CModel::GetSize(void)
{
	return D3DXVECTOR2(0.0f, 0.0f);
}

//モデルの設定処理
void CModel::SetModel(const ModelType type)
{
	m_pMesh = m_pMeshAll[type];
	m_pBuffMat = m_pBuffMatAll[type];
	m_nNumMat = m_nNumMatAll[type];
	m_type = type;
}

//回転速度の設定処理
void CModel::StartRotation(const D3DXVECTOR3 frameRot)
{
	m_frameRot = frameRot;
}

//回転を止まる処理
void CModel::StopRotating(void)
{
	m_frameRot = Vec3Null;
}

//カーラーの設定処理
void CModel::SetModelColor(const int nNumMat, const D3DXCOLOR col)
{
	ModelColor mCol = {};
	mCol.nMatNumber = nNumMat;
	mCol.col = col;

	m_vCol.push_back(mCol);
}

//影が描画されているかどうかの設定処理
void CModel::SetShadowDraw(const bool bDraw)
{
	m_bShadow = bDraw;
}

//影の高さの設定処理
void CModel::SetShadowHeight(const float fHeight)
{
	m_fShadowHeight = fHeight;
}



//=============================================================================
//								静的関数
//=============================================================================




//モデル情報の取得処理
void CModel::GetModel(ModelType type, LPD3DXMESH* pMesh, LPD3DXBUFFER* pBuffMat, DWORD* numMat)
{
	*pMesh = m_pMeshAll[type];				//メッシュへのポインタの設定
	*pBuffMat = m_pBuffMatAll[type];		//マテリアルへのポインタの設定
	*numMat = m_nNumMatAll[type];			//マテリアル数の設定
}

//モデル全部の読み込み処理
void CModel::LoadAllModels(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();		//デバイスの取得

	for (int nCnt = 0; nCnt < CModel::MODEL_MAX; nCnt++)
	{
		m_vModelTexture[nCnt].clear();				//テクスチャをクリアする

		//Xファイルの読み込み
		D3DXLoadMeshFromX(m_pModelPass[nCnt],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMatAll[nCnt],
			NULL,
			&m_nNumMatAll[nCnt],
			&m_pMeshAll[nCnt]);

		D3DXMATERIAL *pMat = nullptr;				//マテリア情報へのポインタ

		//マテリアルデータへのポインタの取得
		pMat = (D3DXMATERIAL*)m_pBuffMatAll[nCnt]->GetBufferPointer();

		for (int a = 0; a < (int)m_nNumMatAll[nCnt]; a++)
		{//マテリアのテクスチャが設定されていたら、生成して、保存する
			LPDIRECT3DTEXTURE9 pTex = nullptr;

			D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &pTex);
			pMat++;
			m_vModelTexture[nCnt].push_back(pTex);
		}
	}	
}

//モデル全部の破棄処理
void CModel::DestroyAllModels(void)
{
	for (int nCnt = 0; nCnt < CModel::MODEL_MAX; nCnt++)
	{
		//メッシュの破棄
		if (m_pMeshAll[nCnt] != nullptr)
		{
			m_pMeshAll[nCnt]->Release();
			m_pMeshAll[nCnt] = nullptr;
		}

		//マテリアルの破棄
		if (m_pBuffMatAll[nCnt] != nullptr)
		{
			m_pBuffMatAll[nCnt]->Release();
			m_pBuffMatAll[nCnt] = nullptr;
		}
	}
}

//テクスチャの取得処理
void CModel::GetTextures(std::vector <LPDIRECT3DTEXTURE9>& vTex, CModel::ModelType type)
{
	vTex.clear();

	for (int nCnt = 0; nCnt < (int)m_nNumMatAll[type]; nCnt++)
	{
		vTex.push_back(m_vModelTexture[type].data()[nCnt]);
	}
}

//生成処理
CModel* CModel::Create(ModelType type, D3DXVECTOR3 pos)
{
	CModel* pModel = new CModel(3);						//モデルを生成する

	if (FAILED(pModel->Init()))
	{//初期化処理
		return nullptr;
	}

	pModel->m_pos = pos;								//位置の設定
	pModel->m_LastPos = pos;							//前回の位置の設定
	pModel->m_move = Vec3Null;							//移動量の設定
	pModel->m_rot = Vec3Null;							//向きの設定
	pModel->m_pMesh = m_pMeshAll[type];					//メッシュへのポインタの取得
	pModel->m_pBuffMat = m_pBuffMatAll[type];			//マテリアルへのポインタの取得
	pModel->m_nNumMat = m_nNumMatAll[type];				//マテリアル数の取得
	pModel->m_type = type;								//種類の設定

	return pModel;										//生成したインスタンスを返す
}

CModel* CModel::Create(ModelType type, D3DXVECTOR3 pos, const int nPriority)
{
	CModel* pModel = new CModel(nPriority);				//モデルを生成する

	if (FAILED(pModel->Init()))
	{//初期化処理
		return nullptr;
	}

	pModel->m_pos = pos;								//位置の設定
	pModel->m_LastPos = pos;							//前回の位置の設定
	pModel->m_move = Vec3Null;							//移動量の設定
	pModel->m_rot = Vec3Null;							//向きの設定
	pModel->m_pMesh = m_pMeshAll[type];					//メッシュへのポインタの取得
	pModel->m_pBuffMat = m_pBuffMatAll[type];			//マテリアルへのポインタの取得
	pModel->m_nNumMat = m_nNumMatAll[type];				//マテリアル数の取得
	pModel->m_type = type;								//種類の設定

	return pModel;										//生成したインスタンスを返す
}