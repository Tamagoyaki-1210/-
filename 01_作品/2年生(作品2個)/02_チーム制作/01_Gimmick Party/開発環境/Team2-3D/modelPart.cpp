//=============================================================================
//
// modelPart.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "modelPart.h"
#include "application.h"
#include "rendering.h"
#include "directionalLight.h"

//コンストラクタ
CModelPart::CModelPart()
{
	//メンバー変数をクリアする
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_shadowPosY = 0.0f;							//影の高さ
	D3DXMatrixIdentity(&m_mtxWorld);				//ワールドマトリックス
	m_type = CModel::MODEL_BODY;					//モデルの種類
	m_pParent = nullptr;							//親へのポインタ
	m_vModelTexture.clear();						//テクスチャへのポインタ
	m_vCol.clear();									//マテリアルの色
}													

//デストラクタ
CModelPart::~CModelPart()
{

}

//初期化処理
HRESULT CModelPart::Init(void)
{
	//初期化処理
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_shadowPosY = 0.0f;							//影の高さ
	D3DXMatrixIdentity(&m_mtxWorld);				//ワールドマトリックス
	m_type = CModel::MODEL_BODY;					//モデルの種類
	m_pParent = nullptr;							//親へのポインタ
	m_vModelTexture.clear();						//テクスチャへのポインタ
	m_vCol.clear();									//マテリアルの色

	return S_OK;
}

//終了処理
void CModelPart::Uninit(void)
{
	m_vCol.clear();					//マテリアルの色をクリアする
}

//更新処理
void CModelPart::Update(void)
{

}

//描画処理
void CModelPart::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();				//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxShadow, mtxParent;		//計算用マトリックス
	D3DMATERIAL9 matDef;									//現在のマテリアル保存用
	D3DXMATERIAL *pMat;										//マテリアルデータへのポインタ
	D3DXVECTOR4 vecLight;									//ライトの向き
	D3DXVECTOR3 pos, Normal;								//投影用の位置と法線
	D3DXPLANE planeField;									//面

	if (m_pParent != nullptr)
	{//親がある場合、親のマトリックスを取得する
		mtxParent = m_pParent->GetMatrix();
	}

	//ステンシルバッファを有効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	//ステンシルバッファの値に対してのマスク設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	//ステンシルテストの比較方法の設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//ステンシルテストの結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	//ライトの向きを設定する
	D3DXVECTOR3 dir = CDirectionalLight::GetPrincipalLightDir();
	D3DXVec3Normalize(&dir, &dir);
	vecLight = D3DXVECTOR4(-dir.x, -dir.y, -dir.z, 0.0f);

	pos = D3DXVECTOR3(0.0f, m_shadowPosY, 0.0f);				//面の高さ
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

	//ワールドマトリックスの設定
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

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

		D3DXCOLOR col = pMat[nCntMat].MatD3D.Diffuse;					//マテリアルの色を保存する

		//マテリアルの色を真っ黒にする
		pMat[nCntMat].MatD3D.Diffuse.r = 0.0f;
		pMat[nCntMat].MatD3D.Diffuse.g = 0.0f;
		pMat[nCntMat].MatD3D.Diffuse.b = 0.0f;
		pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_vModelTexture.data()[nCntMat]);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);

		//マテリアルの色を元に戻す
		pMat[nCntMat].MatD3D.Diffuse.r = col.r;
		pMat[nCntMat].MatD3D.Diffuse.g = col.g;
		pMat[nCntMat].MatD3D.Diffuse.b = col.b;
		pMat[nCntMat].MatD3D.Diffuse.a = col.a;
	}

	pMat = nullptr;

	//ステンシルバッファと比較する参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	//ステンシルバッファの値に対してのマスク設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	//ステンシルテストの比較方法の設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

	//ステンシルテストの結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

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
		pDevice->SetTexture(0, m_vModelTexture.data()[nCntMat]);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);

		if (bCol)
		{
			pMat[nCntMat].MatD3D.Diffuse = c;
		}
	}

	//保持しいたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	//ステンシルバッファを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//描画処理
void CModelPart::Draw(D3DXMATRIX mtxParent)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();				//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxShadow;					//計算用マトリックス
	D3DMATERIAL9 matDef;									//現在のマテリアル保存用
	D3DXMATERIAL *pMat;										//マテリアルデータへのポインタ
	D3DXVECTOR4 vecLight;									//ライトの向き
	D3DXVECTOR3 pos, Normal;								//投影用の位置と法線
	D3DXPLANE planeField;									//面

															//ライトの向きを設定する
	D3DXVECTOR3 dir = CDirectionalLight::GetPrincipalLightDir();
	D3DXVec3Normalize(&dir, &dir);
	vecLight = D3DXVECTOR4(-dir.x, -dir.y, -dir.z, 0.0f);

	pos = D3DXVECTOR3(0.0f, m_shadowPosY, 0.0f);							//面の高さ
	Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);									//面の法線

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixIdentity(&mtxShadow);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//影の描画用のマトリックスを作る
	D3DXPlaneFromPointNormal(&planeField, &pos, &Normal);
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//ステンシルバッファを有効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	//ステンシルバッファの値に対してのマスク設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	//ステンシルテストの比較方法の設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	//ステンシルテストの結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタの取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		D3DXCOLOR col = pMat[nCntMat].MatD3D.Diffuse;					//マテリアルの色を保存する

		//マテリアルの色を真っ黒にする
		pMat[nCntMat].MatD3D.Diffuse.r = 0.0f;
		pMat[nCntMat].MatD3D.Diffuse.g = 0.0f;
		pMat[nCntMat].MatD3D.Diffuse.b = 0.0f;
		pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_vModelTexture.data()[nCntMat]);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);

		//マテリアルの色を元に戻す
		pMat[nCntMat].MatD3D.Diffuse.r = col.r;
		pMat[nCntMat].MatD3D.Diffuse.g = col.g;
		pMat[nCntMat].MatD3D.Diffuse.b = col.b;
		pMat[nCntMat].MatD3D.Diffuse.a = col.a;
	}

	pMat = nullptr;

	//ステンシルバッファと比較する参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	//ステンシルバッファの値に対してのマスク設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	//ステンシルテストの比較方法の設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

	//ステンシルテストの結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

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
		pDevice->SetTexture(0, m_vModelTexture.data()[nCntMat]);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);

		if (bCol)
		{
			pMat[nCntMat].MatD3D.Diffuse = c;
		}
	}

	//保持しいたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	pDevice->SetTexture(0, nullptr);

	//ステンシルバッファを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//親の設定処理
void CModelPart::SetParent(CModelPart* pParent)
{
	m_pParent = pParent;
}

//親の取得処理
CModelPart* CModelPart::GetParent(void)
{
	return m_pParent;
}

//位置の設定処理
void CModelPart::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//位置の取得処理
const D3DXVECTOR3 CModelPart::GetPos(void)
{
	return m_pos;
}

//向きの設定処理
void CModelPart::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//向きの取得処理
const D3DXVECTOR3 CModelPart::GetRot(void)
{
	return m_rot;
}

//サイズの取得処理
const D3DXVECTOR3 CModelPart::GetSize(void)
{
	return Vec3Null;
}

//マトリックスの取得処理
D3DXMATRIX CModelPart::GetMatrix(void)
{
	return m_mtxWorld;
}

//影の高さの設定処理
void CModelPart::SetShadowHeight(const float fHeight)
{
	m_shadowPosY = fHeight;
}

//カーラーの設定処理
void CModelPart::SetModelColor(const int nNumMat, const D3DXCOLOR col)
{
	//新しいマテリアルが既に入れたかどうか確認する、そうだったら、上書きする
	for (int nCnt = 0; nCnt < (int)m_vCol.size(); nCnt++)
	{
		if (m_vCol.data()[nCnt].nMatNumber == nNumMat)
		{
			CModel::ModelColor mCol = {};
			mCol.nMatNumber = nNumMat;
			mCol.col = col;
			m_vCol.data()[nCnt] = mCol;
			return;
		}
	}

	//新しいマテリアルの色を保存する
	CModel::ModelColor mCol = {};
	mCol.nMatNumber = nNumMat;
	mCol.col = col;

	m_vCol.push_back(mCol);
}

//モデルの設定処理
void CModelPart::SetModel(CModel::ModelType model)
{
	//必要なメンバー変数をクリアする
	m_pMesh = nullptr;
	m_pBuffMat = nullptr;
	m_nNumMat = 0;
	m_vModelTexture.clear();

	CModel::GetModel(model, &m_pMesh, &m_pBuffMat, &m_nNumMat);			//モデルの取得処理
	CModel::GetTextures(m_vModelTexture, model);						//テクスチャの取得処理
}



//=============================================================================
//								静的関数
//=============================================================================




//生成処理
CModelPart* CModelPart::Create(CModel::ModelType type, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CModelPart* pModel = new CModelPart;					//インスタンスを生成する

	if (FAILED(pModel->Init()))
	{//初期化処理
		return nullptr;
	}

	pModel->m_pos = pos;					//位置の設定
	pModel->m_rot = rot;					//向きの設定
	pModel->m_LastPos = pos;				//前回の位置の設定
	pModel->m_type = type;					//種類の設定
	pModel->m_shadowPosY = pos.y;			//影の高さの設定

	CModel::GetModel(type, &pModel->m_pMesh, &pModel->m_pBuffMat, &pModel->m_nNumMat);			//モデルの設定処理
	CModel::GetTextures(pModel->m_vModelTexture, type);											//テクスチャの設定処理

	return pModel;							//生成したインスタンスを返す
}