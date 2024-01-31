//=============================================================================
//
// fontString.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "fontString.h"
#include "Application.h"

//=====================================
// デフォルトコンストラクタ
//=====================================
CFontString::CFontString()
{

}

//=====================================
// デストラクタ
//=====================================
CFontString::~CFontString()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CFontString::Init(void)
{
	m_fScale = 0.0f;
	m_bScale = false;
	m_bScaleChange = false;
	m_bBlink = false;
	m_bBlinkScale = false;
	m_fBlinkSpeed = 0.0f;
	m_fBlinkAlpha = 1.0f;

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CFontString::Uninit(void)
{
	// 文字オブジェクトが空になるまで終了させる
	while (!m_apFont.empty())
	{
		// 後ろのオブジェクト2Dから消す処理
		m_apFont.back()->Uninit();
		m_apFont.pop_back();
	}

	Release();
}

//=====================================
// 更新処理
//=====================================
void CFontString::Update(void)
{
	// 位置が一致しない場合
	if (m_pos != m_anchor)
	{
		if ((m_pos.x - m_anchor.x < 0.1f) && (m_pos.x - m_anchor.x > -0.1f) && (m_pos.y - m_anchor.y < 0.1f) && (m_pos.y - m_anchor.y > -0.1f))
		{
			m_pos = m_anchor;
		}
		else
		{
			m_pos += (m_anchor - m_pos) * Anchor_Damping;
		}
		SetFont();
	}

	// サイズ変更処理
	if (m_bScale == true)
	{
		SizeScale();
	}

	// フォント全体の点滅処理
	if (m_bBlink == true)
	{
		BlinkColor();
	}
}

//=====================================
// 描画処理
//=====================================
void CFontString::Draw(void)
{

}

//=====================================
// サイズ変更処理
//=====================================
void CFontString::SizeScale(void)
{
	// 文字が拡大している場合
	if (m_bScaleChange == false)
	{
		// 文字を拡大させる
		m_fScale += ScaleSpeed;

		// 文字が一定の値まで拡大した場合
		if (m_fScale > ScaleSize)
		{
			// 文字を最大拡大値に収め、文字を縮小させる
			m_fScale = ScaleSize;
			m_bScaleChange = true;
		}
	}
	// 文字が縮小している場合
	else
	{
		// 文字を縮小させる
		m_fScale -= ScaleSpeed;

		// 文字が一定の値まで縮小した場合
		if (m_fScale < -ScaleSize)
		{
			// 文字を最小拡大値に収め、文字を拡大させる
			m_fScale = -ScaleSize;
			m_bScaleChange = false;
		}
	}

	// 文字の大きさの設定処理
	for (auto pFont : m_apFont) pFont->SetSize(m_sizeYuan.x + m_fScale, m_sizeYuan.y + m_fScale);
}

//=====================================
// フォント全体の点滅処理
//=====================================
void CFontString::BlinkColor()
{
	for (auto pFont : m_apFont) pFont->SetAlpha(m_fBlinkAlpha);

	// 点の増減判定
	m_fBlinkAlpha += m_bBlinkScale ? m_fBlinkSpeed : -m_fBlinkSpeed;

	if (m_fBlinkAlpha >= 1.0f)
	{
		// アルファ値の減衰
		m_bBlinkScale = false;
		m_fBlinkAlpha = 1.0f;
	}
	if (m_fBlinkAlpha <= Mimi_Blink)
	{
		// アルファ値の増加
		m_bBlinkScale = true;
		m_fBlinkAlpha = Mimi_Blink;
	}
}

//=====================================
// フォント位置の設定処理
//=====================================
void CFontString::SetFont()
{
	// 文字の元の大きさの設定
	m_sizeYuan = m_size;

	// 文字の位置と大きさを設定
	for (int nCnt = 0; nCnt < (int)m_apFont.size(); nCnt++)
	{
		m_apFont[nCnt]->Setposition(m_pos + D3DXVECTOR3((m_size.x + m_size.x * 0.5f) * nCnt, 0.0f, 0.0f));
		m_apFont[nCnt]->SetSize(m_sizeYuan.x + m_fScale, m_sizeYuan.y + m_fScale);
	}
}

//=====================================
// 点滅の終了処理
//=====================================
void CFontString::BlinkFinish()
{
	// 初期化する
	Init();

	// 文字のアルファ値の設定処理
	for (auto pFont : m_apFont) pFont->SetAlpha(m_fBlinkAlpha);
}

//=====================================
// 点滅の終了処理
//=====================================
void CFontString::SetScale(const bool scale)
{
	m_bScale = scale;	// 文字の拡大処理の設定

	// 拡大処理を終了する場合
	if (m_bScale == false)
	{
		// 文字の大きさの初期化
		m_bScaleChange = false;
		m_fScale = 0.0f;

		// 文字の大きさの設定処理
		for (auto pFont : m_apFont) pFont->SetSize(m_sizeYuan.x + m_fScale, m_sizeYuan.y + m_fScale);
	}
}

//=====================================
// 文字列の設定処理
//=====================================
void CFontString::SetString(const std::string letter, const int priority)
{
	// string型の文字数を取得
	int nTex = strlen(letter.c_str());

	// 文字のメモリの大きさ(bit)
	int nStrSize = 0;
	int nStrSizeOld = 0;

	std::string sLatter;

	float fStrLenPos = 0.0f;	// 文字の横の位置
	D3DXCOLOR col = { 1.0f, 1.0f, 1.0f, 1.0f };
	std::string stdMath;

	int nCounter = 0;	// 何回読み込まれたか

	for (int nLatter = 0; nLatter < nTex; nLatter += nStrSize)
	{
		// 特殊コマンドが読まれた場合(使用例 : <c=220,20,60,255,>テキスト</>)
		if (letter[nLatter] == '<')
		{
			nLatter++;

			// 特殊コマンドが終了するまで読み込む
			while (letter[nLatter] != '>')
			{
				switch (letter[nLatter])
				{
					// 色変更コマンド
				case 'c':
					nLatter += 2;

					for (int nCntColor = 0; nCntColor < 4; nCntColor++)
					{
						while (letter[nLatter] != ',')
						{
							stdMath.push_back(letter[nLatter]);
							nLatter++;
						}
						nLatter++;

						switch (nCntColor)
						{
						case 0:
							col.r = std::stof(stdMath);
							break;
						case 1:
							col.g = std::stof(stdMath);
							break;
						case 2:
							col.b = std::stof(stdMath);
							break;
						case 3:
							col.a = std::stof(stdMath);
							break;
						default:
							break;
						}
						stdMath.clear();
					}
					col = D3DCOLOR_RGBA((int)col.r, (int)col.g, (int)col.b, (int)col.a);

					break;
					// 終了コマンドコマンド
				case '/':
					col = { 1.0f, 1.0f, 1.0f, 1.0f };
					nLatter++;
					break;
				default:
					break;
				}
			}
			nStrSize = 1;
		}
		// 文字を読み込む処理
		else
		{
			// 日本語の場合
			if (letter[nLatter] <= -125 && letter[nLatter] >= -127)
			{
				nStrSize = 2;
			}
			// アルファベットの場合
			else
			{
				nStrSize = 1;
			}

			// 現在位置から1文字進む処理
			sLatter = letter.substr(nLatter, nStrSize);

			// 前回と現在の文字の大きさから位置を調整する
			fStrLenPos += m_size.x * (nStrSize < 2 ? 0.5f : 0.75f);

			// 前回文字が存在する場合場合
			if (nStrSizeOld != 0)
			{
				fStrLenPos += m_size.x * (nStrSizeOld < 2 ? 0.5f : 0.75f);
			}

			// 文字が使用されていない場合
			if ((int)m_apFont.size() <= nCounter)
			{
				// 文字の生成
				m_apFont.push_back(CFont::Create(
					m_pos + D3DXVECTOR3(fStrLenPos, 0.0f, 0.0f),
					m_size,
					sLatter.c_str(),
					priority,
					col
				));
			}
			else
			{
				// 文字の設定
				m_apFont[nCounter]->SetString(sLatter.c_str(), col);
				m_apFont[nCounter]->Setposition(m_pos + D3DXVECTOR3(fStrLenPos, 0.0f, 0.0f));
			}

			// 最初のサイズを登録する
			m_sizeYuan = m_size;

			// 文字数をカウント
			nCounter++;
		}
		// 前回文字の大きさを記録させる
		nStrSizeOld = nStrSize;
	}

	// 使用済みの領域が存在する場合削除
	for (int nCnt = nCounter; nCounter < (int)m_apFont.size(); nCnt++)
	{
		m_apFont.back()->Uninit();
		m_apFont.pop_back();
	}
}

//=====================================
// 生成処理
//=====================================
CFontString* CFontString::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const std::string letter, const int priority)
{
	CFontString* pFontString = new CFontString;
	
	if (FAILED(pFontString->Init()))
	{
		return nullptr;
	}

	pFontString->m_pos = pos;
	pFontString->m_anchor = pos;
	pFontString->m_size = size;

	pFontString->SetString(letter, priority);

	return pFontString;
}