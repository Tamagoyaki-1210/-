//=============================================================================
//
// debugProc.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifdef _DEBUG

#include "debugProc.h"
#include "application.h"
#include "renderer.h"

LPD3DXFONT CDebugProc::m_pFont = nullptr;
std::string CDebugProc::m_str = "";

//=====================================
// デフォルトコンストラクタ
//=====================================
CDebugProc::CDebugProc()
{

}

//=====================================
// デストラクタ
//=====================================
CDebugProc::~CDebugProc()
{

}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CDebugProc::Init()
{
	//初期化処理
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();		//デバイスの取得

																				// デバッグ情報表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CDebugProc::Uninit()
{
	//フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//============================================================================
// 文字の入力処理
//============================================================================
void CDebugProc::Print(const char *pFormat, ...)
{
	va_list args;

	va_start(args, pFormat);

	// 文字列の最後まで読み込む処理
	for (const char* pPointer = pFormat; *pPointer != '\0'; pPointer++)
	{
		// %では無い場合文字列に追加する
		if (*pPointer != '%')
		{
			m_str += *pPointer;
		}
		else
		{
			pPointer++;	// ポインタを進める

						// 次の文字の種類で型を決める処理
			switch (*pPointer)
			{

			case 'd':
			{
				m_str += std::to_string(va_arg(args, int));
			}
			break;

			case 'f':
			{
				m_str += std::to_string(va_arg(args, double));
			}
			break;

			case 'c':
			{
				m_str += std::to_string(va_arg(args, char));
			}
			break;

			case 's':
			{
				m_str += va_arg(args, char*);
			}
			break;

			default:
			{
				m_str += '%';
				m_str += *pPointer;
			}
			break;

			}
		}
	}

	va_end(args);
}

//============================================================================
// 描画処理
//============================================================================
void CDebugProc::Draw()
{
	RECT rect = { 0, 0, 1280, 780 };

	TCHAR str[Str_Max];

	// 文字列が最大文字数より少ない場合続ける
	if (m_str.length() < Str_Max)
	{
		sprintf(str, _T(m_str.c_str()));

		// テキスト描画
		m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0xff));
	}

	m_str.clear();
}

//============================================================================
// 生成処理
//============================================================================
CDebugProc* CDebugProc::Create()
{
	CDebugProc* pDebug = new CDebugProc;

	if (FAILED(pDebug->Init()))
	{
		return nullptr;
	}

	return pDebug;
}

#endif