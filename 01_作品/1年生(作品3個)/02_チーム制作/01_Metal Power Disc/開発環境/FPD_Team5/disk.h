//================================//
//								  //
//       ディスクのヘッダー		  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _DISK_H_		//このマクロが定義されてなかったら
#define _DISK_H_		//2重インクルード防止のマクロ定義

#include "main.h" 

//マクロ定義
#define MAX_DISK					(1)					//ディスクの最大数
#define LOB_STARTING_SPEED			(5.0f)				//上投げのはじめの上下移動量
#define NORMAL_VERTICAL_SPEED		(0.0f)				//ディスクの普通の上下移動量
#define NORMAL_DISK_HEIGHT			(15.0f)				//ディスクの普通高さ
#define JUMP_MAX_HEIGHT				(260.0f)			//ジャンプした後の最大高さ
#define JUMP_ATTACK_TIME			(60.0f)				//ジャンプ投げの時、ディスクが落ちるまでの時間
#define GRAVITY_ACCELERATION	(-0.1f)					//重量の加速
#define BASE_ROTATION_SPEED			(-D3DX_PI * 0.01f)	//ベースの回転スピード

//ディスクの種類
typedef enum
{
	DISK_TYPE_NORMAL = 0,			//普通
	DISK_TYPE_LOB,					//上投げ
	DISK_TYPE_JUMP,					//ジャンプ投げ
	DISK_TYPE_BLOCKED,
	DISK_TYPE_SPECIAL_0,			//必殺技0
	DISK_TYPE_SPECIAL_1,			//必殺技1
	DISK_TYPE_SPECIAL_2,			//必殺技2
	DISK_TYPE_SPECIAL_3,			//必殺技3
	DISK_TYPE_SPECIAL_4,			//必殺技4
	DISK_TYPE_MAX
}DISK_TYPE;

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;						//ディスクの現在の位置
	D3DXVECTOR3 lastPos;					//ディスクの前回の位置
	D3DXVECTOR3 move;						//ディスクの移動量
	D3DXVECTOR3 acc;						//ディスクの加速
	DISK_TYPE type;							//ディスクの種類
	float fSize;							//ディスクの大きさ
	float fHeight;							//上投げ用の高さ
	float fVerticalSpeed;					//上投げの移動量
	float fAngle;							//回転用の角度
	float fLenght;							//ディスクの半径
	float fVertexLenght;					//回転アニメーション用の長さ
	float rot;								//回転角度
	float fRotSpeed;						//回転スピード
	int nCntPhase;							//必殺技用のカウンター
	int nPlayer;							//ディスクを投げたプレイヤーのインデックス
	int nCntAnim;							//アニメーションカウンター
	int nAnimPattern;						//アニメーションのパターン番号
	int nIdxShadow;							//ディスクの影のインデックス
	bool bBounce;							//壁に当たったら、反射するかどうか
	bool bUse;								//使用されているかどうか
}Disk;

//プロトタイプ宣言
void InitDisk(void);
void UninitDisk(void);
void UpdateDisk(void);
void DrawDisk(void);
void SetDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acc, DISK_TYPE type, int nPlayer, float size);
void UpdateSpecialDisk(int nCntDisk);
D3DXVECTOR3 SetLobSpeed(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nCntDisk, float fHeight, float fVerticalSpeed);
D3DXVECTOR3 SetJumpAttackSpeed(D3DXVECTOR3 pos);
void DestroyDisk(void);

Disk *GetDisk(void);

#endif