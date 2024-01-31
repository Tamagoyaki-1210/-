//==============================================================================================
//
// フェード　　　fade.h
// tutida ryousei
//
//==============================================================================================
#ifndef _FADE_H_
#define	_FADE_H_

#include"object2D.h"
#include"application.h"

class CFade
{
public:
	//フェードの状態
	enum FADE
	{
		FADE_NONE = 0,		//何もしてない状態
		FADE_IN,			//フェードイン
		FADE_OUT,			//フェードアウト
		FADE_MAX
	};

	CFade();
	~CFade();
	 
	HRESULT Init();
	void Uninit();
	void Update();

	static CFade *CFade::Create();
	static void SetFade(CApplication::MODE mode, float fadespeed);

	//EObjType GetObjType() override { return OBJTYPE_MODE; }
	static FADE GetFade() { return FadeMode; }

private:
	D3DXVECTOR3 m_Pos;					// 位置
	float m_fAlpha;						// アルファ
	static bool m_bCreate;				// 生成したか
	static float m_fFadeSpeed;			// フェードの速度
	static FADE FadeMode;				// フェードの状態
	static CApplication::MODE NextMode;	// 次の画面
	static CObject2D* m_pFadeObj;				// フェード用の2Dオブジェクト

};

#endif // !_FADE_H_
