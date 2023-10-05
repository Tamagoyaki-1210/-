//=============================================================================
//
// メッセージ
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//クラス
//---------------------------
class CMessage : public CObject2D
{
public:
	enum Message_Type
	{
		TYPE_THISNUMBER = 0,	//今回のスコア
		TYPE_TITLE,				//タイトルロゴ
		TYPE_PAUSE,				//ポーズ画面
		TYPE_MAX,
	};

	CMessage();
	~CMessage() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CMessage *Create(D3DXVECTOR3 pos, Message_Type type);
	void Load();
private:
	float m_fWidth;			//長さ横
	float m_fHeight;		//長さ縦
	Message_Type m_type;	//ナンバー種別
};

#endif // !_MESSAGE_H_
