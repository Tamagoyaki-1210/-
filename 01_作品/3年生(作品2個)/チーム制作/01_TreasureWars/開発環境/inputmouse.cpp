//=============================================================================
//
// üÍ [directmouse.h]
// Author1 : ú±l
// Author2 : KOZUNA HIROHITO
// Tv : }EX¶¬ðs¤
//
//=============================================================================

//***************************************************************************
// CN[h
//***************************************************************************
#include <assert.h>
#include <stdio.h>
#include "main.h"
#include "inputmouse.h"

//=============================================================================
// RXgN^
// Author : ú±l
// Tv : CX^X¶¬És¤
//=============================================================================
CInputMouse::CInputMouse()
{
	memset(&m_aKeyState, 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	memset(&m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));

	m_fHeightWnd = 0.0f;
	m_fWidthWnd = 0.0f;
	m_bCursorErase = true;
}

//=============================================================================
// fXgN^
// Author : ú±l
// Tv : CX^XI¹És¤
//=============================================================================
CInputMouse::~CInputMouse()
{

}

//=============================================================================
// ú»
// Author1 : ú±l
// Author2 : KOZUNA HIROHITO
// Tv : }EXÌANZX ðl¾Å«½êAðp±·é
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{

	// üÍfoCXi}EXjÌ¶¬
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//f[^tH[}bgðÝè
	if (FAILED(m_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// ¦²[hðÝè
	if (FAILED(m_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// EBhEnhÌi[
	m_hWnd = hWnd;

	// DeviceÖÌANZX ðl¾
	m_pDevMouse->Acquire();

	//EBhEÌ¡Æ³Ìæ¾
	m_fWidthWnd = SCREEN_WIDTH;
	m_fHeightWnd = SCREEN_HEIGHT;

	return S_OK;
}

//=============================================================================
// I¹
// Author : KOZUNA HIROHITO
// Tv : }EXÌANZX ððúµAfoCXððú·é
//=============================================================================
void CInputMouse::Uninit(void)
{
	//üÍfoCXiL[{[hjÌúü
	if (m_pDevMouse != nullptr)
	{
		m_pDevMouse->Unacquire();		//L[{[hÖÌANZX ðúü
		m_pDevMouse->Release();
		m_pDevMouse = nullptr;
	}
}

//=============================================================================
// XV
// Author1 : ú±l
// Author2 : KOZUNA HIROHITO
// Tv : }EXüÍ³ê½îñðÛ¶·é
//=============================================================================
void CInputMouse::Update(void)
{
	// Ïé¾
	DIMOUSESTATE2 aKeyState;		// }EXüÍîñ
	int nCntKey;

	// üÍfoCX©çf[^ðæ¾
	if (SUCCEEDED(m_pDevMouse->GetDeviceState(sizeof(aKeyState), &aKeyState)))
	{
		for (nCntKey = 0; nCntKey < MAX_MOUSE_KEY; nCntKey++)
		{
			// }EXgK[îñðÛ¶
			m_aKeyStateTrigger.rgbButtons[nCntKey] = ~m_aKeyState.rgbButtons[nCntKey] & aKeyState.rgbButtons[nCntKey];

			// }EX[XîñðÛ¶
			m_aKeyStateRelease.rgbButtons[nCntKey] = m_aKeyState.rgbButtons[nCntKey] & ~aKeyState.rgbButtons[nCntKey];
			
		}

		// }EXvXîñðÛ¶
		m_aKeyState = aKeyState;
	}
	else
	{
		// }EXÖÌANZX ðl¾
		m_pDevMouse->Acquire();
	}
}

//=============================================================================
//  }EXvXîñÌæ¾
// Author1 : ú±l
// Author2 : KOZUNA HIROHITO
// Tv : øÌL[ÌîñðÔ·
//=============================================================================
bool CInputMouse::GetPress(MOUSE_KEY eKey)
{
	return (m_aKeyState.rgbButtons[eKey] & 0x80) ? true : false;
}

//=============================================================================
// }EXgK[îñÌæ¾
// Author1 : ú±l
// Author2 : KOZUNA HIROHITO
// Tv : øÌL[ÌîñðÔ·
//=============================================================================
bool CInputMouse::GetTrigger(MOUSE_KEY eKey)
{
	return (m_aKeyStateTrigger.rgbButtons[eKey] & 0x80) ? true : false;
}

//=============================================================================
// }EX[XîñÌæ¾
// Author1 : ú±l
// Author2 : KOZUNA HIROHITO
// Tv : øÌL[ÌîñðÔ·
//=============================================================================
bool CInputMouse::GetRelease(MOUSE_KEY eKey)
{
	return (m_aKeyStateRelease.rgbButtons[eKey] & 0x80) ? true : false;
}

//=============================================================================
// SL[vXÌîñÌæ¾
// Author : KOZUNA HIROHITO
// Tv : øÌL[ÌîñðÔ·
//=============================================================================
bool CInputMouse::GetPressAll()
{
	for (int nCnt = 0; nCnt < MAX_MOUSE_KEY; nCnt++)
	{
		if (GetPress((MOUSE_KEY)nCnt))
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// SL[vXÌîñÌæ¾
// Author : KOZUNA HIROHITO
// Tv : øÌL[ÌîñðÔ·
//=============================================================================
bool CInputMouse::GetTriggerAll()
{
	for (int nCnt = 0; nCnt < MAX_MOUSE_KEY; nCnt++)
	{
		if (GetTrigger((MOUSE_KEY)nCnt))
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// SL[vXÌîñÌæ¾
// Author : KOZUNA HIROHITO
// Tv : øÌL[ÌîñðÔ·
//=============================================================================
bool CInputMouse::GetReleaseAll()
{
	for (int nCnt = 0; nCnt < MAX_MOUSE_KEY; nCnt++)
	{
		if (GetRelease((MOUSE_KEY)nCnt))
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// }EXJ[\ÌÊuÌæ¾
// Author1 : ú±l
// Author2 : KOZUNA HIROHITO
// Tv : }EXJ[\ÌÊuðæ¾µAlðÔ·
//=============================================================================
D3DXVECTOR3 CInputMouse::GetMouseCursor(void)
{
	// XN[ÀWãÌ}EXJ[\ÌÊuÌæ¾
	GetCursorPos(&m_Pos);

	// EBhEãÌ}EXJ[\ÌÊuÌæ¾
	ScreenToClient(m_hWnd, &m_Pos);

	return D3DXVECTOR3((float)m_Pos.x, (float)m_Pos.y, 0.0f);
}

//=============================================================================
// }EXzC[ÌÚ®Êæ¾
// Author : ú±l
// Tv : }EXzC[ÌÚ®ÊÌlðÔ·
//=============================================================================
int CInputMouse::GetMouseWheel(void)
{
	return (int)m_aKeyState.lZ;
}

//=============================================================================
// }EXÌÚ®Êæ¾
// Author : ú±l
// Tv : }EXÌÚ®ÊÌlðÔ·
//=============================================================================
D3DXVECTOR3 CInputMouse::GetMouseMove(void)
{
	return D3DXVECTOR3((float)(m_aKeyState.lX), (float)(m_aKeyState.lY), (float)(m_aKeyState.lZ));
}