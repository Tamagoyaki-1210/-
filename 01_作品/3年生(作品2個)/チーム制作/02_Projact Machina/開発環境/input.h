//=============================================================================
//
// 入力処理 [input.h]
// Author : KOZUNA HIROHITO
// 
//=============================================================================

#ifndef _INPUT_H_		//このマクロ定義がされなかったら
#define _INPUT_H_		//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "inputkeydata.h"
#include "DirectInput.h"
#include <vector>

//----------------------------------------------------------------------------
//前方宣言
//----------------------------------------------------------------------------
class CInputKeyboard;
class CInputJoyPad;
class CInputMouse;

//----------------------------------------------------------------------------
//クラス定義
//----------------------------------------------------------------------------
class CInput
{
public:

	CInput();
	~CInput();

	//*アプリケーション処理に書くやつ
	static CInput *Create();							//入力処理系のクリエイト、Initの前に書く
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//入力処理全部の初期化
	void Uninit();										//入力処理全部の終了処理
	void Update();										//入力処理全部の更新処理
	//*

	//*インプットが必要な時呼び出す
	static CInput *GetKey() { return m_pInput; }		//プレイやトリガーなどのアドレスの取得

	/* 総合 */
	bool Press(STAN_DART_INPUT_KEY key, int nKeyLock) { return KeyChackAll(key, nKeyLock, 1); }		// プレス
	bool Trigger(STAN_DART_INPUT_KEY key, int nKeyLock) { return KeyChackAll(key, nKeyLock, 2); }	// トリガー
	bool Release(STAN_DART_INPUT_KEY key, int nKeyLock) { return KeyChackAll(key, nKeyLock, 3); }	// リリース

	bool Press(STAN_DART_INPUT_KEY key, int nKeyLock, int nNum) { return KeyChackNum(key, nKeyLock, 1, nNum); }		// プレス
	bool Trigger(STAN_DART_INPUT_KEY key, int nKeyLock, int nNum) { return KeyChackNum(key, nKeyLock, 2, nNum); }	// トリガー
	bool Release(STAN_DART_INPUT_KEY key, int nKeyLock, int nNum) { return KeyChackNum(key, nKeyLock, 3, nNum); }	// リリース

	//本ゲーム専用
	bool MovePress(BOOK_GAME_DEDICATED_MOVE_KEY key,int nKeyLock = 0);


	D3DXVECTOR3 VectorMove(); //移動用ベクトルの取得

	// 入力しているデバイスを返す
	std::vector<int> PressDevice(STAN_DART_INPUT_KEY key);		// プレス
	std::vector<int> TriggerDevice(STAN_DART_INPUT_KEY key);	// トリガー
	std::vector<int> ReleaseDevice(STAN_DART_INPUT_KEY key);	// リリース

	/* Keyboard */
	bool Press(int nKey, int nKeyLock = 0,int num = -1);								//プレス
	bool Trigger(int nkey, int nKeyLock = 0, int num = -1);								//トリガー
	bool Release(int nkey, int nKeyLock = 0, int num = -1);								//リリース

	/* JoyPad */
	bool Press(DirectJoypad key, int nKeyLock = 0, int nNum = 0);			//ジョイパットプレス
	bool Trigger(DirectJoypad key, int nKeyLock = 0, int nNum = 0);		//ジョイパットトリガー
	bool Release(DirectJoypad key, int nKeyLock = 0, int nNum = 0);		//ジョイパットリリース
	bool StickPress(JOYKEY_DIRECT_CROSS key, int nNum = 0, bool bleftandright = false, float frot = 0.25f,int nKeyLock = 0); //スティックのプレス

	D3DXVECTOR3 VectorMoveKey(int nNum);				//十字キー式のベクトル取得
	D3DXVECTOR3 VectorMoveJoyStick(int nNum = 0, bool bleftandright = false); //ジョイスティックのベクトル取得
	D3DXVECTOR3 VectorMoveJoyStickAll(bool bleftandright = false); //ジョイスティックのベクトル取得

	/* Mouse */
	bool Press(MOUSE_KEY key, int nKeyLock = 0);				//プレス
	bool Trigger(MOUSE_KEY key, int nKeyLock = 0);			//トリガー
	bool Release(MOUSE_KEY key, int nKeyLock = 0);			//リリース

	D3DXVECTOR3 VectorMoveKeyAll();			//十字キー式のベクトル取得

	D3DXVECTOR3 GetMouseCursor(void);		// マウスカーソルのスクリーン座標の取得
	int GetMouseWheel(void);				// マウスのホイールの動き処理
	D3DXVECTOR3 GetMouseMove(void);			// マウスの移動量を出力処理
	void SetCursorErase(bool bUse);			// 画面内のカーソルを消すかどうか

	InputType GetOldInputType() { return m_nOldInputType; }		//最後に触ったデバイス
																
	void SetJoypadKeyConfig(int nPlayerNum, DirectJoypad OldKey, DirectJoypad NewKey); //キーの入れ替え

	bool IsConnected(int nNum);	//ジョイパッドが接続されているかどうか
	int GetIsConnectedNumMax(); //ジョイパッドの接続数

	void IntermediateReception(bool bIR = false);	//デバイスの途中検知のオンオフ
	bool GetIntermediateReception();		//現在デバイスの途中検知を行っているかどうか

	void SetKeyLock(int nKeyLock); //キーの設定(上書き設定はエラーを表示)
	void UnlockKey(int nKeyLock); //キーの解除

private:
	bool KeyChackAll(STAN_DART_INPUT_KEY key, int type, int nKeyLock);			// 全デバイスの入力を確認
	bool KeyChackNum(STAN_DART_INPUT_KEY key, int type, int nKeyLock, int nNum);	// 指定したデバイスの入力を確認
private:

	CInputKeyboard *m_pKeyboard;	//キーボードの情報
	CInputJoyPad *m_pJoyPad;		//ジョイパッドの情報
	CInputMouse *m_pMouse;			//マウスの情報
	static CInput *m_pInput;		//このクラスの情報
	InputType m_nOldInputType;		//最後に触ったデバイス
	int m_nKeyLock;					//キーがしてされているときキーをもっていない処理からの入力を無視する

};

#endif

