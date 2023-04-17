//=========================================
// 
// プレイヤーの処理
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "input.h"
#include "player.h"
#include "disk.h"
#include <assert.h>
#include <stdio.h>
#include "shadow.h"
#include "stage.h"
#include "landingPoint.h"
#include "effect.h"
#include "game.h"
#include "goal.h"
#include "sound.h"

//-----------------------------------------
// マクロ定義
//-----------------------------------------
#define NUM_PLAYER		(2)								// プレイヤーの数
#define PLAYER_FILE		"data/player.txt"				// プレイヤー読み込みファイル
#define PLAYER_SIZ		(45.0f)							// プレイヤーの大きさ
#define ZERO_VECTOR		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// ゼロベクトル

//-----------------------------------------
// スタティック変数
//-----------------------------------------
static Player s_player[NUM_PLAYER] = {};
static Player s_playerType[PLAYERTYPE_MAX] = {};
static D3DXVECTOR3 moveCurve;
static bool s_bCurveInput;

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
static void MovePlayer(int nIdxPlayer);
static void JumpPlayer(int nIdxPlayer);
static void ThrowPlayer(int nIdxPlayer);
static void ThrowDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acc, DISK_TYPE type, int nIdxPlayer);
static void ChargePlayer(int nIdxPlayer);
static void CatchDiscPlayer(int nIdxPlayer);
static void MoveLimitPlayer(int nIdxPlayer);
static void UpdateVtxPosPlayer(int nIdxPlayer);
static void LoadPlayer(void);
static bool CollisionCircle(D3DXVECTOR3 Pos1, float fRadius1, D3DXVECTOR3 Pos2, float fRadius2);	// 円同士の読み込み処理
static D3DXVECTOR3 InputMovePlayer(int nIdxPlayer);

//=========================================
// プレイヤーの初期化処理
//=========================================
void InitPlayer(void)
{
	//初期化
	ZeroMemory(s_player, sizeof(s_player));
	ZeroMemory(s_playerType, sizeof(s_playerType));

	moveCurve = ZERO_VECTOR;
	LoadPlayer();	// 読み込み

	// プレイヤーの設定
	SetPlayer(D3DXVECTOR3(200.0f, SCREEN_HEIGHT * 0.5f, 0.0f), PLAYERTYPE_1);
	//s_player[0].nIdxShadow = SetShadow(s_player[0].pos, PLAYER_SIZ * 3.0f);

	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT * 0.5f, 0.0f), PLAYERTYPE_2);
	//s_player[1].nIdxShadow = SetShadow(s_player[1].pos, PLAYER_SIZ * 3.0f);
}

//=========================================
// プレイヤーの終了処理
//=========================================
void UninitPlayer(void)
{
	Player *pPlayer = s_player;

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		// テクスチャの破棄
		if (pPlayer->pTexture != NULL)
		{
			pPlayer->pTexture->Release();
			pPlayer->pTexture = NULL;
		}

		// 頂点バッファの破棄
		if (pPlayer->pVtxBuff != NULL)
		{
			pPlayer->pVtxBuff->Release();
			pPlayer->pVtxBuff = NULL;
		}
	}
}

//=========================================
// プレイヤーの更新処理
//=========================================
void UpdatePlayer(void)
{
	Player *pPlayer = s_player;

	for (int nIdxPlayer = 0; nIdxPlayer < NUM_PLAYER; nIdxPlayer++, pPlayer++)
	{
		pPlayer->pos += pPlayer->move;

		// 影の位置の調整
		//SetPositionShadow(pPlayer->nIdxShadow, pPlayer->pos);

		if (pPlayer->bHaveDisk)
		{ //ディスクを所持してる場合

			pPlayer->move -= pPlayer->move * pPlayer->fAttenuationSlidingSpead;	// 移動量の減衰

			// 投げる
			ThrowPlayer(nIdxPlayer);

			if (pPlayer->jumpstate == JUMP_NOW)
			{
				if (pPlayer->fVerticalSpeed > 0.0f)
				{
					pPlayer->fHeight += pPlayer->fVerticalSpeed;
					pPlayer->fVerticalSpeed -= 0.15f;
				}

				if (pPlayer->fThrowPower <= 3.5f)	// ここの条件式は後調整
				{	// 一定時間の経過で床に戻る
					pPlayer->fHeight += pPlayer->fVerticalSpeed;
					pPlayer->fVerticalSpeed -= 0.15f;

					if (pPlayer->fHeight <= 0.0f)
					{
						pPlayer->fVerticalSpeed = 2.5f;
						pPlayer->fHeight = 0.0f;
						pPlayer->jumpstate = JUMP_NONE;
					}
				}
			}
		}
		else
		{
			switch (pPlayer->jumpstate)
			{
			case JUMP_NONE:
				if (!*GetResetScore())
				{ // リセット中ではない
				  // 移動
					MovePlayer(nIdxPlayer);
					// チャージ
					ChargePlayer(nIdxPlayer);
					// 跳躍
					JumpPlayer(nIdxPlayer);
				}
				break;
			case JUMP_NOW:
				pPlayer->fHeight += pPlayer->fVerticalSpeed;
				pPlayer->fVerticalSpeed -= 0.15f;

				if (pPlayer->fHeight <= 0.0f)
				{
					pPlayer->fVerticalSpeed = 5.0f;
					pPlayer->fHeight = 0.0f;
					pPlayer->jumpstate = JUMP_NONE;
				}

				break;
			default:
				assert(false);
				break;
			}
		}

		// 受け取る
		CatchDiscPlayer(nIdxPlayer);

		// 移動制限
		MoveLimitPlayer(nIdxPlayer);

		// 頂点座標の更新
		UpdateVtxPosPlayer(nIdxPlayer);
	}
}

//=========================================
// プレイヤーの描画処理
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント
	Player *pPlayer = GetPlayer();

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		if (!pPlayer->bUse)
		{
			continue;
		}

		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, pPlayer->pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, pPlayer->pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=========================================
// プレイヤーの移動処理
//=========================================
void MovePlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];

	// 方向入力処理
	D3DXVECTOR3 inputMove = InputMovePlayer(nIdxPlayer);

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPadの有無
		if (GetJoypadTrigger(JOYKEY_A, nIdxPlayer))
		{ //スライディングの使用
			pPlayer->bUseSliding = true;
			PlaySound(SOUND_LABEL_SE_SLIDING);
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_B))
		{ //スライディングの使用
			pPlayer->bUseSliding = true;
			PlaySound(SOUND_LABEL_SE_SLIDING);
		}
	}

	if (pPlayer->bUseSliding)
	{ // スライディング
		if (pPlayer->nSlidingRigorCnt == 0)
		{
			D3DXVec3Normalize(&inputMove, &inputMove);	// 長さの正規化
			pPlayer->move = inputMove * pPlayer->fSlidingVolume;
			pPlayer->nSlidingRigorCnt++;
		}
		else if (pPlayer->nSlidingRigorCnt >= pPlayer->nSlidingRigorMax)
		{
			pPlayer->nSlidingRigorCnt = 0;
			pPlayer->bUseSliding = false;
		}
		else
		{
			pPlayer->move -= pPlayer->move * pPlayer->fAttenuationSlidingSpead;	// 移動量の減衰
			pPlayer->nSlidingRigorCnt++;

			// エフェクトの生成
			for (int i = 0; i < 25; i++)
			{
				SetEffect(pPlayer->pos, 0.0f, EFFECT_TYPE_SLIDING_IMPACT_2);
			}
			for (int i = 0; i < 10; i++)
			{
				SetEffect(pPlayer->pos, 0.0f, EFFECT_TYPE_SLIDING_IMPACT_3);
			}
		}
	}
	else
	{
		if (D3DXVec3Length(&inputMove) > 0.0f)
		{
			pPlayer->move = inputMove * pPlayer->fMoveSpeed;
			D3DXVec3Normalize(&inputMove, &inputMove);	// 長さの正規化
		}

		// 減衰処理
		pPlayer->move -= pPlayer->move * pPlayer->fAttenuationMoveSpead;
	}
}

//=========================================
// プレイヤーのジャンプ処理
//=========================================
void JumpPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];

	if ((IsJoyPadUse(nIdxPlayer) && GetJoypadTrigger(JOYKEY_Y, nIdxPlayer))
		|| !IsJoyPadUse(nIdxPlayer) && GetKeyboardTrigger(DIK_J))
	{
		pPlayer->move = ZERO_VECTOR;
		pPlayer->jumpstate = JUMP_NOW;
	}
}

//=========================================
// プレイヤーの投げる処理
//=========================================
void ThrowPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];

	// 相手プレイヤー方向にベクトルを向かせる
	D3DXVECTOR3 inputVec = ZERO_VECTOR;

	// 入力処理
	inputVec += InputMovePlayer(nIdxPlayer);

	if (D3DXVec3Length(&inputVec) >= 0.95f)
	{
		if (!s_bCurveInput)
		{
			// どれくらい曲がるかの入力
			moveCurve.y = inputVec.y;
		}
		s_bCurveInput = true;
	}
	else
	{
		moveCurve = ZERO_VECTOR;
		s_bCurveInput = false;
	}

	inputVec.x += (nIdxPlayer == 0) ? 2.0f : -2.0f;

	D3DXVECTOR3 move = inputVec * pPlayer->fThrowPower;

	if (pPlayer->fThrowPower >= 2.5f)
	{
		// 威力の減衰
		pPlayer->fThrowPower -= pPlayer->fThrowPower * 0.005f;
	}
	else
	{
		// 強制排出
		ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_NORMAL, nIdxPlayer);
	}

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPad入力
		switch (pPlayer->jumpstate)
		{
		case JUMP_NONE:
			if (GetJoypadTrigger(JOYKEY_A, nIdxPlayer))
			{
				if (pPlayer->nSpecialSkillCnt <= 40)
				{
					if (inputVec.y - moveCurve.y <= 0.1f)
					{
						moveCurve = ZERO_VECTOR;
					}

					ThrowDisk(pPlayer->pos, move, moveCurve, DISK_TYPE_NORMAL, nIdxPlayer);
					PlaySound(SOUND_LABEL_SE_THROW_NORMAL);
				}
				else
				{
					DISK_TYPE special = (DISK_TYPE)((rand() / RAND_MAX) * (DISK_TYPE_SPECIAL_4 - DISK_TYPE_SPECIAL_0) + DISK_TYPE_SPECIAL_0);
					ThrowDisk(pPlayer->pos, move * 2.0f, ZERO_VECTOR, special, nIdxPlayer);
					PlaySound(SOUND_LABEL_SE_THROW_SPECIAL);
				}
			}
			else if (GetJoypadTrigger(JOYKEY_B, nIdxPlayer))
			{
				ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_LOB, nIdxPlayer);
				PlaySound(SOUND_LABEL_SE_THROW_LOB);
			}
			break;
		case JUMP_NOW:
			if (GetJoypadTrigger(JOYKEY_A, nIdxPlayer))
			{
				ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_JUMP, nIdxPlayer);
				PlaySound(SOUND_LABEL_SE_THROW_NORMAL);
			}
			break;
		case JUMP_MAX:
		default:
			assert(false);
			break;
		}
	}
	else
	{ // キーボード入力
		switch (pPlayer->jumpstate)
		{
		case JUMP_NONE:
			if (GetKeyboardTrigger(DIK_RETURN))
			{
				if (pPlayer->nSpecialSkillCnt <= 40)
				{
					ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_NORMAL, nIdxPlayer);
					PlaySound(SOUND_LABEL_SE_THROW_NORMAL);
				}
				else
				{
					DISK_TYPE special = (DISK_TYPE)((int)((rand() / (float)RAND_MAX) * (DISK_TYPE_SPECIAL_4 - DISK_TYPE_SPECIAL_0) + DISK_TYPE_SPECIAL_0));
					ThrowDisk(pPlayer->pos, move * 2.0f, ZERO_VECTOR, special, nIdxPlayer);
					PlaySound(SOUND_LABEL_SE_THROW_SPECIAL);
				}
			}
			else if (GetKeyboardTrigger(DIK_SPACE))
			{
				ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_LOB, nIdxPlayer);
				PlaySound(SOUND_LABEL_SE_THROW_LOB);
			}
			break;
		case JUMP_NOW:
			if (GetKeyboardTrigger(DIK_RETURN))
			{
				ThrowDisk(pPlayer->pos, move, ZERO_VECTOR, DISK_TYPE_JUMP, nIdxPlayer);
				PlaySound(SOUND_LABEL_SE_THROW_NORMAL);
			}
			break;
		case JUMP_MAX:
		default:
			assert(false);
			break;
		}
	}
}

//=========================================
// プレイヤーがディスクを投げる
//=========================================
void ThrowDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acc, DISK_TYPE type, int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];
	SetDisk(pos, move, acc, type, nIdxPlayer, 90.0f);
	pPlayer->bHaveDisk = false;
	s_bCurveInput = false;
}

//=========================================
// 必殺技をチャージする
//=========================================
void ChargePlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];
	LandingMark *pMark = GetLandingMark();

	if (CollisionCircle(pPlayer->pos, pPlayer->fSize, pMark->pos,pMark->fSize * 0.5f) && pPlayer->jumpstate == JUMP_NONE && pMark->bUse)
	{
		pPlayer->nSpecialSkillCnt++;
	}
	else
	{
		pPlayer->nSpecialSkillCnt = 0;
	}
}

//=========================================
// ディスクを受け取る処理
//=========================================
void CatchDiscPlayer(int nIdxPlayer)
{
	Player *pPlayer = &s_player[nIdxPlayer];
	Disk* pDisk = GetDisk();

	// ディスクとプレイヤーの当たり判定
	if ((CollisionCircle(pPlayer->pos, (pPlayer->fSize - 45.0f) * (1 + (pPlayer->fHeight * 0.005f)), pDisk->pos, (pDisk->fSize * 0.5f))) && (pDisk->nPlayer != nIdxPlayer))
	{
		// 床で取得
		if ((pDisk->type != DISK_TYPE_LOB || (pDisk->type == DISK_TYPE_LOB && pDisk->fHeight <= 0.0f)) && pPlayer->jumpstate == JUMP_NONE)
		{
			float fBack = D3DXVec3Length(&pDisk->move);
			D3DXVECTOR3 nomal;
			D3DXVec3Normalize(&nomal, &pDisk->move);
			pPlayer->move += nomal * fBack * 2.5f;

			if (pDisk->type >= DISK_TYPE_SPECIAL_0)
			{
				ColisionGoal(&pPlayer->pos, &(pPlayer->pos + pPlayer->move));
			}

			DestroyDisk();	// ディスクの破棄
			pPlayer->bHaveDisk = true;
			pPlayer->fThrowPower = pPlayer->fMaxThrowPower;
			PlaySound(SOUND_LABEL_SE_CATCH);
		}

		// 空中で取得
		if (pDisk->type == DISK_TYPE_LOB && pPlayer->jumpstate == JUMP_NOW)
		{
			DestroyDisk();	// ディスクの破棄
			pPlayer->bHaveDisk = true;
			pPlayer->fThrowPower = pPlayer->fMaxThrowPower;
			PlaySound(SOUND_LABEL_SE_CATCH);
		}
	}
}

//=========================================
// 移動制限
//=========================================
void MoveLimitPlayer(int nIdxPlayer)
{
	Player* pPlayer = &s_player[nIdxPlayer];

	STAGE_LENGTH stageLength = nIdxPlayer == 0 ? *GetP1StgLng() : *GetP2StgLng();

	if (stageLength.max.y - 10.0f <= pPlayer->pos.y + PLAYER_SIZ)
	{
		pPlayer->pos.y = stageLength.max.y - PLAYER_SIZ - 10.0f;
	}
	if (stageLength.max.x - 10.0f <= pPlayer->pos.x + PLAYER_SIZ)
	{
		pPlayer->pos.x = stageLength.max.x - PLAYER_SIZ - 10.0f;
	}
	if (stageLength.min.y + 10.0f >= pPlayer->pos.y - PLAYER_SIZ)
	{
		pPlayer->pos.y = stageLength.min.y + PLAYER_SIZ + 10.0f;
	}
	if (stageLength.min.x + 10.0f >= pPlayer->pos.x - PLAYER_SIZ)
	{
		pPlayer->pos.x = stageLength.min.x + PLAYER_SIZ + 10.0f;
	}
}

//=========================================
// 頂点座標の更新
//=========================================
void UpdateVtxPosPlayer(int nIdxPlayer)
{
	Player* pPlayer = &s_player[nIdxPlayer];
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pPlayer->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
		pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

	pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
		pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

	pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
		pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

	pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))),
		pPlayer->pos.y - ((pPlayer->fHeight - 10.0f) * 0.5f) + (pPlayer->fSize * (1 + (pPlayer->fHeight * 0.015f))), 0.0f);

	// 頂点バッファをアンロックする
	pPlayer->pVtxBuff->Unlock();
}

//=========================================
// 設定
//=========================================
void SetPlayer(const D3DXVECTOR3& pos, PLAYERTYPE type)
{
	Player *pPlayer = s_player;

	for (int i = 0; i < NUM_PLAYER; i++, pPlayer++)
	{
		if (pPlayer->bUse)
		{
			continue;
		}

		// データの初期化
		ZeroMemory(&s_player[i],sizeof(s_player[0]));

		// 種類別のデータの代入
		s_player[i] = s_playerType[type];

		pPlayer->pos = pos; 			// 位置を初期化
		pPlayer->fSize = PLAYER_SIZ;	// プレイヤーの大きさ
		pPlayer->bUse = true;			// プレイヤーの表示の有無

		// 頂点バッファの生成
		GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pPlayer->pVtxBuff,
			NULL);

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pPlayer->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fSize, pPlayer->pos.y - pPlayer->fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fSize, pPlayer->pos.y - pPlayer->fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fSize, pPlayer->pos.y + pPlayer->fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fSize, pPlayer->pos.y + pPlayer->fSize, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャ座標の設定
		if (i == 0)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}

		// 頂点バッファをアンロックする
		pPlayer->pVtxBuff->Unlock();

		break;
	}

}

//=========================================
// プレイヤーの読み込み処理
//=========================================
void LoadPlayer(void)
{
	char s_aString[256];	// 読み込み用文字列
	int nNumType = 0;		// 読み込んだタイプ数

	//ファイルを開く
	FILE* pFile = fopen(PLAYER_FILE, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", s_aString);

		while (strncmp(s_aString, "SCRIPT", 6) != 0)
		{//スタート来るまで空白読み込む
			s_aString[0] = {};
			fscanf(pFile, "%s", s_aString);
		}

		while (strncmp(s_aString, "END_SCRIPT", 10) != 0)
		{// 文字列の初期化と読み込み

			fscanf(pFile, "%s", s_aString);

			if (strncmp(s_aString, "#", 1) == 0)
			{//これのあとコメント
				fgets(s_aString, sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(s_aString, "TYPESTATE") == 0)
			{// 文字列が一致した場合

				assert(PLAYERTYPE_MAX != nNumType);	// 想定より多いファイル読み込み

				while (strncmp(s_aString, "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", s_aString);	// =の読み込み

					if (strcmp(s_aString, "TEXTURE") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%s", s_aString);

						//テクスチャの読み込み
						D3DXCreateTextureFromFile(GetDevice(), s_aString, &s_playerType[nNumType].pTexture);
					}
					else if (strcmp(s_aString, "MOVESPEED") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fMoveSpeed);
					}
					else if (strcmp(s_aString, "MOVE_ATTENUATION") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fAttenuationMoveSpead);
					}
					else if (strcmp(s_aString, "SLIDING_ATTENUATION") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fAttenuationSlidingSpead);
					}
					else if (strcmp(s_aString, "THROW_POWER") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fMaxThrowPower);
					}
					else if (strcmp(s_aString, "THROW_CURVE") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fThrowCurvePower);
					}
					else if (strcmp(s_aString, "SLIDING_VOLUME") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%f", &s_playerType[nNumType].fSlidingVolume);
					}
					else if (strcmp(s_aString, "SLIDING_RIGOR") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%d", &s_playerType[nNumType].nSlidingRigorMax);
					}
					else if (strcmp(s_aString, "SPECIALSKILL_TYPE") == 0)
					{
						fscanf(pFile, "%s", s_aString);	// =の読み込み
						fscanf(pFile, "%d", &s_playerType[nNumType].SpecialSkillType);
					}
				}

				nNumType++;
			}
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}

//-----------------------------------------------------------------------------
// 開始位置に戻る
//-----------------------------------------------------------------------------
bool ResetPosPlayer(void)
{
	D3DXVECTOR3 posDist;
	D3DXVECTOR3 vec;

	for (int nIdxPlayer = 0; nIdxPlayer < 2; nIdxPlayer++)
	{
		if (nIdxPlayer == 0)
		{
			posDist = D3DXVECTOR3(200.0f, SCREEN_HEIGHT * 0.5f, 0.0f);
		}
		else
		{
			posDist = D3DXVECTOR3(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT * 0.5f, 0.0f);
		}

		Player* pPlayer = &s_player[nIdxPlayer];

		posDist -= pPlayer->pos;

		D3DXVec3Normalize(&vec, &posDist);

		pPlayer->move = vec * 5.0f;

		if (D3DXVec3Length(&posDist) < D3DXVec3Length(&pPlayer->move))
		{
			pPlayer->move = posDist;
		}
	}

	return (D3DXVec3Length(&posDist) == D3DXVec3Length(&s_player[0].move)) && (D3DXVec3Length(&posDist) == D3DXVec3Length(&s_player[1].move));
}

//-----------------------------------------------------------------------------
//円の当たり判定
//-----------------------------------------------------------------------------
static bool CollisionCircle(D3DXVECTOR3 Pos1, float fRadius1, D3DXVECTOR3 Pos2, float fRadius2)
{
	float fDiff = fRadius1 + fRadius2;		//2個の物体の半径同士の和
	float fCalculationX = Pos1.x - Pos2.x;	//Xの差分
	float fCalculationY = Pos1.y - Pos2.y;	//Yの差分

	//現在の２点の距離
	float fLength = sqrtf(fCalculationX * fCalculationX + fCalculationY * fCalculationY);

	return fDiff >= fLength;
}

//-----------------------------------------------------------------------------
// 入力
//-----------------------------------------------------------------------------
D3DXVECTOR3 InputMovePlayer(int nIdxPlayer)
{
	D3DXVECTOR3 inputVec = ZERO_VECTOR;

	if (IsJoyPadUse(nIdxPlayer))
	{ // JoyPad入力

		inputVec = GetJoypadStick(JOYKEY_LEFT_STICK, nIdxPlayer);

		if (GetJoypadPress(JOYKEY_UP))
		{
			inputVec.y -= 1.0f;
		}
		if (GetJoypadPress(JOYKEY_LEFT))
		{
			inputVec.x -= 1.0f;
		}
		if (GetJoypadPress(JOYKEY_DOWN))
		{
			inputVec.y += 1.0f;
		}
		if (GetJoypadPress(JOYKEY_RIGHT))
		{
			inputVec.x += 1.0f;
		}
	}
	else
	{ // キーボード入力
		switch (nIdxPlayer)
		{
		case 0:
			if (GetKeyboardPress(DIK_W))
			{
				inputVec.y -= 1.0f;
			}
			if (GetKeyboardPress(DIK_A))
			{
				inputVec.x -= 1.0f;
			}
			if (GetKeyboardPress(DIK_S))
			{
				inputVec.y += 1.0f;
			}
			if (GetKeyboardPress(DIK_D))
			{
				inputVec.x += 1.0f;
			}
			break;
		case 1:
			if (GetKeyboardPress(DIK_UP))
			{
				inputVec.y -= 1.0f;
			}
			if (GetKeyboardPress(DIK_LEFT))
			{
				inputVec.x -= 1.0f;
			}
			if (GetKeyboardPress(DIK_DOWN))
			{
				inputVec.y += 1.0f;
			}
			if (GetKeyboardPress(DIK_RIGHT))
			{
				inputVec.x += 1.0f;
			}
			break;
		default:
			break;
		}
	}

	D3DXVec3Normalize(&inputVec, &inputVec);
	return inputVec;
}

//=========================================
// プレイヤーの取得処理
//=========================================
Player* GetPlayer(void)
{
	return s_player;
}