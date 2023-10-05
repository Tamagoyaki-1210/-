//=============================================================================
//
// meshfield.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "meshfield.h"
#include "application.h"
#include "rendering.h"
#include "object2D.h"

std::vector <CMeshfield*> CMeshfield::m_vMeshfield;

//コンストラクタ
CMeshfield::CMeshfield()
{
	//メンバー変数をクリアする
	m_pVtxBuff = nullptr;								//頂点バッファ
	m_pIdxBuff = nullptr;								//インデックスバッファ
	m_pTexture = nullptr;								//テクスチャへのポインタ
	m_pos = Vec3Null;									//位置
	m_rot = Vec3Null;									//向き
	m_size = Vec2Null;									//サイズ
	m_LateralCoords = Vec2Null;							//幅
	D3DXMatrixIdentity(&m_mtxWorld);					//ワールドマトリックス
	m_nVertexNumber = 0;								//頂点数
	m_nIntexNumber = 0;									//インデックス数
	m_nPolygonNumber = 0;								//ポリゴン数
	m_nLineVertex = 0;									//一行の頂点数
	m_nColumnVertex = 0;								//一列の頂点数
	m_fFriction = 0.0f;									//摩擦係数
	m_fAnimAngle = 0.0f;								//アニメーション用の角度
	m_fAnimSpeed = 0.0f;								//アニメーションスピード
	m_fAmplitude = 0.0f;								//アニメーションの振幅
	m_nPriority = 0;									//プライオリティ
	m_bTextureAnim = false;								//テクスチャアニメーションのフラグ
	m_bAnim = false;									//アニメーションのフラグ
	m_animSpeed = Vec2Null;								//アニメーションスピード
														
	m_vMeshfield.push_back(this);						//メッシュフィールドのアドレスを持っているベクトルにこのポインタを保存する
}

CMeshfield::CMeshfield(const int nPriority) : CObject::CObject(nPriority)
{
	//メンバー変数をクリアする
	m_pVtxBuff = nullptr;								//頂点バッファ
	m_pIdxBuff = nullptr;								//インデックスバッファ
	m_pTexture = nullptr;								//テクスチャへのポインタ
	m_pos = Vec3Null;									//位置
	m_rot = Vec3Null;									//向き
	m_size = Vec2Null;									//サイズ
	m_LateralCoords = Vec2Null;							//幅
	D3DXMatrixIdentity(&m_mtxWorld);					//ワールドマトリックス
	m_nVertexNumber = 0;								//頂点数
	m_nIntexNumber = 0;									//インデックス数
	m_nPolygonNumber = 0;								//ポリゴン数
	m_nLineVertex = 0;									//一行の頂点数
	m_nColumnVertex = 0;								//一列の頂点数
	m_fFriction = 0.0f;									//摩擦係数
	m_fAnimAngle = 0.0f;								//アニメーション用の角度
	m_fAnimSpeed = 0.0f;								//アニメーションスピード
	m_fAmplitude = 0.0f;								//アニメーションの振幅
	m_nPriority = 0;									//プライオリティ
	m_bTextureAnim = false;								//テクスチャアニメーションのフラグ
	m_bAnim = false;									//アニメーションのフラグ
	m_animSpeed = Vec2Null;								//アニメーションスピード

	m_vMeshfield.push_back(this);						//メッシュフィールドのアドレスを持っているベクトルにこのポインタを保存する
}

//デストラクタ
CMeshfield::~CMeshfield()
{

}

//初期化処理
HRESULT CMeshfield::Init(void)
{
	m_pVtxBuff = nullptr;								//頂点バッファ
	m_pIdxBuff = nullptr;								//インデックスバッファ
	m_pTexture = nullptr;								//テクスチャへのポインタ
	m_pos = Vec3Null;									//位置
	m_rot = Vec3Null;									//向き
	m_size = Vec2Null;									//サイズ
	m_LateralCoords = Vec2Null;							//幅
	D3DXMatrixIdentity(&m_mtxWorld);					//ワールドマトリックス
	m_nVertexNumber = 0;								//頂点数
	m_nIntexNumber = 0;									//インデックス数
	m_nPolygonNumber = 0;								//ポリゴン数
	m_nLineVertex = 0;									//一行の頂点数
	m_nColumnVertex = 0;								//一列の頂点数
	m_fFriction = 0.1f;									//摩擦係数
	m_fAnimAngle = 0.0f;								//アニメーション用の角度
	m_fAnimSpeed = D3DX_PI * 0.025f;					//アニメーションスピード
	m_fAmplitude = 25.0f;								//アニメーションの振幅
	m_nPriority = 0;									//プライオリティ
	m_animSpeed = Vec2Null;								//アニメーションスピード
	m_bTextureAnim = false;								//テクスチャアニメーションのフラグ
	m_bAnim = false;									//アニメーションのフラグ

	return S_OK;
}

//終了処理
void CMeshfield::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//インデックスバッファの破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
	//テクスチャへのポインタをnullにする
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	int a = m_vMeshfield.size();

	//このインスタンスのアドレスを消す
	for (int nCnt = 0; nCnt < a; nCnt++)
	{
		if (m_vMeshfield.data()[nCnt] == this)
		{
			m_vMeshfield.erase(m_vMeshfield.begin() + nCnt);
			break;
		}
	}
}

//更新処理
void CMeshfield::Update(void)
{
	//テクスチャアニメーションのフラグがtrueだったら、
	if (m_bTextureAnim)
	{
		//頂点情報へのポインタ
		VERTEX_3D*pVtx = nullptr;
		VERTEX_3D Vtx;
		ZeroMemory(&Vtx, sizeof(VERTEX_3D));

		//頂点バッファをロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点情報の設定
		for (int nCnt = 0; nCnt < m_nVertexNumber; nCnt++)
		{
			Vtx.tex = pVtx[nCnt].tex;
			Vtx.tex += m_animSpeed;
			pVtx[nCnt].tex = Vtx.tex;
		}

		//頂点バッファのアンロック
		m_pVtxBuff->Unlock();
	}
	if (m_bAnim)
	{//アニメーションのフラグがtrueだったら(波みたいなアニメーション)
		VERTEX_3D*pVtx = nullptr;
		VERTEX_3D Vtx;
		ZeroMemory(&Vtx, sizeof(VERTEX_3D));

		//頂点バッファをロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点情報の設定
		for (int nCnt = 0; nCnt < m_nVertexNumber; nCnt++)
		{
			Vtx.pos = pVtx[nCnt].pos;													//頂点の位置を取得
			Vtx.pos.y += m_fAmplitude * sinf(m_fAnimAngle * (nCnt / m_nLineVertex));	//Y座標を更新する
			pVtx[nCnt].pos = Vtx.pos;													//新しい位置の設定
		}

		m_fAnimAngle += m_fAnimSpeed;													//角度の更新

		//頂点バッファのアンロック
		m_pVtxBuff->Unlock();

	}
}

//描画処理
void CMeshfield::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX	mtxRot, mtxTrans;											//計算用マトリックス

	//ワルドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワルドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画処理
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nVertexNumber, 0, m_nPolygonNumber);

	pDevice->SetTexture(0, NULL);
}

//位置の設定処理
void CMeshfield::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//プライオリティの設定処理
void CMeshfield::SetPriority(const int nPriority)
{
	m_nPriority = nPriority;

	//範囲外の値がないように確認する
	if (m_nPriority < 0)
	{
		m_nPriority = 0;
	}
	else if (m_nPriority >= MAX_FIELD_PRIORITY)
	{
		m_nPriority = MAX_FIELD_PRIORITY - 1;
	}
}

//サイズの取得処理
const D3DXVECTOR2 CMeshfield::GetSize(void)
{
	return m_size;
}

//位置の取得処理
const D3DXVECTOR3 CMeshfield::GetPos(void)
{
	return m_pos;
}

//摩擦係数の取得処理
const float CMeshfield::GetFriction(void)
{
	return m_fFriction;
}

//テクスチャの設定処理
void CMeshfield::SetTexture(CObject::TextType texture)
{
	LPDIRECT3DTEXTURE9 text = CObject_2D::GetTexturePointer(texture);		//ロードしたテクスチャのポインタを取得
	m_pTexture = text;														//テクスチャを設定する
}

//テクスチャの大きさの設定処理
void CMeshfield::SetTextureTiling(D3DXVECTOR2 TileSize)
{
	//頂点情報へのポインタ
	VERTEX_3D*pVtx = nullptr;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	for (int nCnt = 0; nCnt < m_nVertexNumber; nCnt++)
	{
		//テクスチャ座標の設定
		pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (TileSize.x * (nCnt % m_nColumnVertex)), 0.0f + (TileSize.y * (nCnt / m_nColumnVertex)));
	}

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//テクスチャの大きさの設定処理
void CMeshfield::SetTextureTiling(float fTileSize)
{
	//頂点情報へのポインタ
	VERTEX_3D*pVtx = nullptr;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	for (int nCnt = 0; nCnt < m_nVertexNumber; nCnt++)
	{
		//テクスチャ座標の設定
		pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (fTileSize * (nCnt % m_nColumnVertex)), 0.0f + (fTileSize * (nCnt / m_nColumnVertex)));
	}

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//テクスチャアニメーションの設定処理
void CMeshfield::SetTextureAnim(const D3DXVECTOR2 animSpeed)
{
	m_bTextureAnim = true;
	m_animSpeed = animSpeed;
}

//テクスチャアニメーションの設定処理
void CMeshfield::SetTextureAnim(const float fX, const float fY)
{
	m_bTextureAnim = true;
	m_animSpeed = D3DXVECTOR2(fX, fY);
}

//テクスチャアニメーションの停止処理
void CMeshfield::StopTextureAnim(void)
{
	m_bTextureAnim = false;
	m_animSpeed = Vec2Null;
}

//アニメーションの設定処理
void CMeshfield::SetAnimation(const bool bAnim)
{
	m_bAnim = bAnim;
}

//アニメーションの設定処理
void CMeshfield::SetAnimation(const bool bAnim, const float fAngularSpeed, const float fAmplitude)
{
	m_bAnim = bAnim;
	m_fAmplitude = fAmplitude;
	m_fAnimSpeed = fAngularSpeed;
}

//頂点の高さの設定処理
void CMeshfield::ChangeHeight(const int nStartVtx, const int nEndVtx, const float fHeight)
{
	int nStart = nStartVtx, nEnd = nEndVtx;

	//存在している頂点の範囲外インデックスがないように確認する
	if (nStart < 0)
	{
		nStart = 0;
	}
	if (nEnd > m_nVertexNumber)
	{
		nEnd = m_nVertexNumber;
	}

	//頂点情報へのポインタ
	VERTEX_3D*pVtx = nullptr;
	VERTEX_3D Vtx = {};

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	for (int nCnt = nStart; nCnt < nEnd; nCnt++)
	{
		Vtx = pVtx[nCnt];

		//テクスチャ座標の設定
		Vtx.pos.y = fHeight;

		pVtx[nCnt] = Vtx;
	}

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}




//=============================================================================
//
//								静的関数
//
//=============================================================================



//生成処理
CMeshfield* CMeshfield::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 unitSize, const int nColumn, const int nLine)
{
	CMeshfield* pField = new CMeshfield(3);				//メッシュフィールドを生成する

	if (FAILED(pField->Init()))
	{//初期化処理
		return nullptr;
	}

	pField->m_pos = pos;						//位置の設定
	pField->m_rot = rot;						//向きの設定
	pField->m_size = unitSize;					//サイズの設定
	pField->m_nColumnVertex = nLine;			//1つの列の頂点の設定
	pField->m_nLineVertex = nColumn;			//1つの行の頂点の設定
	pField->SetVertex();						//頂点の設定処理

	return pField;								//生成したインスタンスを返す
}

//生成処理
CMeshfield* CMeshfield::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 unitSize, const int nColumn, const int nLine, const int nPriority)
{
	CMeshfield* pField = new CMeshfield(nPriority);			//メッシュフィールドを生成する

	if (FAILED(pField->Init()))
	{//初期化処理
		return nullptr;
	}

	pField->m_pos = pos;						//位置の設定
	pField->m_rot = rot;						//向きの設定
	pField->m_size = unitSize;					//サイズの設定
	pField->m_nColumnVertex = nLine;			//1つの列の頂点の設定
	pField->m_nLineVertex = nColumn;			//1つの行の頂点の設定
	pField->SetVertex();						//頂点の設定処理

	return pField;								//生成したインスタンスを返す
}

//生成処理
CMeshfield* CMeshfield::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 unitSize, const int nColumn, const int nLine, const float fFriction)
{
	CMeshfield* pField = new CMeshfield(3);		//メッシュフィールドを生成する

	if (FAILED(pField->Init()))
	{//初期化処理
		return nullptr;
	}

	pField->m_pos = pos;						//位置の設定
	pField->m_rot = rot;						//向きの設定
	pField->m_size = unitSize;					//サイズの設定
	pField->m_nColumnVertex = nLine;			//1つの列の頂点の設定
	pField->m_nLineVertex = nColumn;			//1つの行の頂点の設定
	pField->SetVertex();						//頂点の設定処理
	pField->m_fFriction = fFriction;			//摩擦係数の設定


	return pField;								//生成したインスタンスを返す
}

//生成処理
CMeshfield* CMeshfield::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 unitSize, const int nColumn, const int nLine, const int nPriority, const float fFriction)
{
	CMeshfield* pField = new CMeshfield(nPriority);			//メッシュフィールドを生成する

	if (FAILED(pField->Init()))
	{//初期化処理
		return nullptr;
	}

	pField->m_pos = pos;						//位置の設定
	pField->m_rot = rot;						//向きの設定
	pField->m_size = unitSize;					//サイズの設定
	pField->m_nColumnVertex = nLine;			//1つの列の頂点の設定
	pField->m_nLineVertex = nColumn;			//1つの行の頂点の設定
	pField->SetVertex();						//頂点の設定処理
	pField->m_fFriction = fFriction;			//摩擦係数の設定

	return pField;								//生成したインスタンスを返す
}

//メッシュフィールドとの当たり判定
bool CMeshfield::FieldInteraction(CObject* pObj)
{
	int nFieldNum = m_vMeshfield.size();		//メッシュフィールドの数を取得する

	D3DXVECTOR3 pos = pObj->GetPos();			//オブジェクトの位置の取得

	for (int nCntField = 0; nCntField < nFieldNum; nCntField++)
	{//全部のメッシュフィールドを確認する
		CMeshfield* pField = m_vMeshfield.data()[nCntField];		//現在のメッシュフィールドを設定する

		VERTEX_3D* pVtx = nullptr;	//頂点情報へのポインタ

		//頂点バッファをロック
		pField->m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		WORD*pIdx = nullptr;		//インデックス情報へのポインタ

		//インデックスバッファをロック
		pField->m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCnt = 0; nCnt < pField->m_nPolygonNumber; nCnt++)
		{//メッシュフィールドの全部のポリゴンを確認する

			//計算用のベクトル
			D3DXVECTOR3 Vtx[3] = {};
			D3DXVECTOR3 Edge[3] = {};
			D3DXVECTOR3 Distance[3] = {};
			D3DXVECTOR3 Cross[3] = {};

			//ポリゴンの頂点座標の設定
			Vtx[0] = pVtx[pIdx[nCnt]].pos;			
			Vtx[1] = pVtx[pIdx[nCnt + 1]].pos;
			Vtx[2] = pVtx[pIdx[nCnt + 2]].pos;

			//計算用のマトリックス
			D3DXMATRIX mtxOut, mtxTrans, mtxRot;

			//ワルドマトリックスの初期化
			D3DXMatrixIdentity(&mtxOut);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pField->m_rot.y, pField->m_rot.x, pField->m_rot.z);
			D3DXMatrixMultiply(&mtxOut, &mtxOut, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, pField->m_pos.x, pField->m_pos.y, pField->m_pos.z);
			D3DXMatrixMultiply(&mtxOut, &mtxOut, &mtxTrans);

			//頂点座標をグローバル座標に変換する
			D3DXVec3TransformCoord(&Vtx[0], &Vtx[0], &mtxOut);
			D3DXVec3TransformCoord(&Vtx[1], &Vtx[1], &mtxOut);
			D3DXVec3TransformCoord(&Vtx[2], &Vtx[2], &mtxOut);

			//各頂点から次の頂点までのベクトル
			Edge[0] = Vtx[1] - Vtx[0];
			Edge[1] = Vtx[2] - Vtx[1];
			Edge[2] = Vtx[0] - Vtx[2];

			//各頂点からオブジェクトまでのベクトル
			Distance[0] = pos - Vtx[0];
			Distance[1] = pos - Vtx[1];
			Distance[2] = pos - Vtx[2];

			//上設定したベクトルの外積を計算する
			D3DXVec3Cross(&Cross[0], &Edge[0], &Distance[0]);
			D3DXVec3Cross(&Cross[1], &Edge[1], &Distance[1]);
			D3DXVec3Cross(&Cross[2], &Edge[2], &Distance[2]);

			if (Cross[0].y * Cross[1].y >= 0 && Cross[0].y * Cross[2].y >= 0 && Cross[1].y * Cross[2].y >= 0)
			{//符号が同じなら、オブジェクトはポリゴンと重なっている

				//ポリゴンの面の法線を計算する
				D3DXVECTOR3 Normal = Vec3Null;		

				//ポリゴンの向きによって外積の符号が違うので、分けます
				if (nCnt % 2 == 0)
				{
					D3DXVec3Cross(&Normal, &Edge[0], &Edge[1]);
				}
				else
				{
					D3DXVec3Cross(&Normal, &Edge[1], &Edge[0]);
				}

				D3DXVec3Normalize(&Normal, &Normal);				//法線を正規化する

				float Y = (Vtx[0].y) - ((((pos.x - (Vtx[0].x)) * Normal.x) + ((pos.z - Vtx[0].z) * Normal.z)) / Normal.y);			//オブジェクトがある所のポリゴンの高さ

				if (pos.y < Y && pos.y + 50.0f >= Y)
				{//オブジェクトはポリゴンにめり込んだ場合、押し出す
					pos.y = Y;
					pObj->SetPos(pos);				//位置の設定
					return true;					//trueを返す
					break;
				}
			}
			
		}

		//インデックスバッファをアンロック
		pField->m_pIdxBuff->Unlock();

		//頂点バッファのアンロック
		pField->m_pVtxBuff->Unlock();
	}

	return false;
}

//当たり判定の処理
CMeshfield* CMeshfield::FieldInteraction(CObject* pObj, float* fHeight)
{
	int nFieldNum = m_vMeshfield.size();				//メッシュフィールドの数を取得する

	D3DXVECTOR3 pos = pObj->GetPos();					//オブジェクトの位置の取得

	for (int nCntPriority = 0; nCntPriority < MAX_FIELD_PRIORITY; nCntPriority++)
	{//プライオリティの順番で確認する
		for (int nCntField = 0; nCntField < nFieldNum; nCntField++)
		{//全部のメッシュフィールドを確認する
			CMeshfield* pField = m_vMeshfield.data()[nCntField];		//現在のメッシュフィールドを設定する

			if (pField->m_nPriority == nCntPriority)
			{
				VERTEX_3D* pVtx = nullptr;				//頂点情報へのポインタ

				//頂点バッファをロック
				pField->m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				WORD*pIdx = nullptr;		//インデックス情報へのポインタ

				//インデックスバッファをロック
				pField->m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

				for (int nCnt = 0; nCnt < pField->m_nPolygonNumber; nCnt++)
				{//メッシュフィールドの全部のポリゴンを確認する

					//計算用のベクトル
					D3DXVECTOR3 Vtx[3] = {};
					D3DXVECTOR3 Edge[3] = {};
					D3DXVECTOR3 Distance[3] = {};
					D3DXVECTOR3 Cross[3] = {};

					//ポリゴンの頂点座標の設定
					Vtx[0] = pVtx[pIdx[nCnt]].pos;
					Vtx[1] = pVtx[pIdx[nCnt + 1]].pos;
					Vtx[2] = pVtx[pIdx[nCnt + 2]].pos;

					//計算用のマトリックス
					D3DXMATRIX mtxOut, mtxTrans, mtxRot;

					//ワルドマトリックスの初期化
					D3DXMatrixIdentity(&mtxOut);

					//向きを反映
					D3DXMatrixRotationYawPitchRoll(&mtxRot, pField->m_rot.y, pField->m_rot.x, pField->m_rot.z);
					D3DXMatrixMultiply(&mtxOut, &mtxOut, &mtxRot);

					//位置を反映
					D3DXMatrixTranslation(&mtxTrans, pField->m_pos.x, pField->m_pos.y, pField->m_pos.z);
					D3DXMatrixMultiply(&mtxOut, &mtxOut, &mtxTrans);

					//頂点座標をグローバル座標に変換する
					D3DXVec3TransformCoord(&Vtx[0], &Vtx[0], &mtxOut);
					D3DXVec3TransformCoord(&Vtx[1], &Vtx[1], &mtxOut);
					D3DXVec3TransformCoord(&Vtx[2], &Vtx[2], &mtxOut);

					//各頂点から次の頂点までのベクトル
					Edge[0] = Vtx[1] - Vtx[0];
					Edge[1] = Vtx[2] - Vtx[1];
					Edge[2] = Vtx[0] - Vtx[2];

					//各頂点からオブジェクトまでのベクトル
					Distance[0] = pos - Vtx[0];
					Distance[1] = pos - Vtx[1];
					Distance[2] = pos - Vtx[2];

					//上設定したベクトルの外積を計算する
					D3DXVec3Cross(&Cross[0], &Edge[0], &Distance[0]);
					D3DXVec3Cross(&Cross[1], &Edge[1], &Distance[1]);
					D3DXVec3Cross(&Cross[2], &Edge[2], &Distance[2]);

					if (Cross[0].y * Cross[1].y >= 0 && Cross[0].y * Cross[2].y >= 0 && Cross[1].y * Cross[2].y >= 0)
					{//符号が同じなら、オブジェクトはポリゴンと重なっている

						 //ポリゴンの面の法線を計算する
						D3DXVECTOR3 Normal = Vec3Null;

						//ポリゴンの向きによって外積の符号が違うので、分けます
						if (nCnt % 2 == 0)
						{
							D3DXVec3Cross(&Normal, &Edge[0], &Edge[1]);
						}
						else
						{
							D3DXVec3Cross(&Normal, &Edge[1], &Edge[0]);
						}

						D3DXVec3Normalize(&Normal, &Normal);			//法線を正規化する

						float Y = (Vtx[0].y) - ((((pos.x - (Vtx[0].x)) * Normal.x) + ((pos.z - Vtx[0].z) * Normal.z)) / Normal.y);			//オブジェクトがある所のポリゴンの高さ

						if (pos.y < Y && pos.y + 50.0f >= Y)
						{//オブジェクトはポリゴンにめり込んだ場合、押し出す
							pos.y = Y;
							pObj->SetPos(pos);			//位置の設定
							*fHeight = Y;				//高さを設定する
							return pField;
							break;
						}
					}

				}

				//インデックスバッファをアンロック
				pField->m_pIdxBuff->Unlock();

				//頂点バッファのアンロック
				pField->m_pVtxBuff->Unlock();
			}
		}
	}

	return nullptr;
}


//頂点インデックスの設定処理
void CMeshfield::SetVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();		//デバイスの取得

	//頂点数の計算
	m_nVertexNumber = m_nLineVertex * m_nColumnVertex;
	//ポリゴン数の計算
	m_nPolygonNumber = (((((m_nColumnVertex - 1) * 2) + 2) * 2) + ((((m_nColumnVertex - 1) * 2) + 4) * ((m_nLineVertex - 1) - 2)));
	//インデックス数の計算
	m_nIntexNumber = (((((m_nColumnVertex - 1) * 2) + 2) * 2) + ((((m_nColumnVertex - 1) * 2) + 4) * ((m_nLineVertex - 1) - 2))) + 2;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertexNumber,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIntexNumber,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D*pVtx = nullptr;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_LateralCoords.x = m_pos.x;
	m_LateralCoords.y = m_pos.x + (m_size.x * (m_nColumnVertex - 1));

	//頂点情報の設定
	for (int nCnt = 0; nCnt < m_nVertexNumber; nCnt++)
	{
		//頂点の位置の初期化
		pVtx[nCnt].pos = D3DXVECTOR3((float)(m_size.x * (nCnt % m_nColumnVertex)), /*(float)CObject::random(-500, 500) * 0.1f*/0.0f, (float)(-m_size.y * (nCnt / m_nColumnVertex)));
		//頂点の法線の初期化
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//頂点カラーの初期化
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//テクスチャ座標の設定
		pVtx[nCnt].tex = D3DXVECTOR2(0.0f + (1.0f * (nCnt % m_nColumnVertex)), 0.0f + (1.0f * (nCnt / m_nColumnVertex)));

		if (nCnt < 60)
		{
			pVtx[nCnt].pos.y += 50.0f;
		}
	}

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	WORD*pIdx = nullptr;		//インデックス情報へのポインタ

	//インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nStart = 0;		//計算用のローカル変数

						//インデックス情報の設定
	for (int nCnt = 0; nCnt < m_nLineVertex - 1; nCnt++)
	{
		for (int nCntColumn = 0; nCntColumn < m_nColumnVertex * 2; nCntColumn += 2)
		{
			//一行のインデックスの設定
			pIdx[(nCnt * m_nColumnVertex) + nCntColumn + nStart] = (WORD)(m_nColumnVertex + (m_nColumnVertex * (nCnt)) + (nCntColumn / 2));
			int a = m_nColumnVertex + (m_nColumnVertex * (nCnt)) + (nCntColumn / 2);
			pIdx[(nCnt * m_nColumnVertex) + nCntColumn + nStart + 1] = (WORD)((m_nColumnVertex * (nCnt)) + (nCntColumn / 2));
			a = (m_nColumnVertex * (nCnt)) + (nCntColumn / 2);
			a++;

			if (nCntColumn + 2 >= 2 * m_nColumnVertex && nCnt != m_nLineVertex - 2)
			{
				//縮退ポリゴン用のインデックスの設定
				pIdx[(nCnt * m_nColumnVertex) + nCntColumn + nStart + 2] = (WORD)((m_nColumnVertex * (nCnt)) + (nCntColumn / 2));
				pIdx[(nCnt * m_nColumnVertex) + nCntColumn + nStart + 3] = (WORD)(m_nColumnVertex + (m_nColumnVertex * (nCnt + 1)));
				nStart += 2 + m_nColumnVertex;
			}

		}

	}

	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	pVtx = nullptr;

	pIdx = nullptr;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//頂点の法線の計算
	for (int nCnt = 0; nCnt < m_nIntexNumber - 2; nCnt++)
	{
		if (pIdx[nCnt] != pIdx[nCnt + 1] && pIdx[nCnt] != pIdx[nCnt + 2] && pIdx[nCnt + 1] != pIdx[nCnt + 2])
		{
			D3DXVECTOR3 V1, V2, Norm, N;

			V1 = pVtx[pIdx[nCnt + 1]].pos - pVtx[pIdx[nCnt]].pos;
			V2 = pVtx[pIdx[nCnt + 2]].pos - pVtx[pIdx[nCnt + 1]].pos;


			if (nCnt % 2 == 0)
			{
				D3DXVec3Cross(&Norm, &V1, &V2);
			}
			else
			{
				D3DXVec3Cross(&Norm, &V2, &V1);
			}

			D3DXVec3Normalize(&Norm, &Norm);

			N = pVtx[pIdx[nCnt]].nor + Norm;
			//D3DXVec3Normalize(&N, &N);
			pVtx[pIdx[nCnt]].nor = N;
			N = pVtx[pIdx[nCnt + 1]].nor + Norm;
			//D3DXVec3Normalize(&N, &N);
			pVtx[pIdx[nCnt + 1]].nor = N;
			N = pVtx[pIdx[nCnt + 2]].nor + Norm;
			//D3DXVec3Normalize(&N, &N);
			pVtx[pIdx[nCnt + 2]].nor = N;
		}
	}

	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	pVtx = nullptr;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//法線の設定
	for (int nCnt = 0; nCnt < m_nVertexNumber; nCnt++)
	{
		D3DXVECTOR3 N = pVtx[nCnt].nor;
		D3DXVec3Normalize(&N, &N);
		pVtx[nCnt].nor = N;
	}

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}