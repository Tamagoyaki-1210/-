//============================================================================
//
//�L�[�{�[�h���͏���[input.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//************************************************
//�}�N����`
//************************************************
#define NUM_KEY_MAX		(256)	//�L�[�̍ő吔

//============================================================================
//�v���g�^�C�v�錾
//============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);

#endif