//=============================================================================
//
// 入力処理 [directjoypad.h]
// Author1 : KOZUNA HIROHITO
//
//=============================================================================

#ifndef _INPUTJOYPAD_H_		//このマクロ定義がされなかったら
#define _INPUTJOYPAD_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//インクルードファイル
//----------------------------------------------------------------------------
#include "DirectInput.h"
#include "inputkeydata.h"

//----------------------------------------------------------------------------
//クラス定義
//----------------------------------------------------------------------------
class CInputJoyPad : public CDirectInput
{

private:
	static const int MAX_JOY_KEY = 32;		//ジョイパッドの使ってないキーを含めた最大数
	static const int JOYPAD_DATA_MAX = 4;	//同時接続可能最大数
	static const int KEY_CONFIG_SUPPORTED_KEY_NUMBER = 13; //キーコンフィグ対応のキー数
	static const int LINE_MAX_READING_LENGTH = 256; //読み込み際の1行当たりの最大文字数
	static const int DEVICE_ADDITIONAL_INTERVAL_EXECUTING_APP = 120; //アプリ実行中のデバイス追加間隔
	static const int DIRECTION_COUNT_OF_STICK_AND_CROSS = 8;//スティックと十字キーの方向数

	//ジョイパッドのひとつに必要な情報の構造体
	struct SJoyPad
	{
		LPDIRECTINPUTDEVICE8 pInputDevice;							//入力デバイスへのポインタ
		DIJOYSTATE aKeyState;										//ジョイパッドのプレス情報
		DIJOYSTATE aKeyStateTrigger;								//ジョイパッドのトリガー情報
		DIJOYSTATE aKeyStateRelease;								//ジョイパッドのリリース情報
		bool bOldKeyTrigger[DIRECTION_COUNT_OF_STICK_AND_CROSS];	//各スティックと十字キーの方向が押されたかどうかトリガー			
		bool bOldKeyRelease[DIRECTION_COUNT_OF_STICK_AND_CROSS];	//各スティックと十字キーの方向が押されたかどうかリリース
		DirectJoypad aOldKeyTrigger;								//前回押されたトリガーキーの種類
		DirectJoypad aOldKeyRelease;								//前回押されたリリースキーの種類
		int nCrossPressRot;											//ジョイパッドの十字キーの押されている方向
	};

public:
	CInputJoyPad();
	~CInputJoyPad()override;
	HRESULT Init(HINSTANCE hInstance, HWND hWnd)override;		//ジョイパッドの初期化
	void Uninit(void)override;									//ジョイパッドの終了処理
	void Update(void)override;									//ジョイパッドの更新処理

	//入力デバイスの登録関数
	HRESULT JoyPadDeviceRegistration(HWND hWnd);

	//入力デバイスへのポインタの取得
	LPDIRECTINPUTDEVICE8 GetInputDevice(int nNum) { return m_JoyPadData[nNum].pInputDevice; }

	//入力デバイスへのポインタの設定 (返り値は登録した際の配列番号)
	int SetInputDevice(LPDIRECTINPUTDEVICE8 pInputDeviceint);

	//現在接続されているジョイパッドの数の取得
	int GetJoyPadNumMax() { return m_nJoyNumCnt; }

	//*接続が確認時に行うコールバック関数
	//デバイスが確認できたら情報を生成するコールバック関数
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);
	//確認が出来たジョイパッドのスティックの傾きの値の幅設定
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext);
	//*

	bool GetPress(DirectJoypad eKey, int nNum);				//プレス処理
	bool GetTrigger(DirectJoypad eKey, int nNum);			//トリガー処理
	bool GetRelease(DirectJoypad eKey, int nNum);			//リリース処理
	bool GetCrossPress(DirectJoypad eKey, int nNum);		//十字キープレス処理
	bool GetCrossTrigger(DirectJoypad eKey, int nNum);	    //十字キートリガー処理
	bool GetCrossRelease(DirectJoypad eKey, int nNum);	    //十字キーリリース処理
	bool GetPressAll(DirectJoypad eKey);			//オールプレス処理	（キー指定あり、プレイヤー指定なし）
	bool GetTriggerAll(DirectJoypad eKey);			//オールトリガー処理（キー指定あり、プレイヤー指定なし）
	bool GetReleaseAll(DirectJoypad eKey);			//オールリリース処理（キー指定あり、プレイヤー指定なし）
	bool GetPressAll(int nNum);						//オールプレス処理	（キー指定なし、プレイヤー指定あり）
	bool GetTriggerAll(int nNum);					//オールトリガー処理（キー指定なし、プレイヤー指定あり）
	bool GetReleaseAll(int nNum);					//オールリリース処理（キー指定なし、プレイヤー指定あり）
	bool GetPressAll();								//オールプレス処理	（キー指定なし、プレイヤー指定なし）
	bool GetTriggerAll();							//オールトリガー処理（キー指定なし、プレイヤー指定なし）
	bool GetReleaseAll();							//オールリリース処理（キー指定なし、プレイヤー指定なし）
	D3DXVECTOR3 GetJoyStickData(int nNum, bool bleftandright = false);			//ジョイスティックの傾き値を返す(true  = 右、false = 左)
	int GetCross(int nNum = 0) { return m_JoyPadData[nNum].nCrossPressRot; }	//ジョイパッドの十字キーの押されている方向(例　45°など)

	void SetKeyConfig(int nPlayerNum, DirectJoypad OldKey, DirectJoypad NewKey); //キーの入れ替え
	void KeyConfigLoading();			//キーコンフィグの読み込み
	void KeyConfigSave();				//キーコンフィグの保存

	//アプリの実行後にデバイスを登録する際に接続されているデバイス数の一時保存のSet,Get,Add
	void SetAfterAppExecutionJoyNumCnt(int nCnt) { m_nAfterAppExecutionJoyNumCnt = nCnt; }
	int GetAfterAppExecutionJoyNumCnt() { return m_nAfterAppExecutionJoyNumCnt; }
	void AddAfterAppExecutionJoyNumCnt() { m_nAfterAppExecutionJoyNumCnt++; }

	//一時保存用デバイスポインタの配列、Set
	void SetDevicePointer(LPDIRECTINPUTDEVICE8 pDevice) { m_apDevice[m_nAfterAppExecutionJoyNumCnt] = pDevice; m_nAfterAppExecutionJoyNumCnt++; }

	void IntermediateReception(bool bIR = false) { m_bIntermediateReception = bIR; }	//デバイスの途中検知のオンオフ
	bool GetIntermediateReception() { return m_bIntermediateReception; }		//現在デバイスの途中検知を行っているかどうか

private:
	SJoyPad m_JoyPadData[JOYPAD_DATA_MAX];		//ジョイパッドのひとつに必要な情報の構造体
	DirectJoypad m_AllOldKeyTrigger;			//全ジョイパッド共通の前回されたトリガーキー
	DirectJoypad m_AllOldKeyRelease;			//全ジョイパッド共通の前回されたリリースキー
	int m_nJoyNumCnt;							//現在接続の接続数
	int m_nAfterAppExecutionJoyNumCnt;			//アプリの実行後にデバイスを登録する際に接続されているデバイス数の一時保存
	DirectJoypad m_KeyConfig[JOYPAD_DATA_MAX][KEY_CONFIG_SUPPORTED_KEY_NUMBER]; //キーコンフィグの対応用（この変数内の数値によって認識するキーが変わる）
	HWND m_hWnd;								//ウィンドウハンドルの保存
	int m_nDeviceAdditionalIntervalExecutingApp;//アプリ実行中のデバイス追加間隔
	LPDIRECTINPUTDEVICE8 m_apDevice[JOYPAD_DATA_MAX]; //デバイスポインタの一時保存場所
	bool m_bIntermediateReception;	//途中デバイス受付
};
#endif