//==============================================================================================
//
// オブジェクトX　　　objectX.cpp
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================
#include "objectX.h"
#include "application.h"
#include "model.h"
#include "light.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CObjectX::CObjectX(const PRIORITY priority) : CObject(priority)
{
	// デフォルト
	m_bShadow = false;
	m_bParts = false;
	m_bWireFrame = false;

	m_size = { 1.0f, 1.0f, 1.0f };
	m_col = { 0.0f, 0.0f, 0.0f, 0.0f };
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CObjectX::~CObjectX()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CObjectX::Init()
{
	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CObjectX::Uninit()
{
	Release();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CObjectX::Update()
{

}

//==============================================================================================
// 描画処理
//==============================================================================================
void CObjectX::Draw()
{
	if (GetDrawFlag())
	{
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
		D3DXMATRIX mtxRot, mtxTrans, mtxScaling;	//計算用のマトリックス
		D3DMATERIAL9 matDef;				//現在のマテリアルの保存用
		D3DXMATERIAL *pMat;					//マテリアルデータへのポインタ

		if (m_bWireFrame)
		{
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// サイズを反映
		D3DXMatrixScaling(&mtxScaling, m_size.x, m_size.y, m_size.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// 親のマトリックス
		D3DXMATRIX mtxParent;

		if (m_pParent != nullptr)
		{// パーツが親の場合
			mtxParent = m_pParent->GetMatrix();

			// 親のマトリックスとかける
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
		}
		else if (m_bParts == true)
		{// 現在のMatrix(PlayerのMatrix)を取得
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent); 

			// 親のマトリックスとかける
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
		}

		// 自身の番号のモデル情報を取得
		CModel::ModelPattern modelPattern = CApplication::GetModel()->GetModelPattern(m_nIndex);

		if (m_bShadow == true)
		{
			// 平面投影
			DrawShadow(pDevice, modelPattern);
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)modelPattern.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
		{
			// マテリアル情報の設定
			D3DMATERIAL9 matD3D = pMat[nCntMat].MatD3D;

			D3DXCOLOR matCol = m_Original_col[nCntMat] + m_col;

			// マテリアルの色変更
			matD3D.Diffuse = matD3D.Specular = matD3D.Emissive = matCol;

			// マテリアルの設定
			pDevice->SetMaterial(&matD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, modelPattern.pTexture[nCntMat]);

			// モデルパーツの描画
			modelPattern.pMesh->DrawSubset(nCntMat);
		}

		// 保持していたマテリアルを元に戻す
		pDevice->SetMaterial(&matDef);

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ステンシルバッファ=>無効
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	}
}

//==============================================================================================
// 平面投影
//==============================================================================================
void CObjectX::DrawShadow(const LPDIRECT3DDEVICE9 device, CModel::ModelPattern model)
{
	D3DMATERIAL9 matDef;		// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;			// マテリアルデータへのポインタ
	D3DXMATRIX mtxShadow;		// シャドウマトリックス
	D3DXPLANE planeField;		// 平面
	D3DXVECTOR3 pos, normal;

	// アンビエントを無効にする
	device->SetRenderState(D3DRS_AMBIENT, 0);

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// ライトの取得
	CLight *pLight = CApplication::GetLight();

	// ライトの方向
	D3DXVECTOR3 vecDir = pLight->GetLight().Direction;

	D3DXVECTOR4 vecLight = D3DXVECTOR4(-vecDir.x, -vecDir.y, -vecDir.z, 0.0f);

	// 法線の設定
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 平面の作成
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	// ライトと平面から影行列を作成
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// シャドウマトリックスの設定
	device->SetTransform(D3DTS_WORLD, &mtxShadow);

	// 現在のマテリアル保持
	device->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)model.pBuffMat->GetBufferPointer();

	// テクスチャの設定
	device->SetTexture(0, NULL);

	for (int nCnt = 0; nCnt < m_nNumMat; nCnt++)
	{
		// マテリアル情報の設定
		D3DMATERIAL9 matD3D = pMat[nCnt].MatD3D;

		// 色の設定
		matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// マテリアルの設定
		device->SetMaterial(&matD3D);

		// モデルパーツの描画
		model.pMesh->DrawSubset(nCnt);
	}

	// 保存していたマテリアルを戻す
	device->SetMaterial(&matDef);

	// アンビエントを戻す
	device->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
}

//==============================================================================================
// ステンシルバッファの設定
//==============================================================================================
void CObjectX::Stencil()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ステンシルバッファ=>有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// ステンシルバッファと比較する参照値設定=>ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// ステンシルバッファの値に対してのマスク設定=>0xff(すべて真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// ステンシルテストの比較方法設定=>
	// 参照値 >= ステンシルバッファの参照値なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);	// GREATEREQUAL:>=(以上)	EQUAL:==(等しい)   REF >= 今の値

																		// ステンシルテキストの結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// PASS:Zテスト＆ステンシルテスト成功		REPLACE:置き換え
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// FAIL:Zテスト＆ステンシルテスト失敗		KEEP:変更なし
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);		// ZFAIL:Zテストのみ失敗					INCR:+1
}

//==============================================================================================
// モデル番号の読み込み
//==============================================================================================
void CObjectX::SetModel(const int index)
{
	// 番号の設定
	m_nIndex = index;

	// 自身の番号のモデル情報を取得
	CModel::ModelPattern model = CApplication::GetModel()->GetModelPattern(m_nIndex);

	// モデルのマテリアル頂点数を設定
	m_nNumMat = model.nNumMat;

	// 使用中の色の初期化
	if (!m_Original_col.empty())
	{
		m_Original_col.clear();
	}

	// マテリアル色を初期化する
	for (int i = 0; i < m_nNumMat; i++)
	{
		m_Original_col.emplace_back();
		m_Original_col.back() = model.MatColor[i];
	}

	m_vtxMinModel = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_vtxMaxModel = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	int nNumMeshVtx;	// モデルのメッシュ頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファへのポインタ

						//頂点数の取得
	nNumMeshVtx = model.pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(model.pMesh->GetFVF());

	//頂点バッファのロック
	model.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumMeshVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//最小値の取得
		if (vtx.x < m_vtxMinModel.x)
		{
			m_vtxMinModel.x = vtx.x;
		}
		else if (vtx.y < m_vtxMinModel.y)
		{
			m_vtxMinModel.y = vtx.y;
		}
		else if (vtx.z < m_vtxMinModel.z)
		{
			m_vtxMinModel.z = vtx.z;
		}

		//最大値の取得
		if (vtx.x > m_vtxMaxModel.x)
		{
			m_vtxMaxModel.x = vtx.x;
		}
		else if (vtx.y > m_vtxMaxModel.y)
		{
			m_vtxMaxModel.y = vtx.y;
		}
		else if (vtx.z > m_vtxMaxModel.z)
		{
			m_vtxMaxModel.z = vtx.z;
		}

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点バッファのアンロック
	model.pMesh->UnlockVertexBuffer();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, const PRIORITY priority)
{
	CObjectX* pObjectX = new CObjectX(priority);

	if (FAILED(pObjectX->Init()))
	{
		return nullptr;
	}
	pObjectX->SetPos(pos);
	pObjectX->SetRot(rot);
	pObjectX->SetParent(parent);
	pObjectX->SetModel(CApplication::GetModel()->ReadObject(Xfilename));

	return pObjectX;
}

//==============================================================================================
// 生成処理
//==============================================================================================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const int index, const PRIORITY priority)
{
	CObjectX* pObjectX = new CObjectX(priority);

	if (FAILED(pObjectX->Init()))
	{
		return nullptr;
	}
	pObjectX->SetPos(pos);
	pObjectX->SetRot(rot);
	pObjectX->SetParent(parent);
	pObjectX->SetModel(index);

	return pObjectX;
}