//=============================================================================
//
// font.h
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _FONT_H
#define _FONT_H

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object2D.h"
#include <string>
#include <map>

//---------------------------
//クラス宣言
//---------------------------
class CFont : public CObject_2D
{
public:
	CFont();
	CFont(const int nPriority);
	~CFont() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	static CFont* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const char* letter);			//生成処理

private:

};

#endif // !_FONT_H
