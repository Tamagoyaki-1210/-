//=============================================================================
//
// fog.h (�t�H�O.h)
// Author : Saito Shian
//
//=============================================================================
#ifndef  _FOG_H_				//���̃}�N����`������ĂȂ�������
#define  _FOG_H_				//��d�C���N���[�h�h�~�̃}�N����`

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "main.h"
#include "object.h"

#include <d3dx9.h>
#include <tchar.h> // _T
#include <assert.h>

//=============================================================================
// �O���錾
//=============================================================================

//=============================================================================
// �t�H�O�N���X
//=============================================================================
class CFog
{
public:
	CFog();				// �t�H�O�̃R���X�g���N�^
	~CFog();			// �t�H�O�̃f�X�g���N�^

	static void SetFog(D3DXCOLOR col, const float intensity = 0.00005f);	// �t�H�O�̐ݒ菈��
	static void DestroyFog();			// �t�H�O�̏I������

private:
	static bool m_bCreate;
	static bool m_bIntensityFlag;
	static float m_fIntensity;
	static float m_fIntensityDest;

	static float m_fFogStartPos;
	static float m_fFogEndPos;
};
#endif