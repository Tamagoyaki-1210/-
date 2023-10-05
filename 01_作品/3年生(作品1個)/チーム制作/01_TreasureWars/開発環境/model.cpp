//==============================================================================================
//
// モデル　　　madel.cpp
// tutida ryousei
//
//==============================================================================================
#include"model.h"
#include"application.h"
#include"light.h"
#include"game.h"
#include"player.h"

std::vector <CModel::Model> CModel::m_Model = {};
//==============================================================================================
// コンストラクタ
//==============================================================================================
CModel::CModel()
{
	m_Size = {1.0f, 1.0f, 1.0f};
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CModel::~CModel()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CModel::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	m_bShadow = true;

	// マテリアル色を初期化する
	for (int i = 0; i < (int)m_Model[m_nIndex].nNumMat; i++)
	{
		m_col.emplace_back();
		m_col.back() = m_Model[m_nIndex].MatColor[i];
	}

	m_vtxMinModel = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_vtxMaxModel = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	//頂点数の取得
	nNumVtx = m_Model[m_nIndex].pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_Model[m_nIndex].pMesh->GetFVF());

	//頂点バッファのロック
	m_Model[m_nIndex].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
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
	m_Model[m_nIndex].pMesh->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CModel::Uninit()
{

}

//==============================================================================================
// 更新処理
//==============================================================================================
void CModel::Update()
{

}

//==============================================================================================
// 描画処理
//==============================================================================================
D3DXMATRIX CModel::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScaling;	//計算用のマトリックス
	D3DMATERIAL9 matDef;				//現在のマテリアルの保存用
	D3DXMATERIAL *pMat;					//マテリアルデータへのポインタ

	//ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズを反映
	D3DXMatrixScaling(&mtxScaling, m_Size.x, m_Size.y, m_Size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 親のマトリックス
	D3DXMATRIX mtxParent;

	if (m_pParent != nullptr)
	{// パーツが親の場合
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{// 現在のMatrix(PlayerのMatrix)を取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 親のマトリックスとかける
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	if (m_bShadow == true)
	{
		// 平面投影
		DrawShadow(pDevice);
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_Model[m_nIndex].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++)
	{
		// マテリアル情報の設定
		D3DMATERIAL9 matD3D = pMat[nCntMat].MatD3D;

		// マテリアルの色変更
		matD3D.Diffuse = matD3D.Specular = matD3D.Emissive = m_col[nCntMat];

		// マテリアルの設定
		pDevice->SetMaterial(&matD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_Model[m_nIndex].pTexture[nCntMat]);

		// モデルパーツの描画
		m_Model[m_nIndex].pMesh->DrawSubset(nCntMat);
	}

	// 保持していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ステンシルバッファ=>無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	return m_mtxWorld;
}

//==============================================================================================
// 平面投影
//==============================================================================================
void CModel::DrawShadow(const LPDIRECT3DDEVICE9 device)
{
	if (CApplication::GetMode() != CApplication::MODE_RESULT)
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

		// 位置の設定
		if (CApplication::GetMode() == CApplication::MODE_ENTRY)
			pos = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
		else if (CApplication::GetMode() != CApplication::MODE_ENTRY)
			pos = D3DXVECTOR3(0.0f, 8.0f, 0.0f);

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
		pMat = (D3DXMATERIAL*)m_Model[m_nIndex].pBuffMat->GetBufferPointer();

		// テクスチャの設定
		device->SetTexture(0, NULL);

		for (int nCnt = 0; nCnt < (int)m_Model[m_nIndex].nNumMat; nCnt++)
		{
			// マテリアル情報の設定
			D3DMATERIAL9 matD3D = pMat[nCnt].MatD3D;

			// 色の設定
			matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			// マテリアルの設定
			device->SetMaterial(&matD3D);

			// モデルパーツの描画
			m_Model[m_nIndex].pMesh->DrawSubset(nCnt);
		}

		// 保存していたマテリアルを戻す
		device->SetMaterial(&matDef);

		// アンビエントを戻す
		device->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	}
}

//==============================================================================================
// ステンシルバッファの設定
//==============================================================================================
void CModel::Stencil()
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
// 全てのモデルの終了処理
//==============================================================================================
void CModel::ModelReleaseAll()
{
	// モデルを全て終了させる
	while (!m_Model.empty())
	{
		// メッシュの破棄
		if (m_Model.back().pMesh != nullptr)
		{
			m_Model.back().pMesh->Release();
		}

		// マテリアルの破棄
		if (m_Model.back().pBuffMat != nullptr)
		{
			m_Model.back().pBuffMat->Release();
		}

		for (int nCntMat = 0; nCntMat < (int)m_Model.back().nNumMat; nCntMat++)
		{
			// テクスチャの破棄
			if (m_Model.back().pTexture[nCntMat] != nullptr)
			{
				m_Model.back().pTexture[nCntMat]->Release();
			}
		}	
		// 一番後ろの要素をリストから排除する
		m_Model.pop_back();
	}
}

//==============================================================================================
// ファイルの読み込み
//==============================================================================================
int CModel::ReadObject(const char* name)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	int nIndex = 0;

	// 作成された全てのモデルを読み込む
	for (auto model : m_Model)
	{
		if (model.fileName == name)
		{
			// 使用中のモデルと一致する場合終了
			return nIndex;
		}
		nIndex++;
	}

	// モデル情報の後ろの要素を追加
	m_Model.emplace_back();

	m_Model.back().fileName = name;

	//Xファイルの読み込み
	D3DXLoadMeshFromX(m_Model.back().fileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_Model.back().pBuffMat,
		NULL,
		&m_Model.back().nNumMat,
		&m_Model.back().pMesh);

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_Model.back().pBuffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)m_Model.back().nNumMat; i++)
	{
		// テクスチャの後ろの要素を追加
		m_Model.back().pTexture.emplace_back();

		if (pMat[i].pTextureFilename != nullptr)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&m_Model.back().pTexture.back());
		}
		else
		{
			m_Model.back().pTexture.back() = nullptr;
		}

		// マテリアルの後ろの要素を追加
		m_Model.back().MatColor.emplace_back();

		// マテリアルの色情報を保存
		m_Model.back().MatColor.back() = pMat[i].MatD3D.Diffuse;
	}

	return nIndex;
}

//==============================================================================================
// 生成処理
//==============================================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel *parent, char *Xfilename)
{
	CModel *pModel = nullptr;

	pModel = new CModel;

	if (pModel != nullptr)
	{
		pModel->SetPos(pos);
		pModel->SetRot(rot);
		pModel->SetParent(parent);
		pModel->m_nIndex = pModel->ReadObject(Xfilename);
		pModel->Init();
	}

	return pModel;
}