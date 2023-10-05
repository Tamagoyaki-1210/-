//=============================================================================
//
// BoxHitbox.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "BoxHitbox.h"
#include "line.h"

//コンストラクタ
CBoxHitbox::CBoxHitbox()
{
	for (int nCnt = 0; nCnt < 12; nCnt++)
	{
		m_pLine[nCnt] = nullptr;
	}
}

//デストラクタ
CBoxHitbox::~CBoxHitbox()
{

}

//初期化処理
HRESULT CBoxHitbox::Init(void)
{
	if (FAILED(CHitbox::Init()))
	{//基本クラスの初期化処理
		return -1;
	}

	//デバッグ用の線の初期化処理
	for (int nCnt = 0; nCnt < 12; nCnt++)
	{
		m_pLine[nCnt] = nullptr;
	}

	return S_OK;
}

//終了処理
void CBoxHitbox::Uninit(void)
{
	CHitbox::Uninit();				//基本クラスの終了処理

	//デバッグ用の線の破棄処理
	for (int nCnt = 0; nCnt < 12; nCnt++)
	{
		if (m_pLine[nCnt] != nullptr)
		{
			m_pLine[nCnt]->Release();
			m_pLine[nCnt] = nullptr;
		}
	}
}

//更新処理
void CBoxHitbox::Update(void)
{
	SetPos(GetPos() + D3DXVECTOR3(0.2f, 0.0f, 0.0f));

	std::vector <CHitbox*>* pHbx = GetAllHitbox();				//全部のヒットボックスの取得処理

	for (int nCnt = 0; nCnt < (int)pHbx->size(); nCnt++)
	{//全部の存在するヒットボックスを判定する

		if (pHbx->data()[nCnt] != this && pHbx->data()[nCnt]->GetParent() != GetParent())
		{//親が同じではなかったら

			HITBOX_SHAPE shape = pHbx->data()[nCnt]->GetShape();				//形の取得

			switch (shape)
			{
			case CHitbox::SHAPE_SPHERE:
				break;

			case CHitbox::SHAPE_BOX:

			{//矩形の場合

				if (BoxBoxHit(pHbx->data()[nCnt]->GetPos(), pHbx->data()[nCnt]->GetRot(), pHbx->data()[nCnt]->GetSize()))
				{//当たった場合

					if (GetEffect() != EFFECT_MAX && pHbx->data()[nCnt]->GetEffect() == EFFECT_MAX)
					{//エフェクトがあったら、設定する
						pHbx->data()[nCnt]->SetEffect(GetEffect());
					}
				}

			}

			break;

			case CHitbox::SHAPE_CYLINDER:

			{//シリンダーの場合

				if (BoxBoxHit(pHbx->data()[nCnt]->GetPos(), Vec3Null, pHbx->data()[nCnt]->GetSize()))
				{//当たった場合

					if (GetEffect() != EFFECT_MAX && pHbx->data()[nCnt]->GetEffect() == EFFECT_MAX)
					{//エフェクトがあったら、設定する
						pHbx->data()[nCnt]->SetEffect(GetEffect());
					}
				}
			}

			break;

			default:
				break;
			}
		}
	}

	//デバッグ用の線の更新
	for (int nCnt = 0; nCnt < 12; nCnt++)
	{
		if (m_pLine[nCnt] != nullptr)
		{
			m_pLine[nCnt]->SetPos(GetPos());
		}
	}

	CHitbox::Update();			//基本クラスの更新処理
}


//==================================================================================================================
//
//												静的関数
//
//==================================================================================================================


//生成処理
CBoxHitbox* CBoxHitbox::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 RelativePos, const D3DXVECTOR3 size, HITBOX_TYPE type, CObject* pParent)
{
	CBoxHitbox* pHitbox = new CBoxHitbox;					//ヒットボックスの生成

	if (FAILED(pHitbox->Init()))
	{//初期化処理
		return nullptr;
	}

	pHitbox->SetRelativePos(RelativePos);				//相対位置の設定
	pHitbox->SetPos(pos);								//位置の設定
	pHitbox->SetLastPos(pos);							//前回の位置の設定
	pHitbox->SetSize(size);								//サイズの設定
	pHitbox->SetType(type);								//種類の設定
	pHitbox->SetShape(CHitbox::SHAPE_BOX);				//形の設定
	pHitbox->SetParent(pParent);						//親の設定

#ifdef _DEBUG

	//デバッグ用の線の生成
	D3DXVECTOR3 VtxPos[8] = {};

	//頂点座標の設定
	VtxPos[0] = D3DXVECTOR3(-size.x, 0.0f, size.z);				
	VtxPos[1] = D3DXVECTOR3(size.x, 0.0f, size.z);				
	VtxPos[2] = D3DXVECTOR3(size.x, 0.0f, -size.z);				
	VtxPos[3] = D3DXVECTOR3(-size.x, 0.0f, -size.z);			
	VtxPos[4] = D3DXVECTOR3(-size.x, size.y, size.z);			
	VtxPos[5] = D3DXVECTOR3(size.x, size.y, size.z);			
	VtxPos[6] = D3DXVECTOR3(size.x, size.y, -size.z);			
	VtxPos[7] = D3DXVECTOR3(-size.x, size.y, -size.z);			

	//線の生成
	pHitbox->m_pLine[0] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[0], VtxPos[1], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[1] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[1], VtxPos[2], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[2] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[2], VtxPos[3], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[3] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[3], VtxPos[0], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	pHitbox->m_pLine[4] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[0], VtxPos[4], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[5] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[1], VtxPos[5], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[6] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[2], VtxPos[6], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[7] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[3], VtxPos[7], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	pHitbox->m_pLine[8] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[4], VtxPos[5], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[9] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[5], VtxPos[6], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[10] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[6], VtxPos[7], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[11] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[7], VtxPos[4], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

#endif // !DEBUG


	return pHitbox;					//生成したインスタンスを返す
}

//生成処理
CBoxHitbox* CBoxHitbox::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 RelativePos, const D3DXVECTOR3 size, HITBOX_TYPE type, CObject* pParent, const int nScore)
{
	CBoxHitbox* pHitbox = new CBoxHitbox;				//ヒットボックスの生成

	if (FAILED(pHitbox->Init()))
	{//初期化処理
		return nullptr;
	}

	pHitbox->SetRelativePos(RelativePos);				//相対位置の設定
	pHitbox->SetPos(pos);								//位置の設定
	pHitbox->SetLastPos(pos);							//前回の位置の設定
	pHitbox->SetSize(size);								//サイズの設定
	pHitbox->SetType(type);								//種類の設定
	pHitbox->SetShape(CHitbox::SHAPE_BOX);				//形の設定
	pHitbox->SetParent(pParent);						//親の設定
	pHitbox->SetScore(nScore);							//スコアの設定

#ifdef _DEBUG

	//デバッグ用の線の生成
	D3DXVECTOR3 VtxPos[8] = {};

	//頂点座標の設定
	VtxPos[0] = D3DXVECTOR3(-size.x, 0.0f, size.z);
	VtxPos[1] = D3DXVECTOR3(size.x, 0.0f, size.z);
	VtxPos[2] = D3DXVECTOR3(size.x, 0.0f, -size.z);
	VtxPos[3] = D3DXVECTOR3(-size.x, 0.0f, -size.z);
	VtxPos[4] = D3DXVECTOR3(-size.x, size.y, size.z);
	VtxPos[5] = D3DXVECTOR3(size.x, size.y, size.z);
	VtxPos[6] = D3DXVECTOR3(size.x, size.y, -size.z);
	VtxPos[7] = D3DXVECTOR3(-size.x, size.y, -size.z);

	//線の生成
	pHitbox->m_pLine[0] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[0], VtxPos[1], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[1] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[1], VtxPos[2], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[2] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[2], VtxPos[3], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[3] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[3], VtxPos[0], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	pHitbox->m_pLine[4] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[0], VtxPos[4], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[5] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[1], VtxPos[5], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[6] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[2], VtxPos[6], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[7] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[3], VtxPos[7], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	pHitbox->m_pLine[8] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[4], VtxPos[5], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[9] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[5], VtxPos[6], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[10] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[6], VtxPos[7], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[11] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[7], VtxPos[4], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

#endif // !DEBUG


	return pHitbox;						//生成したインスタンスを返す
}

//生成処理
CBoxHitbox* CBoxHitbox::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 RelativePos, const D3DXVECTOR3 size, HITBOX_TYPE type, CObject* pParent, const int nScore, INTERACTION_EFFECT effect)
{
	CBoxHitbox* pHitbox = new CBoxHitbox;				//ヒットボックスの生成

	if (FAILED(pHitbox->Init()))
	{//初期化処理
		return nullptr;
	}

	pHitbox->SetRelativePos(RelativePos);				//相対位置の設定
	pHitbox->SetPos(pos);								//位置の設定
	pHitbox->SetLastPos(pos);							//前回の位置の設定
	pHitbox->SetSize(size);								//サイズの設定
	pHitbox->SetType(type);								//種類の設定
	pHitbox->SetShape(CHitbox::SHAPE_BOX);				//形の設定
	pHitbox->SetParent(pParent);						//親の設定
	pHitbox->SetScore(nScore);							//スコアの設定
	pHitbox->SetEffect(effect);							//エフェクトの設定

#ifdef _DEBUG

	//デバッグ用の線の生成
	D3DXVECTOR3 VtxPos[8] = {};

	//頂点座標の設定
	VtxPos[0] = D3DXVECTOR3(-size.x, 0.0f, size.z);
	VtxPos[1] = D3DXVECTOR3(size.x, 0.0f, size.z);
	VtxPos[2] = D3DXVECTOR3(size.x, 0.0f, -size.z);
	VtxPos[3] = D3DXVECTOR3(-size.x, 0.0f, -size.z);
	VtxPos[4] = D3DXVECTOR3(-size.x, size.y, size.z);
	VtxPos[5] = D3DXVECTOR3(size.x, size.y, size.z);
	VtxPos[6] = D3DXVECTOR3(size.x, size.y, -size.z);
	VtxPos[7] = D3DXVECTOR3(-size.x, size.y, -size.z);

	//線の生成
	pHitbox->m_pLine[0] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[0], VtxPos[1], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[1] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[1], VtxPos[2], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[2] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[2], VtxPos[3], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[3] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[3], VtxPos[0], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	pHitbox->m_pLine[4] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[0], VtxPos[4], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[5] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[1], VtxPos[5], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[6] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[2], VtxPos[6], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[7] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[3], VtxPos[7], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	pHitbox->m_pLine[8] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[4], VtxPos[5], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[9] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[5], VtxPos[6], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[10] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[6], VtxPos[7], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[11] = CLine::Create(pos + RelativePos, Vec3Null, VtxPos[7], VtxPos[4], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

#endif // !DEBUG


	return pHitbox;						//生成したインスタンスを返す
}



//==================================================================================================================
//
//											プライベート関数
//
//==================================================================================================================


//当たり判定の処理
bool CBoxHitbox::BoxBoxHit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	D3DXVECTOR3 thisPos = GetPos();						//このヒットボックスの位置の取得
	D3DXVECTOR3 thisSize = GetSize();					//このヒットボックスのサイズの取得
	D3DXVECTOR3 thisLastPos = GetLastPos();				//このヒットボックスの前回の位置の取得

	if (thisPos.y + thisSize.y > pos.y && thisPos.y < pos.y + size.y)
	{//Y座標が重なった場合

		D3DXVECTOR3 Vtx[4], V[4], P[4], PCtrl, R[4], RCtrl, N;		//計算用のベクトル
		D3DXMATRIX mtxOut, mtxTrans, mtxRot;						//計算用のマトリックス
		float fResult;												//計算用の変数

		//頂点の相対座標の設定
		Vtx[0] = D3DXVECTOR3(-size.x, 0.0f, size.z);				
		Vtx[1] = D3DXVECTOR3(size.x, 0.0f, size.z);
		Vtx[2] = D3DXVECTOR3(size.x, 0.0f, -size.z);
		Vtx[3] = D3DXVECTOR3(-size.x, 0.0f, -size.z);

		//絶対座標に変換する
		D3DXMatrixIdentity(&mtxOut);										
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);		
		D3DXMatrixMultiply(&mtxOut, &mtxOut, &mtxRot);						
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);				
		D3DXMatrixMultiply(&mtxOut, &mtxOut, &mtxTrans);					

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			D3DXVec3TransformCoord(&Vtx[nCnt], &Vtx[nCnt], &mtxOut);
		}

		//各頂点から次の頂点までのベクトルを計算する
		V[0] = Vtx[1] - Vtx[0];
		V[1] = Vtx[2] - Vtx[1];
		V[2] = Vtx[3] - Vtx[2];
		V[3] = Vtx[0] - Vtx[3];

		//各頂点からこのインスタンスの位置までのベクトルを計算する
		P[0] = D3DXVECTOR3(thisPos.x - Vtx[0].x, 0.0f, thisPos.z - Vtx[0].z);
		P[1] = D3DXVECTOR3(thisPos.x - Vtx[1].x, 0.0f, thisPos.z - Vtx[1].z);
		P[2] = D3DXVECTOR3(thisPos.x - Vtx[2].x, 0.0f, thisPos.z - Vtx[2].z);
		P[3] = D3DXVECTOR3(thisPos.x - Vtx[3].x, 0.0f, thisPos.z - Vtx[3].z);

		//上に計算したベクトルの外積を計算する
		D3DXVec3Cross(&R[0], &V[0], &P[0]);
		D3DXVec3Cross(&R[1], &V[1], &P[1]);
		D3DXVec3Cross(&R[2], &V[2], &P[2]);
		D3DXVec3Cross(&R[3], &V[3], &P[3]);

		if (R[0].y * R[1].y >= 0 && R[1].y * R[2].y >= 0 && R[2].y * R[3].y >= 0 && R[3].y * R[0].y >= 0)
		{//全部の外積の符号が同じだったら、当たったという意味です

			if (thisLastPos.y >= pos.y + size.y && thisPos.y < pos.y + size.y)
			{//上から当たった場合

				thisPos.y = pos.y + size.y;								//上に押し出す
				GetParent()->SetPos(thisPos - GetRelativePos());		//親も押し出す

				return true;											//trueを返す
			}
			else if (thisLastPos.y + thisSize.y <= pos.y && thisPos.y + thisSize.y <= pos.y)
			{//下から当たった場合

				thisPos.y = pos.y - thisSize.y;							//下に押し出す
				GetParent()->SetPos(thisPos - GetRelativePos());		//親も押し出す

				return true;											//trueを返す
			}
			else
			{//横から当たった場合
				for (int Count = 0; Count < 4; Count++)
				{//前回の位置と比べて、どこから当たったか求める

					PCtrl = D3DXVECTOR3(thisLastPos.x - Vtx[Count].x, 0.0f, thisLastPos.z - Vtx[Count].z);
					D3DXVec3Cross(&RCtrl, &V[Count], &PCtrl);

					D3DXVECTOR3 Ctrl;

					if (RCtrl.y * R[Count].y <= 0)
					{//交点座標の計算
						P[0] = D3DXVECTOR3(thisPos.x - thisLastPos.x, 0.0f, thisPos.z - thisLastPos.z);			//前回の位置から現在の位置までのベクトル
						P[1] = D3DXVECTOR3(Vtx[Count].x - thisLastPos.x, 0.0f, Vtx[Count].z - thisLastPos.z);	//頂点から前回の位置までのベクトル
						D3DXVec3Normalize(&N, &P[0]);															//前回の位置から現在の位置までのベクトルを正規化する

						D3DXVec3Cross(&R[0], &P[1], &V[Count]);		//頂点から前回の位置までのベクトルと矩形の辺のベクトルの外積
						D3DXVec3Cross(&R[1], &N, &V[Count]);		//前回の位置から現在の位置までの単位ベクトルと矩形の辺のベクトルの外積

						if (R[1].y == 0)
						{
							break;
						}

						fResult = R[0].y / R[1].y;					//前回の位置から交点までのベクトルの長さ

						if (fResult > 0)
						{
							fResult -= 0.1f;
						}
						else if (fResult < 0)
						{
							fResult += 0.1f;
						}

						D3DXVECTOR3 Cross, VtxtoPos, VtxtoCross;			//計算用のベクトル
						float Dot, Lenght1, Lenght2, Alpha;					//計算用の変数

						//滑る処理
						Cross = D3DXVECTOR3(thisLastPos.x + (N.x * fResult), 0.0f, thisLastPos.z + (N.z * fResult));		//交点座標
						VtxtoPos = D3DXVECTOR3(thisPos.x - Vtx[Count].x, 0.0f, thisPos.z - Vtx[Count].z);					//頂点から現在の位置までのベクトル
						VtxtoCross = D3DXVECTOR3(Cross.x - Vtx[Count].x, 0.0f, Cross.z - Vtx[Count].z);						//頂点から交点までのベクトル

						D3DXVec3Normalize(&N, &VtxtoCross);			//頂点から交点までのベクトルを正規化する

						Dot = D3DXVec3Dot(&VtxtoCross, &VtxtoPos);											//内積を計算する
						Lenght1 = sqrtf((VtxtoCross.x * VtxtoCross.x) + (VtxtoCross.z * VtxtoCross.z));		//頂点から交点までのベクトルの長さ
						Lenght2 = sqrtf((VtxtoPos.x * VtxtoPos.x) + (VtxtoPos.z * VtxtoPos.z));				//頂点から現在の位置までのベクトルの長さ

						if ((Lenght1 * Lenght2) * (Lenght1 * Lenght2) >= Dot * Dot && Lenght1 * Lenght2 != 0)
						{
							Alpha = acosf((Dot) / (Lenght1 * Lenght2));				//上のベクトルの間の角度を計算する

							fResult = (Lenght2 * cosf(Alpha));

							//新しい座標の計算
							thisPos.x = Vtx[Count].x + (N.x * fResult);
							thisPos.z = Vtx[Count].z + (N.z * fResult);
						}
						else
						{//エラーが起きたら、新しい座標を公転と同じにする
							thisPos.x = thisLastPos.x + (N.x * fResult);
							thisPos.z = thisLastPos.z + (N.z * fResult);
						}

						break;
					}
				}

				if (GetParent() != nullptr)
				{//親の位置の設定
					GetParent()->SetPos(thisPos - GetRelativePos());
				}

				return true;				//trueを返す
			}
		}
	}

	return false;				//falseを返す
}



