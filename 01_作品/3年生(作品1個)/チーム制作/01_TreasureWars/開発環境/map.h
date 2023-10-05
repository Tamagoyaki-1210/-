//==============================================================================
//
// Map.h
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2�d�C���N���[�h�K�[�h
//==============================================================================
#ifndef _MAP_H_
#define	_MAP_H_

//==============================================================================
// �C���N���[�h
//==============================================================================
#include "main.h"
#include"trolley_manager.h"
#include <list>

//==============================================================================
// �O���錾
//==============================================================================
class CObjectX;
class C3DPolygon;

//==============================================================================
// �}�b�v�N���X
//==============================================================================
class CMap
{
	static const int LINE_MAX_READING_LENGTH = 256;
	static const char* MAP_OBSTACLE_OBJECT_FILENAME[];		// �t�@�C���p�X
	static const char* MAP_BACKGROUND_OBJECT_FILENAME[];	// �t�@�C���p�X
	static const float Map_Collision_Decreace;				// �}�b�v�I�u�W�F�N�g�̓����蔻�������������l
	
public:
	CMap();						// �R���X�g���N�^
	~CMap();					// �f�X�g���N�^

	HRESULT Init(int nMopNum);				// ����������
	void Uninit();				// �I������
	void Update();				// �X�V����
	void Draw();				// �`�揈��

	D3DXVECTOR3 CollisionMapObject(const D3DXVECTOR3 pos, const float radius);

	void LoadMap(bool bCollision, const char* s_FileName);			// �}�b�v�ǂݍ���

	void Trolley_Route();		// �g���b�R�̃��[�g�ݒ�

	static CTrolley_Manager *GetTrolleyManager() { return m_pTrolley_Manager; }	// �g���b�R�}�l�[�W���[

private:
	C3DPolygon *m_pFloor;			//�@��
	std::list<CObjectX*> m_MapObjectList;		// �I�u�W�F�N�gX�̃C���X�^���X
	int m_nMapSizeMax;					//�@�}�b�v�̍ő啝

	int m_nMapMax;
	//int *m_pMapTypeData;
	int m_nMapTypeMax;
	CObjectX *m_pMapMadelData;
	int m_nMapNum;

	static CTrolley_Manager *m_pTrolley_Manager;
};

#endif